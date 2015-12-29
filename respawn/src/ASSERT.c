#include "DEBUG.h"      // bug hunter: ABORT(), ABORT_ON(), ABORT_VERBOSE_ON()
#include "LOG.h"        // logging: ELOG(), WLOG(), DLOG(), TLOG()
#include "COMPILER.h"	// UNUSED

#include <stdlib.h>	// abort()

//#include <signal.h>
//#include <sys/types.h>
//#include <unistd.h>

void assert_fail(const char *expr, const char *file UNUSED, int line, const char *function)
{
    ELOG("ASSERT(%s) failure in '%s':%d", expr, function, line);
    //kill(0, SIGABRT);
    abort();
}
