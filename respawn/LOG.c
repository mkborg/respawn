#include "DEBUG.h"      // bug hunter: ABORT(), ABORT_ON(), ABORT_VERBOSE_ON()
#include "LOG.h"        // logging: ELOG(), WLOG(), DLOG(), TLOG()

#include "options.h"    // src_prefix, active_log_level

#include <stdarg.h>	// va_list, va_start(), va_end()
#include <stdio.h>	// vfprintf()
#include <string.h>	// memset(), strcmp


//#define FILL_WITH_CONST_CHAR

#define HEX_DUMP_WIDTH 16
//#define HEX_DUMP_WIDTH 32

#define FILL_BUFFER(buffer, filler) \
    do { \
	memset((buffer), (filler), sizeof(buffer) - 1); \
	(buffer)[sizeof(buffer) - 1] = 0; \
    } while (0)


void log_hex_dump(const char *function, unsigned line, const char *file, void *p, size_t size)
{
#if 0
  // "XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX"
  // "XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX"
  char hex_buf[HEX_DUMP_WIDTH*2 + HEX_DUMP_WIDTH-1 + (HEX_DUMP_WIDTH-1)/4 + 1];
#else
  // "XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX"
  // "XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX"
  char hex_buf[HEX_DUMP_WIDTH*2 + (HEX_DUMP_WIDTH-1)/4 + 1];
#endif
  // ".... .... .... ...."
  // ".... .... .... .... .... .... .... ...."
  char sym_buf[HEX_DUMP_WIDTH + (HEX_DUMP_WIDTH-1)/4 + 1];

  while(size) {

    unsigned hex_off = 0;
    unsigned sym_off = 0;
    unsigned off;

#if defined FILL_WITH_CONST_CHAR
    FILL_BUFFER(hex_buf, '#');
    FILL_BUFFER(sym_buf, '#');
#endif // defined FILL_WITH_CONST_CHAR

    for(off=0; off<HEX_DUMP_WIDTH && off<size; ++off) {
    //for(off=0; off<HEX_DUMP_WIDTH; ++off) {
      //char *d1 = (0!=off) ? " " : "";
      const char *d4 = (0!=off/4 && 0==off%4) ? " " : "";
      unsigned char b = ((unsigned char *)p)[off];
      //unsigned char b = (off<size) ? ((unsigned char *)p)[off] : 0;

      // "XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX  XX XX XX XX"
#if 0
      hex_off+=snprintf(hex_buf+hex_off, sizeof(hex_buf)-hex_off, "%s%s%02X", d1, d4, b);
#else
      hex_off+=snprintf(hex_buf+hex_off, sizeof(hex_buf)-hex_off, "%s%02X", d4, b);
#endif
      if (hex_off >= sizeof(hex_buf)-1) { hex_off = sizeof(hex_buf)-1; }

      if (b >= 0x80) { b &= ~0x80; }
      if (b < ' ' || b == 0x7F) { b = '.'; }

      // ".... .... .... .... .... .... .... ...."
      sym_off+=snprintf(sym_buf+sym_off, sizeof(sym_buf)-sym_off, "%s%c", d4, b);
      if (sym_off >= sizeof(sym_buf)-1) { sym_off = sizeof(sym_buf)-1; }
    }

    for(; off<HEX_DUMP_WIDTH; ++off) {
      const char *d4 = (0!=off/4 && 0==off%4) ? " " : "";

      hex_off += snprintf(hex_buf+hex_off, sizeof(hex_buf)-hex_off, "%s..", d4);
      if (hex_off >= sizeof(hex_buf)-1) { hex_off = sizeof(hex_buf)-1; }

      sym_off += snprintf(sym_buf+sym_off, sizeof(sym_buf)-sym_off, "%s.", d4);
      if (sym_off >= sizeof(sym_buf)-1) { sym_off = sizeof(sym_buf)-1; }
    }

    _LOG_LINE(LOG_TRACE, function, line, file, "%08lX: %s %s", (unsigned long)p, hex_buf, sym_buf);

    if (off > size) { off = size; }
    size -= off;
    p = ((char *)p) + off;
  }
}


void _log_line(log_level_t log_level, const char *function, unsigned line, const char *file, const char *format, ...)
{
  va_list argsList;
  FILE *stream = /*(log_level != LOG_NORMAL) ?*/ stderr /*: stdout*/;

  if (log_level > active_log_level) { return; }

  if (NULL != src_prefix) {
    size_t prefix_len = strlen(src_prefix);
    if (0 != prefix_len && 0 == strncmp(file, src_prefix, prefix_len)) {
      file += prefix_len; // skip prefix
      if ('/' == *file) { ++file; } // prefix often has no trailing slash
    }
  }

  fprintf(stream, "[%s:%d:%s] ", function, line, file);

  switch(log_level) {
    case LOG_ERROR:
      fprintf(stream, "ERROR: ");
      break;
    case LOG_WARNING:
      fprintf(stream, "WARNING: ");
      break;
    case LOG_INFO:
      fprintf(stream, "INFO: ");
      break;
    default:
      break;
  }

  va_start(argsList, format);
  vfprintf(stream, format, argsList);
  va_end(argsList);

  fprintf(stream, "\n");
}

typedef struct log_level_map_s {
  log_level_t log_level;
  const char *text;
} log_level_map_t;

#define LOG_LEVEL_MAP_ITEM(i) { LOG_##i, #i }
log_level_map_t log_level_map[] = {
  LOG_LEVEL_MAP_ITEM(NONE),
  LOG_LEVEL_MAP_ITEM(FATAL),
  LOG_LEVEL_MAP_ITEM(EMERG),    // system is unusable
  LOG_LEVEL_MAP_ITEM(ALERT),    // action must be taken immediately
  LOG_LEVEL_MAP_ITEM(CRIT),     // critical conditions
  LOG_LEVEL_MAP_ITEM(ERROR),    // error conditions
  LOG_LEVEL_MAP_ITEM(WARNING),  // warning conditions
  LOG_LEVEL_MAP_ITEM(NOTICE),   // normal, but significant, condition
  LOG_LEVEL_MAP_ITEM(INFO),
  LOG_LEVEL_MAP_ITEM(NORMAL),
  LOG_LEVEL_MAP_ITEM(TRACE),
  LOG_LEVEL_MAP_ITEM(DEBUG),    // debug-level message
  LOG_LEVEL_MAP_ITEM(DETAILS),
  LOG_LEVEL_MAP_ITEM(MAXIMUM)
};

const char *log_level_to_string(log_level_t log_level)
{
  unsigned i;
  for(i=0; i<sizeof(log_level_map)/sizeof(log_level_map[0]); ++i) {
    if (log_level == log_level_map[i].log_level) {
      return log_level_map[i].text;
    }
  }
  return "<UNKNOWN>";
}

log_level_t log_level_from_string(const char *s)
{
  unsigned i;
  for(i=0; i<sizeof(log_level_map)/sizeof(log_level_map[0]); ++i) {
    if (0 == strcmp(s, log_level_map[i].text)) {
      return log_level_map[i].log_level;
    }
  }
  return LOG_MAXIMUM; // FIXME: may be "abort()" call is better?
}
