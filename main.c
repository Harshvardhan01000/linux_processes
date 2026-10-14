#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  if (pid)
    wait(NULL);
  // NOTE: parent have to wait for child to finish else ppid of child will be
  // systemd's id
  if (pid)
    printf("process id: %d && parent id: %d\n", getpid(), getppid());
  else
    printf("child -> process id: %d && parent id: %d\n", getpid(), getppid());
  return 0;
}
