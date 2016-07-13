// Copyright (c) 2009-2010, 2015 Mikhail Krivtsov

// gcc -O2 -Wall -o respawn respawn.c ASSERT.c LOG.c options.c x_time.c -lrt
// strip --strip-unneeded -R .comment respawn

/*
    Note: Our task is to indefinitely respawn child. Normally respawning is to
    be stopped only by SIGTERM sent to us (it is necessary for 'system
    shutdown').

    Probably sometimes it can be usefull to hide 'respawner' from 'external
    processes' pretending it is 'our child' by accepting and transferring to
    child some signals (SIGHUP, SIGINT, SIGQUIT, SIGUSR1, SIGUSR2) sent to us.
*/

#include "x_time.h"

#include <errno.h>	// EINVAL
#include <stdbool.h>
#include <stdio.h>	// fprintf(), printf(), stderr
#include <stdlib.h>	// EXIT_FAILURE, EXIT_SUCCESS, strtoul()
#include <string.h>	// strcmp(), strerror()
#include <sys/wait.h>	// waitpid()
#include <time.h>	// CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, struct timespec, clock_gettime()
#include <unistd.h>	// getpid(), execv(), execve(), execvp(), fork()

//extern char **environ;

//#define HANDLE_STOP_CONTINUE

#if defined HANDLE_STOP_CONTINUE
#define WAITPID_FLAGS WUNTRACED | WCONTINUED
#else
#define WAITPID_FLAGS 0
#endif // defined HANDLE_STOP_CONTINUE

#if !defined RESPAWN_SECONDS
#define RESPAWN_SECONDS 5
#endif

#define CONSTSTRLEN(s) (sizeof(s)-1)
#define STRNCMP1(s1, s2) strncmp((s1), (s2), CONSTSTRLEN(s1))
#define STRNCMP2(s1, s2) strncmp((s1), (s2), CONSTSTRLEN(s2))

typedef struct timespec timespec_t;

const char * argv0 = "?";
unsigned respawn_seconds = RESPAWN_SECONDS;
bool quiet = false;
bool verbose = false;

#define ERROR_PRINT(fmt, args...) \
	fprintf(stderr, "%s: " fmt "\n", argv0, ##args)

#define INFO_PRINT(fmt, args...) \
	fprintf(stdout, "%s: " fmt "\n", argv0, ##args)

#define IF_NOT_QUIET_PRINT(fmt, args...) \
    do { \
	if (!quiet) { INFO_PRINT(fmt, ##args); } \
    } while(0)

#define IF_VERBOSE_PRINT(fmt, args...) \
    do { \
	if (verbose) { INFO_PRINT(fmt, ##args); } \
    } while(0)

void timespec_delta(timespec_t *p_dt, const timespec_t *p_t2, const timespec_t *p_t1)
{
  p_dt->tv_sec = p_t2->tv_sec - p_t1->tv_sec;
  if (p_t2->tv_nsec < p_t1->tv_nsec) {
    // underflow
    p_dt->tv_sec -= 1;
    p_dt->tv_nsec = 1000000000 + p_t2->tv_nsec - p_t1->tv_nsec;
  } else {
    p_dt->tv_nsec = p_t2->tv_nsec - p_t1->tv_nsec;
  }
}

void handle_options(int * p_argc, char *** p_argv)
{
    for(;;) {
	const char * p;

	if (0 == *p_argc) { break; }

	p = (*p_argv)[0];

	if ('-' != *p) { break; }
	++p;

	if ('-' != *p) { break; }
	++p;

	if (0 == strcmp(p, "quiet")) {
	    verbose = false;
	    quiet = true;
	    IF_NOT_QUIET_PRINT("'%s' mode", p);
	    --*p_argc; ++*p_argv;
	    continue;
	}

	if (0 == strcmp(p, "verbose")) {
	    quiet = false;
	    verbose = true;
	    IF_NOT_QUIET_PRINT("'%s' mode", p);
	    --*p_argc; ++*p_argv;
	    continue;
	}

	if (0 == STRNCMP2(p, "respawn_seconds=")) {
	    respawn_seconds = strtoul(p+CONSTSTRLEN("respawn_seconds="), NULL, 0);
	    IF_NOT_QUIET_PRINT("respawn_seconds=%u", respawn_seconds);
	    --*p_argc; ++*p_argv;
	    continue;
	}

	break;
    }
}

pid_t sighandler_child_pid = 0;
bool sigterm_received = false;

void sighandler(int signum)
{
    if (SIGTERM == signum) { sigterm_received = true; }

    if (sighandler_child_pid > 0) {
	IF_VERBOSE_PRINT("delivering received signal %i to 'child'", signum);
	kill(sighandler_child_pid, signum);
    } else {
	IF_VERBOSE_PRINT("there is no 'child' to deliver received signal %i", signum);
    }
}

int main(int argc, char ** argv)
{
    argv0 = argv[0];
    --argc; ++argv;

    setlinebuf(stdout);
    setlinebuf(stderr);

    handle_options(&argc, &argv);

    if (argc<1) {
	// PACKAGE_STRING="${PACKAGE_NAME} ${PACKAGE_VERSION}"
	fprintf(stderr, "%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);
	fprintf(stderr, "Usage: %s [options] filename [args...]\n", argv0);
	fprintf(stderr, "  supported options:\n");
	fprintf(stderr, "    --quiet\n");
	fprintf(stderr, "    --respawn_seconds=MIN_RESPAWN_SECONDS (default is %u)\n", RESPAWN_SECONDS);
	fprintf(stderr, "    --verbose\n");
	return EXIT_FAILURE;
    }

    signal(SIGHUP , sighandler); //  1 FIXME: shall we handle 'SIGHUP'?
    signal(SIGINT , sighandler); //  2 Ctrl-C
    signal(SIGQUIT, sighandler); //  3 FIXME: shall we handle 'SIGQUIT'?
    signal(SIGUSR1, sighandler); // 10
    signal(SIGUSR2, sighandler); // 12
    signal(SIGTERM, sighandler); // 15 Used on system shutdown

    while(!sigterm_received) {

	struct timespec t1;

	if (clock_gettime(CLOCK_MONOTONIC, &t1) < 0) {
	    int l_errno = errno;
	    ERROR_PRINT("clock_gettime() errno=%i '%s'", l_errno, strerror(l_errno));
	    return EXIT_FAILURE;
	}
	if (verbose) {
	    timeval_t tv;
	    char time_string[TIME_STRING_LEN+1];
	    get_time(&tv);
	    time2string(time_string, sizeof(time_string), &tv);
	    INFO_PRINT("timestamp='%s'", time_string);

	    INFO_PRINT("t1=%lu.%09lu", (unsigned long) t1.tv_sec, t1.tv_nsec);
	}

	IF_NOT_QUIET_PRINT("invoking fork()");

	pid_t child_pid = fork();
	if (child_pid < 0) { // error

	    int l_errno = errno;
	    ERROR_PRINT("fork() errno=%i '%s'", l_errno, strerror(l_errno));
	    return EXIT_FAILURE;

	} else if (child_pid == 0) { // child

	    IF_NOT_QUIET_PRINT("child[%ld]: invoking exec*(%s)", (long)getpid(), argv[0]);
	    // Note: 'execvp()' searches PATH using 'environment' from 'extern
	    // char **environ'. Returns in case of error only.
	    execvp(argv[0], argv);

	    int l_errno = errno;
	    ERROR_PRINT("child: exec*() errno=%i '%s'", l_errno, strerror(l_errno));
	    return EXIT_FAILURE;

	} else { // parent

	    sighandler_child_pid = child_pid; // There is 'child' for 'sighandler'

	    IF_NOT_QUIET_PRINT("parent[%ld]: invoking waitpid(%ld)", (long)getpid(), (long)child_pid);
	    int status;
	    pid_t waitpid_result = waitpid(child_pid, &status, 0);
	    if (waitpid_result < 0) {
		int l_errno = errno;
		ERROR_PRINT("parent: waitpid() errno=%i '%s'", l_errno, strerror(l_errno));
		return EXIT_FAILURE;
	    }

	    sighandler_child_pid = 0; // There is no 'child' for 'sighandler'

	    if (verbose) { 
		timeval_t tv;
		struct timespec t2;
		struct timespec dt;
		unsigned days;
		unsigned hours;
		unsigned minutes;
		unsigned seconds;
		char time_string[TIME_STRING_LEN+1];
		if (clock_gettime(CLOCK_MONOTONIC, &t2) < 0) {
		    int l_errno = errno;
		    ERROR_PRINT("clock_gettime() errno=%i '%s'", l_errno, strerror(l_errno));
		    return EXIT_FAILURE;
		}

		get_time(&tv);
		time2string(time_string, sizeof(time_string), &tv);
		INFO_PRINT("timestamp='%s'", time_string);

		INFO_PRINT("t2=%lu.%09lu", (unsigned long) t2.tv_sec, t2.tv_nsec);
		timespec_delta(&dt, &t2, &t1);
		seconds = dt.tv_sec;
		days = seconds / (24 * 60 * 60);
		seconds -= days * (24 * 60 * 60);
		hours = seconds / (60 * 60);
		seconds -= hours * (60 * 60);
		minutes = seconds / 60;
		seconds -= minutes * 60;
		INFO_PRINT("dt=%lu.%09lu (days=%u hours=%u minutes=%u seconds=%u)", (unsigned long) dt.tv_sec, dt.tv_nsec, days, hours, minutes, seconds);
	    }

	    if (WIFEXITED(status)) {
		IF_NOT_QUIET_PRINT("parent: child process terminated normally with status=0x%02X/%i", WEXITSTATUS(status), WEXITSTATUS(status));
		if (sigterm_received) {
		    IF_VERBOSE_PRINT("parent: we shall terminate as well");
		    return WEXITSTATUS(status);
		}
	    } else if (WIFSIGNALED(status)) {
		IF_NOT_QUIET_PRINT("parent: child process was terminated by a signal=0x%02X/%i", WTERMSIG(status), WTERMSIG(status));
		if (sigterm_received) {
		    IF_VERBOSE_PRINT("parent: we shall terminate as well");
		    return EXIT_SUCCESS;
		}
	    } else {
		ERROR_PRINT("parent: child process terminated in unexpected way");
		return EXIT_FAILURE;
	    }

	    t1.tv_sec += respawn_seconds;
	    IF_NOT_QUIET_PRINT("parent: invoking clock_nanosleep() to slowdown respawn");
	    int clock_nanosleep_result = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t1, NULL);
	    // Normally 'clock_nanosleep()' can be interrupted by signal only
	    if (clock_nanosleep_result) {
		if (EINTR != clock_nanosleep_result) {
		    ERROR_PRINT("parent: clock_nanosleep() unexpectedly returned %i '%s'", clock_nanosleep_result, strerror(clock_nanosleep_result));
		}
		// Interrupted 'clock_nanosleep()' means that respawning shall
		// be finished (even if signal is not SIGTERM)
		IF_VERBOSE_PRINT("parent: we shall terminate since 'clock_nanosleep()' was interrupted by signal");
		return EXIT_SUCCESS;
	    }
	} // parent
    } // main loop

    return EXIT_SUCCESS;
}
