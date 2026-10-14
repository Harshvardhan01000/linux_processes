#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERR_MSG(msg, error_n)                                                  \
  do {                                                                         \
    printf("error occur in %s", msg);                                          \
  } while (0)

int sum_func(int *arr, int start, int end) {
  printf("log:: start:%d end:%d\n", start, end);
  int sum = 0;
  for (int i = start; i < end; i++)
    sum += arr[i];
  return sum;
}

int read_sum_from_child(int fd) {
  int child_sum;
  if (read(fd, &child_sum, sizeof(int)) == -1)
    ERR_MSG("read", 2);
  close(fd);
  return child_sum;
}

void write_to_parent(int fd, int *buff) {
  if (write(fd, buff, sizeof(int)) == -1)
    ERR_MSG("write", 3);
  close(fd);
}

int main() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7};
  int length = 7;
  int fd[2];
  if (pipe(fd) == -1)
    ERR_MSG("pipe", 1);

  pid_t pid = fork();

  if (pid == -1)
    ERR_MSG("fork", 4);

  if (pid) {
    // NOTE: parent
    int sum = sum_func(arr, 0, length / 3);
    printf("parent sum = %d\n", sum);

    close(fd[1]);
    int child_sum = read_sum_from_child(fd[0]);
    sum += child_sum;
    printf("total sum is %d\n", sum);
  } else {
    // NOTE: child

    pid_t pid_child = fork();

    if (pid_child == -1)
      ERR_MSG("fork", 5);

    if (pid_child) {
      int sum = sum_func(arr, length / 3, (length / 3) * 2);

      printf("child sum = %d\n", sum);

      int child_sum = read_sum_from_child(fd[0]);
      sum += child_sum;

      write_to_parent(fd[1], &sum);
      printf("child total sum = %d\n", sum);
    } else {
      close(fd[0]);
      int sum = sum_func(arr, (length / 3) * 2, length);
      printf("child sum = %d\n", sum);
      write_to_parent(fd[1], &sum);
    }
  }
  if (wait(NULL) == -1 && errno == ECHILD)
    ;
  return 0;
}
