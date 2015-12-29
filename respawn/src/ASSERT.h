/*

  Our own variant of "assert()" macro.

*/

#if !defined ASSERT_h
#define ASSERT_h


#include "COMPILER.h"	// NORETURN

#ifdef __cplusplus
extern "C" {
#endif

void assert_fail(const char *expr, const char *file, int line, const char *function) NORETURN;

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#define ASSERT_FUNCTION __PRETTY_FUNCTION__
#else
#define ASSERT_FUNCTION __FUNCTION__
#endif


#define _STRING(x) #x

#define ASSERT(expr) \
    ((expr) \
	? (void) (0) \
	: (assert_fail (_STRING(expr), __FILE__, __LINE__, ASSERT_FUNCTION), (void) (0)))


#endif // !defined ASSERT_h
