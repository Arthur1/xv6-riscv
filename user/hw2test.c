#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(void)
{
  int fd = open("hw2test.txt", O_WRONLY | O_CREATE | O_TRUNC);
  write(fd, "test\n", 6);
  close(fd);
  int fd2 = open("hw2test.txt", O_WRONLY | O_APPEND);
  write(fd2, "test2\n", 7);
  close(fd2);
  int fd3 = open("hw2test.txt", O_WRONLY | O_APPEND);
  write(fd3, "test3\n", 7);
  close(fd3);
  exit(0);
}
