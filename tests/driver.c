#include "driver.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "test_lifecycle/test_lifecycle.h"

// in order to be able to run the program with setting scheduling requests
// you need to interact with the system by setting limits
static int setup_environment(void) {
  struct rlimit r;

  r.rlim_max = 50;
  r.rlim_cur = 20;

  return setrlimit(RLIMIT_RTPRIO, &r);
}

int main(void) {
  int no_failed = 0;
  Suite *s = NULL;
  SRunner *runner = NULL;

  if (setup_environment() == -1) {
    switch (errno) {
      case EFAULT:
        strerror(errno);
        break;
      case EINVAL:
        strerror(errno);
        break;
      case EPERM:
        fprintf(stderr, "%s \n", strerror(errno));
        break;
      case ESRCH:
        strerror(errno);
        break;
      default:
        break;
    }
  }

  s = lifecycle_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}