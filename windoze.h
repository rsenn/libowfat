#if ((defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__) && !defined(__MSYS__))

#ifdef __cplusplus
extern "C" {
#endif

/* set errno to WSAGetLastError() */
int winsock2errno(long l);
void __winsock_init(void);

#ifdef __cplusplus
}
#endif

#else

#define winsock2errno(fnord) (fnord)
#define __winsock_init()

#endif
