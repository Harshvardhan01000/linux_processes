#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 x_
 | |
 y z
 |
 p
 */

#define PRINT_PROCESS(name)                                                    \
  do {                                                                         \
    printf("process : %s (pid = %d && parent id = %d)\n", name, getpid(),      \
           getppid());                                                         \
  } while (0)

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  pid_t pid2 = fork();

  if (pid) {
    if (pid2) {
      PRINT_PROCESS("x");
    } else {
      PRINT_PROCESS("y");
    }
  } else {
    if (pid2)
      PRINT_PROCESS("z");
    else
      PRINT_PROCESS("p");
  }

  while (wait(NULL) != -1 && errno != ECHILD)
    ;
  return 0;
}
