#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(void)
{
  int fd;
  char buf[100];
  fd = open("fappendtest.txt", O_WRONLY | O_CREATE | O_TRUNC);
  write(fd, "test,", 5);
  close(fd);

  fd = open("fappendtest.txt", O_WRONLY | O_APPEND);
  write(fd, "test2,", 6);
  close(fd);

  fd = open("fappendtest.txt", O_WRONLY | O_APPEND);
  write(fd, "test3\n", 6);
  close(fd);

  fd = open("fappendtest.txt", O_RDONLY);
  read(fd, buf, 100);
  if (strcmp(buf, "test,test2,test3\n") == 0) {
    printf("Success!\n");
    exit(0);
  } else {
    fprintf(2, "Failure...\n");
    exit(1);
  }
}
