/*

  Run time execution logging/tracing support.

  LOG(level, fmt, args...)
  ELOG(fmt, args...)
  WLOG(fmt, args...)
  ILOG(fmt, args...)
  TLOG(fmt, args...)

*/

#if !defined LOG_h
#define LOG_h

//#include "config.h"
#include <stdio.h>	// fprintf(), stderr

typedef enum log_level_e {
  LOG_NONE,
  LOG_FATAL,
  LOG_EMERG,	// system is unusable
  LOG_ALERT,	// action must be taken immediately
  LOG_CRIT,	// critical conditions
  LOG_ERROR,	// error conditions
  LOG_WARNING,	// warning conditions
  LOG_NOTICE,	// normal, but significant, condition
  LOG_INFO,
  LOG_NORMAL,
  LOG_TRACE,
  LOG_DEBUG,	// debug-level message
  LOG_DETAILS,
  LOG_MAXIMUM
} log_level_t;

extern const char *log_level_to_string(log_level_t log_level);
extern log_level_t log_level_from_string(const char *s);

// "LOG_LEVEL" stands for maximal compiled-in supported level of logging
#if !defined LOG_LEVEL
#define LOG_LEVEL	LOG_MAXIMUM
//#warning "LOG_LEVEL set to LOG_MAXIMUM"
#endif // !defined LOG_LEVEL

// "LOG_LEVEL_DEFAULT" stands for run time "default" level of logging
#if !defined LOG_LEVEL_DEFAULT
//#define LOG_LEVEL_DEFAULT LOG_NORMAL
#define LOG_LEVEL_DEFAULT LOG_MAXIMUM
//#warning "LOG_LEVEL_DEFAULT set to LOG_MAXIMUM"
#endif // !defined LOG_LEVEL_DEFAULT

#if !defined FUNCTION_NAME
#if defined __cplusplus
#define FUNCTION_NAME __PRETTY_FUNCTION__
#else // defined __cplusplus
#define FUNCTION_NAME __FUNCTION__
#endif // defined __cplusplus
#endif // !defined(FUNCTION_NAME)

void _log_line(log_level_t log_level, const char *function, unsigned line, const char *file, const char *format, ...);

#define _LOG_LINE(log_level, function, line, file, fmt, args...) \
    _log_line(log_level, function, line, file, fmt, ##args)

#define LOG(level, fmt, args...) \
    do { \
      if (LOG_LEVEL >= level) { \
	_LOG_LINE(level, FUNCTION_NAME, __LINE__, __FILE__, fmt, ##args); \
      } \
    } while(0)

#define ELOG(fmt, args...) LOG(LOG_ERROR  , fmt, ##args)
#define WLOG(fmt, args...) LOG(LOG_WARNING, fmt, ##args)
#define ILOG(fmt, args...) LOG(LOG_INFO   , fmt, ##args)
#define NLOG(fmt, args...) LOG(LOG_NORMAL , fmt, ##args)
#define TLOG(fmt, args...) LOG(LOG_TRACE  , fmt, ##args)

void log_hex_dump(const char *function, unsigned line, const char *file, void *p, size_t size);
#define LOG_hex_dump(p, size) \
    do { \
      log_hex_dump(FUNCTION_NAME, __LINE__, __FILE__, p, size); \
    } while(0)


#endif // !defined LOG_h
