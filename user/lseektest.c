#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
offtest(int off, int expectedoff)
{
  static int counter = 0;
  if (off != expectedoff) {
    fprintf(2, "offtest%d: failure (result=%d, expected=%d)\n", counter, off, expectedoff);
    exit(1);
  }
  counter++;
}

void
strtest(char *buf, const char *expectedbuf)
{
  static int counter = 0;
  if (strcmp(buf, expectedbuf) != 0) {
    fprintf(2, "strtest%d: failure (result=%s, expected=%s)\n", counter, buf, expectedbuf);
    exit(1);
  }
  counter++;
}

int
main(void)
{
  int fd = open("flseektest.txt", O_RDWR | O_CREATE | O_TRUNC);
  int off = lseek(fd, 0, SEEK_CUR);
  char buf[60];

  offtest(off, 0);
  write(fd, "Hello.\n", 7);
  off = lseek(fd, 0, SEEK_CUR);
  offtest(off, 7);

  off = lseek(fd, 5, SEEK_SET);
  offtest(off, 5);

  write(fd, ", world.\n", 9);
  off = lseek(fd, 0, SEEK_CUR);
  offtest(off, 14);

  off = lseek(fd, -10, SEEK_SET);
  offtest(off, -1);

  off = lseek(fd, 0, SEEK_CUR);
  offtest(off, 14);

  off = lseek(fd, 0, SEEK_SET);
  offtest(off, 0);
  read(fd, buf, 60);
  strtest(buf, "Hello, world.\n");
  memset(buf, 0, 60);

  off = lseek(fd, -7, SEEK_END);
  offtest(off, 7);
  read(fd, buf, 60);
  strtest(buf, "world.\n");
  memset(buf, 0, 60);

  off = lseek(fd, 40, SEEK_END);
  offtest(off, 54);

  off = lseek(fd, 0, SEEK_SET);
  offtest(off, 0);
  read(fd, buf, 60);
  strtest(buf, "Hello, world.\n");
  memset(buf, 0, 60);

  printf("Success!\n");
  exit(0);
}
