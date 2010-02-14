// Copyright (c) 2009-2010 Mikhail Krivtsov

// gcc -O2 -Wall -o respawn respawn.c ASSERT.c LOG.c options.c x_time.c -lrt
// strip --strip-unneeded -R .comment respawn

#include "x_time.h"

#include <errno.h>	// EINVAL
#include <stdio.h>	// fprintf(), printf(), stderr
#include <stdlib.h>	// EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>	// strcmp(), strerror()
#include <sys/wait.h>	// waitpid()
#include <time.h>	// CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, struct timespec, clock_gettime()
#include <unistd.h>	// getpid(), execv(), execve(), execvp(), fork()

//extern char **environ;

#define HANDLE_STOP_CONTINUE

#define MIN_RUN_TIME_SECONDS (15*60)

typedef struct timespec timespec_t;

int keepalive = 0;
int quiet = 0;
int verbose = 0;

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
	    
	if (0 == strcmp(p, "keepalive")) {
	    keepalive = 1;
	    if (!quiet) { printf("'%s' mode\n", p); }
	    --*p_argc; ++*p_argv;
	    continue;
	}

	if (0 == strcmp(p, "quiet")) {
	    verbose = 0;
	    quiet = 1;
	    if (!quiet) { printf("'%s' mode\n", p); }
	    --*p_argc; ++*p_argv;
	    continue;
	}

	if (0 == strcmp(p, "verbose")) {
	    quiet = 0;
	    verbose = 1;
	    if (!quiet) { printf("'%s' mode\n", p); }
	    --*p_argc; ++*p_argv;
	    continue;
	}

	break;
    }
}

//int main(int argc, char * argv[])
int main(int argc, char ** argv)
//int main(int argc, char * argv[], char * envv[])
{
    char * argv0 = argv[0];
    --argc; ++argv;

    setlinebuf(stdout);
    setlinebuf(stderr);

    //printf("environ=%p envv=%p\n", environ, envv);
    //environ = envv;

    handle_options(&argc, &argv);

    if (argc<1) {
	fprintf(stderr, "Usage: %s [options] filename [args...]\n", argv0);
	return EXIT_FAILURE;
    }

    for(;;) {
	pid_t child_pid;

	struct timespec t1;

	if (clock_gettime(CLOCK_MONOTONIC, &t1) < 0) {
	    int l_errno = errno;
	    fprintf(stderr, "%s: clock_gettime() errno=%i '%s'\n", argv0, l_errno, strerror(l_errno));
	    return EXIT_FAILURE;
	}
	if (verbose) {
	    timeval_t tv;
	    char time_string[TIME_STRING_LEN+1];
	    get_time(&tv);
	    time2string(time_string, sizeof(time_string), &tv);
	    printf("%s: timestamp='%s'\n", argv0, time_string);

	    printf("%s: t1=%lu.%09lu\n", argv0, (unsigned long) t1.tv_sec, t1.tv_nsec);
	}

	if (!quiet) { printf("%s: invoking fork()\n", argv0); }

	child_pid = fork();
	if (child_pid < 0) { // error

	    int l_errno = errno;
	    fprintf(stderr, "%s: fork() errno=%i '%s'\n", argv0, l_errno, strerror(l_errno));
	    return EXIT_FAILURE;

	} else if (child_pid == 0) { // child

	    //if (verbose) { printf("%s: child: I am child process\n", argv0); }

	    if (!quiet) { printf("%s: child[%ld]: invoking exec*(%s)\n", argv0, (long)getpid(), argv[0]); }
	    // Uses "extern char **environ" for environment. Searches PATH.
	    execvp(argv[0], argv);

	    int l_errno = errno;
	    fprintf(stderr, "%s: child: exec*() errno=%i '%s'\n", argv0, l_errno, strerror(l_errno));
	    return EXIT_FAILURE;

	} else { // parent

	    int status;

	    //if (verbose) { printf("%s: parent: I am parent process\n", argv0); }

	    for(;;) {
		pid_t pid;
		if (!quiet) { printf("%s: parent[%ld]: invoking waitpid(%ld)\n", argv0, (long)getpid(), (long)child_pid); }
#if defined HANDLE_STOP_CONTINUE
		pid = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
#else
		pid = waitpid(child_pid, &status, 0);
#endif // defined HANDLE_STOP_CONTINUE

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
			fprintf(stderr, "%s: clock_gettime() errno=%i '%s'\n", argv0, l_errno, strerror(l_errno));
			return EXIT_FAILURE;
		    }

		    get_time(&tv);
		    time2string(time_string, sizeof(time_string), &tv);
		    printf("%s: timestamp='%s'\n", argv0, time_string);

		    printf("%s: t2=%lu.%09lu\n", argv0, (unsigned long) t2.tv_sec, t2.tv_nsec);
		    timespec_delta(&dt, &t2, &t1);
		    seconds = dt.tv_sec;
		    days = seconds / (24 * 60 * 60);
		    seconds -= days * (24 * 60 * 60);
		    hours = seconds / (60 * 60);
		    seconds -= hours * (60 * 60);
		    minutes = seconds / 60;
		    seconds -= minutes * 60;
		    printf("%s: dt=%lu.%09lu (days=%u hours=%u minutes=%u seconds=%u)\n", argv0, (unsigned long) dt.tv_sec, dt.tv_nsec, days, hours, minutes, seconds);
		}

		//if (verbose) { printf("%s: parent: pid=%i status=0x%X/%i\n", argv0, pid, status, status); }
		if (WIFEXITED(status)) {
		    if (!quiet) { printf("%s: parent: child process terminated normally with status=0x%02X/%i\n", argv0, WEXITSTATUS(status), WEXITSTATUS(status)); }
		    if (keepalive) {
			return WEXITSTATUS(status);
		    }
		    break;
		} else if (WIFSIGNALED(status)) {
		    if (!quiet) { printf("%s: parent: child process was terminated by a signal=0x%02X/%i\n", argv0, WTERMSIG(status), WTERMSIG(status)); }
		    break;
#if defined HANDLE_STOP_CONTINUE
		} else if (WIFSTOPPED(status)) {
		    if (!quiet) { printf("%s: parent: child process was stopped by a signal=0x%02X/%i\n", argv0, WSTOPSIG(status), WSTOPSIG(status)); }
		} else if (WIFCONTINUED(status)) {
		    if (!quiet) { printf("%s: parent: child process was resumed by SIGCONT signal\n", argv0); }
#endif // defined HANDLE_STOP_CONTINUE
		} else {
		    fprintf(stderr, "%s: parent: child process terminated in unexpected way\n", argv0);
		    break;
		}
	    } // waitpid loop

	    t1.tv_sec += MIN_RUN_TIME_SECONDS;
	    do {
		if (!quiet) { printf("%s: parent: invoking clock_nanosleep() to slowdown respawn\n", argv0); }
		if (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t1, NULL) < 0) {
		    int l_errno = errno;
		    fprintf(stderr, "%s: child: clock_nanosleep() errno=%i '%s'\n", argv0, l_errno, strerror(l_errno));
		    if (EINTR == l_errno) { continue; }
		}
	    } while(0);
	} // parent
    } // fork loop

    return EXIT_SUCCESS;
}
