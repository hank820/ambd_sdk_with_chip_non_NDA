/**
 * @brief High resolution sleep.
 *
 * http://pubs.opengroup.org/onlinepubs/9699919799/functions/nanosleep.html
 *
 * @note rmtp is ignored, as signals are not implemented.
 */
#ifdef __cplusplus
extern "C" {
#endif

int _nanosleep( const struct timespec * rqtp, struct timespec * rmtp );
int _vTaskDelay( const TickType_t xTicksToDelay );
time_t _time( time_t * tloc );

// for AMBDConfig
int32_t initPref(void);
int32_t deinitPref(void);
int32_t registerPref(char * ns);
int32_t clearPref(char * ns);
int32_t deleteKey(char *domain, char *key);
bool checkExist(char *domain, char *key);
int32_t setPref_new(char *domain, char *key, uint8_t *value, size_t byteCount);
int32_t getPref_bool_new(char *domain, char *key, uint32_t *val);
int32_t getPref_u32_new(char *domain, char *key, uint32_t *val);
int32_t getPref_u64_new(char *domain, char *key, uint64_t *val);
int32_t getPref_str_new(char *domain, char *key, char * buf, size_t bufSize, size_t *outLen);
int32_t getPref_bin_new(char *domain, char *key, uint8_t * buf, size_t bufSize, size_t *outLen);

#ifdef __cplusplus
}
#endif