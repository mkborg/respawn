/*

  Run time Debugging support

  BUG(msg)
  BUG2_ON(condition, msg)
  BUG_ON(condition)

*/

#if !defined DEBUG_h
#define DEBUG_h

#include "LOG.h"        // logging: ELOG(), WLOG(), DLOG(), TLOG()
#include <stdlib.h>     // abort()

#define ABORT(fmt, args...) \
    do { \
	ELOG("BUG: " fmt, ##args); \
	abort(); \
    } while(0)

#define ABORT_ON_VERBOSE(condition, fmt, args...) \
    do { \
	if (condition) { \
	    ABORT(fmt, ##args); \
	} \
    } while(0)

#define ABORT_ON(condition) ABORT_ON_VERBOSE((condition), "'%s'", #condition)

#define BUG(fmt, args...) ABORT(fmt, ##args)
#define BUG_ON_VERBOSE(condition, fmt, args...) ABORT_ON_VERBOSE((condition), fmt, ##args)
#define BUG_ON(condition) ABORT_ON(condition)

#endif // !defined DEBUG_h
