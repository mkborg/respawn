#include "x_time.h"

#include "ASSERT.h"     // ASSERT(expr) macro
#include "DEBUG.h"      // bug hunter: ABORT(), ABORT_ON(), ABORT_VERBOSE_ON()
#include "LOG.h"        // logging: ELOG(), WLOG(), DLOG(), TLOG()

#include <errno.h>	// errno
#include <stddef.h>	// NULL
#include <stdio.h>	// snprintf
#include <string.h>	// strerror()
#include <time.h>	// localtime_r()
#include <sys/time.h>	// gettimeofday()
#include <sys/types.h>	// size_t

// Note: Our "timeval_t" is silly typedef for "struct timeval"
void get_time(timeval_t *p_tv)
{
  int error;

  error = gettimeofday(p_tv, NULL);
  if (error) {
    int l_errno = errno;
    ELOG("gettimeofday() failure %d '%s'", l_errno, strerror(l_errno));
  }
  ASSERT(!error);
}

// makes sure that "tv_usec" is < 1000000
void normalize_time(timeval_t *p_tv)
{
    while (1000000 <= p_tv->tv_usec) {
	p_tv->tv_sec += 1;
	p_tv->tv_usec -= 1000000;
    }
}

void unpack_time(tm_t *p_tm, const time_t *p_time)
{
  tm_t *result;
  errno = 0; // XXX
  result = localtime_r(p_time, p_tm);
  if (NULL == result) {
    int l_errno = errno; // XXX
    ELOG("localtime_r() failure %d '%s'", l_errno, strerror(l_errno)); // XXX
  }
  ASSERT(NULL != result);

  /* The "time_t" contains seconds since 1970.01.01 00:00:00", but tm_t.tm_year contains years since 1900. */
  p_tm->tm_year += 1900;

  /* tm_t.tm_mon The number of months since January, in the range 0 to 11 */
  p_tm->tm_mon += 1;
}

void time2string(char *s, size_t size, const timeval_t *p_tv)
{
  struct tm tm;
  timeval_t tv = *p_tv;
  int result;

#if 0
  // mk: Note: The following line modifies const parameter. It is very bad and must be fixed.
  normalize_time(p_tv); // makes sure that "tv_usec" is < 1000000
#endif
  normalize_time(&tv); // makes sure that "tv_usec" is < 1000000

  unpack_time(&tm, &(tv.tv_sec));

  ASSERT(1000000 > tv.tv_usec);
  result = snprintf(s, size, "%04u.%02u.%02u %02u:%02u:%02u.%03u%03u",
	tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
	(unsigned)(tv.tv_usec/1000), (unsigned)(tv.tv_usec%1000));
  ABORT_ON_VERBOSE((size_t)result >= size, "snprintf() buffer overflow (required=%d+1/available=%u)", result, (unsigned)size);
}

void time_sub(timeval_t *p_t, const timeval_t *p_t1, const timeval_t *p_t2)
{
  p_t->tv_sec = p_t1->tv_sec - p_t2->tv_sec;
  if (p_t1->tv_usec < p_t2->tv_usec) {
    // underflow
    p_t->tv_sec -= 1;
    p_t->tv_usec = 1000000 + p_t1->tv_usec - p_t2->tv_usec;
  } else {
    p_t->tv_usec = p_t1->tv_usec - p_t2->tv_usec;
  }
}
