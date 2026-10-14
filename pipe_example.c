#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // this is something
  int fd[2];
  if (pipe(fd) == -1) {
    printf("error occur opening pipe\n");
    return 1;
  }

  pid_t pid = fork();

  if (pid == -1) {
    printf("error with fork");
    return 1;
  }

  if (pid) {
    close(fd[0]);
    int x;
    printf("input a number: ");
    scanf("%d", &x);
    if (write(fd[1], &x, sizeof(int)) == -1) {
      printf("getting error from parent in write");
      return 1;
    }
    close(fd[1]);
  } else {
    close(fd[1]);
    int y;
    if (read(fd[0], &y, sizeof(int)) == -1) {
      printf("getting error from child in read");
      return 1;
    }
    close(fd[0]);

    printf("from child process : %d\n", y);
  }
  // wait(NULL);
  return 0;
}
