#if !defined x_time_h
#define x_time_h

#include <time.h>	// time_t, struct tm
#include <sys/time.h>	// struct timeval
#include <sys/types.h>  // size_t

typedef struct timeval timeval_t;
typedef struct tm tm_t;

void get_time(timeval_t *p_time);
void normalize_time(timeval_t *p_tv); // makes sure that "tv_usec" is < 1000000
void unpack_time(tm_t *p_tm, const time_t *p_time);
void time2string(char *s, size_t size, const timeval_t *p_tv);
void time_sub(timeval_t *p_t, const timeval_t *p_t1, const timeval_t *p_t2);

// .........1.........2.........
// 12345678901234567890123456789
// 2008.08.19 14:48:13.704032
// 1996.09.07 12:20:09.909456435
#define TIME_STRING_LEN 26

#endif // !defined x_time_h
