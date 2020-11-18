#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void opentest(int fd)
{
  static int counter = 0;
  if (fd < 0) {
    fprintf(2, "opentest%d: failure\n", counter);
    exit(1);
  }
  counter++;
}

void strtest(char *buf, const char *expectedbuf)
{
  static int counter = 0;
  if (strcmp(buf, expectedbuf) != 0) {
    fprintf(2, "strtest%d: failure (result=%s, expected=%s)\n", counter, buf, expectedbuf);
    exit(1);
  }
  counter++;
}

int main(void)
{
  int fd = open("fsymlinktest.txt", O_WRONLY | O_CREATE | O_TRUNC);
  opentest(fd);
  write(fd, "Hello\n", 6);
  close(fd);

  symlink("fsymlinktest.txt", "fsymlinktest1.txt");
  fd = open("fsymlinktest1.txt", O_RDONLY);
  opentest(fd);
  char buf[60];
  read(fd, buf, 60);
  strtest(buf, "Hello\n");
  memset(buf, 0, 60);
  close(fd);

  symlink("fsymlinktest1.txt", "fsymlinktest2.txt");
  fd = open("fsymlinktest2.txt", O_RDONLY);
  opentest(fd);
  read(fd, buf, 60);
  strtest(buf, "Hello\n");
  memset(buf, 0, 60);
  close(fd);

  fd = open("fsymlinktest1.txt", O_WRONLY);
  opentest(fd);
  lseek(fd, 5, SEEK_SET);
  write(fd, ", world.\n", 9);
  close(fd);

  fd = open("fsymlinktest.txt", O_RDONLY);
  opentest(fd);
  read(fd, buf, 60);
  strtest(buf, "Hello, world.\n");
  memset(buf, 0, 60);
  close(fd);

  fd = open("fsymlinktest2.txt", O_RDONLY);
  opentest(fd);
  read(fd, buf, 60);
  strtest(buf, "Hello, world.\n");
  memset(buf, 0, 60);
  close(fd);

  printf("Success!\n");
  exit(0);
}