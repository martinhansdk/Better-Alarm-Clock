#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC int testAlarm (int seconds);

#undef EXTERNC
