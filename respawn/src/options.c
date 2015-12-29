#include "options.h"

log_level_t active_log_level = LOG_LEVEL_DEFAULT;

const char *src_prefix =
#if defined SRC_PREFIX
  SRC_PREFIX
#else
  NULL
#endif
;
