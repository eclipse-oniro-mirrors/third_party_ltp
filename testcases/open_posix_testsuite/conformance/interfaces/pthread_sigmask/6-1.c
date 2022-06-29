/*
 * Copyright (c) 2003, Intel Corporation. All rights reserved.
 * Created by:  salwan.searty REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this
 * source tree.

 The resulting set shall be the intersection of the current set and
 the complement of the set pointed to by set.

 Steps:
 1. Have main create a new thread and wait for its termination.
 2. Inside the new thread, set up the signal mask such that it contains
    both SIGABRT and SIGALRM
 3. Also inside the new thread, using the SIG_UNBLOCK as the value to
    pthread_sigmask's first parameter, unblock SIGALRM. Now only SIGABRT
    should be in the signal mask of the new thread.
 4. Raise SIGALRM, and verify that handler was called, otherwise
    test fails.
 5. Reset handler_called variable to 0. Raise SIGABRT, and verify that
    handler wasn't called, otherwise test fails.
 6. Make sure that the only pending signal is SIGABRT, otherwise fail.
 7. Return one of three return codes to the main() function:
    - A value of -1 if any of failures mentioned above occured.
    - A value of 0 if both SIGALRM was successfully blocked.
    - A value of 1 incase of any UNRESOLVED situation such as an
      unexpected function failure.
*/

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include "posixtest.h"

static volatile int handler_called;

static void handler(int signo PTS_ATTRIBUTE_UNUSED)
{
	handler_called = 1;
}

static void *a_thread_func()
{
	struct sigaction act;
	sigset_t set1, set2, pending_set;

	sigemptyset(&set1);
	sigaddset(&set1, SIGABRT);
	sigaddset(&set1, SIGALRM);

	sigemptyset(&set2);
	sigaddset(&set2, SIGALRM);

	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	if (sigaction(SIGABRT, &act, 0) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		pthread_exit((void *)1);
	}

	if (sigaction(SIGALRM, &act, 0) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		pthread_exit((void *)1);
	}

	if (pthread_sigmask(SIG_SETMASK, &set1, NULL) == -1) {
		perror
		    ("Unexpected error while attempting to use pthread_sigmask.\n");
		pthread_exit((void *)1);
	}

	if (pthread_sigmask(SIG_UNBLOCK, &set2, NULL) == -1) {
		perror
		    ("Unexpected error while attempting to use pthread_sigmask.\n");
		pthread_exit((void *)1);
	}

	if (raise(SIGALRM) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		pthread_exit((void *)1);
	}

	if (!handler_called) {
		printf
		    ("FAIL: Handler was not called for even though signal was removed from the signal mask\n");
		pthread_exit((void *)-1);
	}

	handler_called = 0;
	if (raise(SIGABRT) == -1) {
		perror("Unexpected error while attempting to setup test "
		       "pre-conditions");
		pthread_exit((void *)1);
	}

	if (handler_called) {
		printf
		    ("FAIL: Hanlder was called for even though signal should have been in the signal mask\n");
		pthread_exit((void *)-1);
	}

	if (sigpending(&pending_set) == -1) {
		perror("Unexpected error while attempting to use sigpending\n");
		pthread_exit((void *)1);
	}

	if (sigismember(&pending_set, SIGABRT) != 1) {
		perror("FAIL: sigismember did not return 1\n");
		pthread_exit((void *)-1);
	}

	if (sigismember(&pending_set, SIGALRM) != 0) {
		perror("FAIL: sigismember did not return 0\n");
		pthread_exit((void *)-1);
	}

	pthread_exit(NULL);
	return NULL;
}

int main(void)
{

	int *thread_return_value;

	pthread_t new_thread;

	if (pthread_create(&new_thread, NULL, a_thread_func, NULL) != 0) {
		perror("Error creating new thread\n");
		return PTS_UNRESOLVED;
	}

	if (pthread_join(new_thread, (void *)&thread_return_value) != 0) {
		perror("Error in pthread_join()\n");
		return PTS_UNRESOLVED;
	}

	if ((long)thread_return_value != 0) {
		if ((long)thread_return_value == 1) {
			printf("Test UNRESOLVED\n");
			return PTS_UNRESOLVED;
		} else if ((long)thread_return_value == -1) {
			printf("Test FAILED\n");
			return PTS_FAIL;
		} else {
			printf("Test UNRESOLVED\n");
			return PTS_UNRESOLVED;
		}
	}
	return PTS_PASS;
}
