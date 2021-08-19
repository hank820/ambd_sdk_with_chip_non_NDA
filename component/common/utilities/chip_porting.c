#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>
#include <string.h>

#include "platform_opts.h"
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

/*
   module size is 4k, we set max module number as 12;
   if backup enabled, the total module number is 12 + 1*12 = 24, the size is 96k;
   if wear leveling enabled, the total module number is 12 + 2*12 + 3*12 = 36, the size is 288k"
*/
#define DCT_BEGIN_ADDR_MATTER   0x1E0000          /*!< DCT begin address of flash, ex: 0x100000 = 1M */
#define MODULE_NUM              12                /*!< max number of module */
#define VARIABLE_NAME_SIZE      32                /*!< max size of the variable name */
#define VARIABLE_VALUE_SIZE     1860 + 4          /*!< max size of the variable value
                                                  /*!< max value number in moudle = 4024 / (32 + 1860+4) = 2 */
#define ENABLE_BACKUP           1
#define ENABLE_WEAR_LEVELING    0

int32_t initPref(void)
{
    int32_t ret;
    ret = dct_init(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_init fail\n");
    else
        printf("dct_init success\n");

    return ret;
}

int32_t deinitPref(void)
{
    int32_t ret;
    ret = dct_format(DCT_BEGIN_ADDR_MATTER, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, ENABLE_BACKUP, ENABLE_WEAR_LEVELING);
    if (ret != 0)
        printf("dct_format fail\n");
    else
        printf("dct_format success\n");

    return ret;
}

int32_t registerPref(char * ns)
{
    int32_t ret;
    ret = dct_register_module(ns);
    if (ret != 0)
        printf("dct_register_module %s fail\n",ns);
    else
        printf("dct_register_module %s success\n",ns);

    return ret;
}

int32_t clearPref(char * ns)
{
    int32_t ret;
    ret = dct_unregister_module(ns);
    if (ret != 0)
        printf("dct_unregister_module %s fail\n",ns);
    else
        printf("dct_unregister_module %s success\n",ns);

    return ret;
}

int32_t deleteKey(char *domain, char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;

    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    ret = dct_delete_variable(&handle, key);
    if(ret == DCT_ERR_NOT_FIND || ret == DCT_SUCCESS)
        ret = DCT_SUCCESS;
    else
        printf("%s : dct_delete_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

bool checkExist(char *domain, char *key)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t DataLen = 0;
    char Data[VARIABLE_VALUE_SIZE];

    ret = dct_open_module(&handle, key);
    if (ret != DCT_SUCCESS){
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        registerPref(key);
        dct_open_module(&handle, key);
        goto exit;
    }

    memset(Data, 0, sizeof(Data));
    DataLen = sizeof(Data);
    ret = dct_get_variable_new(&handle,key ,Data,&DataLen);
    if(ret == DCT_ERR_NOT_FIND)
        printf("%s not found.\n", key);

exit:
    dct_close_module(&handle);

    return (DCT_ERR_NOT_FIND != ret ? 1 : 0);
}

int32_t setPref_new(char *domain, char *key, uint8_t *value, size_t byteCount)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint32_t prefSize;

    if (byteCount > VARIABLE_VALUE_SIZE-4)
    {
        printf("ERROR : byteCount(%d) large than VARIABLE_VALUE_SIZE(%d)\n", byteCount,VARIABLE_VALUE_SIZE-4);
        goto exit;
    }

    printf("%s : domain=%s, key=%s, byteCount=%d\n",__FUNCTION__,domain, key, byteCount);

    ret = registerPref(key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : registerPref(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    ret = dct_set_variable_new(&handle, key, (char *)value, (uint16_t)byteCount);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_set_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bool_new(char *domain, char *key, uint32_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;

    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        return exit;
    }

    len = sizeof(uint32_t);
    ret = dct_get_variable_new(&handle, key, (char *)val, &len);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_u32_new(char *domain, char *key, uint32_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;

    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    len = sizeof(uint32_t);
    ret = dct_get_variable_new(&handle, key, (char *)val, &len);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_u64_new(char *domain, char *key, uint64_t *val)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t len = 0;

    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    len = sizeof(uint64_t);
    ret = dct_get_variable_new(&handle, key, (char *)val, &len);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_str_new(char *domain, char *key, char * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;

    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    ret = dct_get_variable_new(&handle, key, buf, &_bufSize);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    *outLen = bufSize;

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

int32_t getPref_bin_new(char *domain, char *key, uint8_t * buf, size_t bufSize, size_t *outLen)
{
    dct_handle_t handle;
    int32_t ret = -1;
    uint16_t _bufSize = bufSize;
    ret = dct_open_module(&handle, key);
    if (DCT_SUCCESS != ret)
    {
        printf("%s : dct_open_module(%s) failed\n",__FUNCTION__,key);
        goto exit;
    }

    ret = dct_get_variable_new(&handle, key, (char *)buf, &_bufSize);
    if (DCT_SUCCESS != ret)
        printf("%s : dct_get_variable(%s) failed\n",__FUNCTION__,key);

    *outLen = bufSize;

    dct_close_module(&handle);

exit:
    return (DCT_SUCCESS == ret ? 1 : 0);
}

#ifdef __cplusplus
}
#endif
