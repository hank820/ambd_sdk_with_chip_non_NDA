#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>
#include <string.h>

#include "platform/platform_stdlib.h"
#include "errno.h"
#include <sntp/sntp.h>
#include "dct.h"

#define MICROSECONDS_PER_SECOND    ( 1000000LL )                                   /**< Microseconds per second. */
#define NANOSECONDS_PER_SECOND     ( 1000000000LL )                                /**< Nanoseconds per second. */
#define NANOSECONDS_PER_TICK       ( NANOSECONDS_PER_SECOND / configTICK_RATE_HZ ) /**< Nanoseconds per FreeRTOS tick. */

extern int FreeRTOS_errno;
#define errno FreeRTOS_errno

bool UTILS_ValidateTimespec( const struct timespec * const pxTimespec )
{
    bool xReturn = FALSE;

    if( pxTimespec != NULL )
    {
        /* Verify 0 <= tv_nsec < 1000000000. */
        if( ( pxTimespec->tv_nsec >= 0 ) &&
            ( pxTimespec->tv_nsec < NANOSECONDS_PER_SECOND ) )
        {
            xReturn = TRUE;
        }
    }

    return xReturn;
}

int _nanosleep( const struct timespec * rqtp,
               struct timespec * rmtp )
{
    int iStatus = 0;
    TickType_t xSleepTime = 0;

    /* Silence warnings about unused parameters. */
    ( void ) rmtp;

    /* Check rqtp. */
    if( UTILS_ValidateTimespec( rqtp ) == FALSE )
    {
        errno = EINVAL;
        iStatus = -1;
    }

    if( iStatus == 0 )
    {
        /* Convert rqtp to ticks and delay. */
        if( UTILS_TimespecToTicks( rqtp, &xSleepTime ) == 0 )
        {
            vTaskDelay( xSleepTime );
        }
    }

    return iStatus;
}

int __clock_gettime(struct timespec * tp)
{
	unsigned int update_tick = 0;
	long update_sec = 0, update_usec = 0, current_sec = 0, current_usec = 0;
	unsigned int current_tick = xTaskGetTickCount();

	sntp_get_lasttime(&update_sec, &update_usec, &update_tick);
	//if(update_tick) {
		long tick_diff_sec, tick_diff_ms;

		tick_diff_sec = (current_tick - update_tick) / configTICK_RATE_HZ;
		tick_diff_ms = (current_tick - update_tick) % configTICK_RATE_HZ / portTICK_RATE_MS;
		update_sec += tick_diff_sec;
		update_usec += (tick_diff_ms * 1000);
        current_sec = update_sec + update_usec / 1000000;
        current_usec = update_usec % 1000000;
	//}
	//else {
		//current_sec = current_tick / configTICK_RATE_HZ;
	//}
	tp->tv_sec = current_sec;
    tp->tv_nsec = current_usec*1000;
	//sntp_set_lasttime(update_sec,update_usec,update_tick);
    //printf("update_sec %d update_usec %d update_tick %d tvsec %d\r\n",update_sec,update_usec,update_tick,tp->tv_sec);
}

time_t _time( time_t * tloc )
{
#if 0
    /* Read the current FreeRTOS tick count and convert it to seconds. */
    time_t xCurrentTime = ( time_t ) ( xTaskGetTickCount() / configTICK_RATE_HZ );
#else
    time_t xCurrentTime;
    struct timespec tp;

    __clock_gettime(&tp);
    xCurrentTime = tp.tv_sec;
#endif
    /* Set the output parameter if provided. */
    if( tloc != NULL )
    {
        *tloc = xCurrentTime;
    }

    return xCurrentTime;
}

extern void vTaskDelay( const TickType_t xTicksToDelay );
int _vTaskDelay( const TickType_t xTicksToDelay )
{
	vTaskDelay(xTicksToDelay);

	return 0;
}

#define kPreferencesBeginAddr   0x1F0000    /*!< Preferences begin address of flash, ex: 0x100000 = 1M */
#define kPreferencesKeySize     32          /*!< max size of the variable name */
#define kPreferencesValueSize   64          /*!< max size of the variable value */
#define kPreferencesMagic       0x81958711
#define kPreferencesDomainNum   2           /*!< max number of module */

#define c_read32(POINTER) *((uint32_t*)(POINTER))

enum {
    kPrefsTypeBoolean = 1,
    kPrefsTypeInteger = 2,
    kPrefsTypeString = 3,
    kPrefsTypeBuffer = 4
};

int32_t initPref(char * ns)
{
	int32_t ret;
    ret = dct_init(kPreferencesBeginAddr, kPreferencesDomainNum, kPreferencesKeySize, kPreferencesValueSize, 1, 0);
    ret = dct_register_module(ns);
    if (ret != 0)
    {
        printf("dct_register_module failed\n");
    }
    return ret;
}

int32_t clearPref(char * ns)
{
	int32_t ret;
    ret = dct_unregister_module(ns);
    if (ret != 0)
    {
        printf("dct_unregister_module failed\n");
    }
    return ret;
}
int32_t setPref(char *domain, char *key, uint8_t type, uint8_t *value, size_t byteCount)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint32_t prefSize;
    char buffer[kPreferencesValueSize];
    char *pref = buffer;

    if (byteCount > 63)
        return 0;

	ret = initPref(domain);
	if (DCT_SUCCESS != ret)
		goto exit;

    ret = dct_open_module(&handle, domain);
	if (DCT_SUCCESS != ret)
		goto exit;

    // build pref entry
    prefSize = sizeof(kPreferencesMagic) + 1 + byteCount + ((kPrefsTypeBuffer == type) ? 2 : 0);
    if (prefSize > sizeof(buffer)) {
        ret = DCT_ERR_INVALID;
        goto exit;
    }

    memset(buffer, 0, prefSize);
    pref[0] = (uint8_t)(kPreferencesMagic & 0xFF);
    pref[1] = (uint8_t)((kPreferencesMagic >> 8) & 0xFF);
    pref[2] = (uint8_t)((kPreferencesMagic >> 16) & 0xFF);
    pref[3] = (uint8_t)((kPreferencesMagic >> 24) & 0xFF);
    pref += sizeof(kPreferencesMagic);
    *pref++ = type;
    if (kPrefsTypeBuffer == type) {
        pref[0] = (uint8_t)byteCount;
        pref[1] = (uint8_t)(byteCount >> 8);
        pref += 2;
    }

    memcpy(pref, value, byteCount);

    ret = dct_set_variable_new(&handle,key,buffer,(uint16_t)prefSize);
	if (DCT_SUCCESS != ret)
		goto exit;

exit:
    dct_close_module(&handle);
    return (DCT_SUCCESS == ret ? 1 : 0);

}

int32_t getPref_u32(char *domain, char *key, uint8_t type, uint32_t *val)
{
	dct_handle_t handle;
	int32_t ret = -1;
	uint16_t DataLen = 0;
	char *pref = NULL;
	char Data[64];

	ret = initPref(domain);
	if (DCT_SUCCESS != ret)
		goto bail;

	ret = dct_open_module(&handle, domain);
	if (ret != DCT_SUCCESS){
		printf("dct_open_module failed\n"); // most likely that domain doesn't exist yet
		return ret;
	}

	memset(Data, 0, sizeof(Data));
	DataLen = sizeof(Data);
	ret = dct_get_variable_new(&handle,key,Data,&DataLen);
	if (ret != DCT_SUCCESS){
		ret = DCT_SUCCESS;
		return ret;
	}

	pref = Data;
	if (kPreferencesMagic != c_read32(pref)) {
		ret = -1;
		goto bail;
	}

	pref += sizeof(kPreferencesMagic);

	switch (*pref++) {
		case kPrefsTypeBoolean:
			if ( *pref == '1' )
				*val = 1;
			else
			    *val = 0;
			break;
		case kPrefsTypeInteger:
			//xsmcSetInteger(xsResult, c_read32(pref));
			break;
	}

bail:
	dct_close_module(&handle);
	if (DCT_SUCCESS != ret){
		printf("can't getPref_u32\n");
		return ret;
	}
}

int32_t getPref_str(char *domain, char *key, uint8_t type, char * buf, size_t *outLen)
{
	dct_handle_t handle;
	int32_t ret = -1;
	uint16_t DataLen = 0;
	char *pref = NULL;
	char Data[64];

	ret = dct_open_module(&handle, domain);
	if (ret != DCT_SUCCESS){
		printf("dct_open_module failed\n"); // most likely that domain doesn't exist yet
		return ret;
	}

	memset(Data, 0, sizeof(Data));
	DataLen = sizeof(Data);
	ret = dct_get_variable_new(&handle,key,Data,&DataLen);
	if (ret != DCT_SUCCESS){
		ret = DCT_SUCCESS;
		return ret;
	}

	pref = Data;
	if (kPreferencesMagic != c_read32(pref)) {
		ret = -1;
		goto bail;
	}

	pref += sizeof(kPreferencesMagic);

	switch (*pref++) {
		case kPrefsTypeBoolean:
		case kPrefsTypeInteger:
			break;
		case kPrefsTypeString:
			*outLen = strlen(pref);
			memcpy(buf,pref,strlen(pref)+1);
			break;
	}

bail:
	dct_close_module(&handle);
	if (DCT_SUCCESS != ret){
		printf("can't getPref_str\n");
		return ret;
	}
}

#ifdef __cplusplus
}
#endif