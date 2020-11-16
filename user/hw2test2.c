#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(void)
{
  int fd = open("hw2test2.txt", O_RDWR | O_CREATE | O_TRUNC);
  int off = lseek(fd, 0, SEEK_CUR);
  char buff[60];

  printf("off=%d\n", off);
  write(fd, "Hello.\n", 8);
  off = lseek(fd, 0, SEEK_CUR);
  printf("off=%d (expected=8)\n", off);

  off = lseek(fd, 5, SEEK_SET);
  printf("off=%d (expected=5)\n", off);

  write(fd, ", world.\n", 10);
  off = lseek(fd, 0, SEEK_CUR);
  printf("off=%d (expected=15)\n", off);

  off = lseek(fd, -10, SEEK_SET);
  printf("off=%d (expected=-1)\n", off);

  off = lseek(fd, 0, SEEK_CUR);
  printf("off=%d (expected=15)\n", off);

  off = lseek(fd, 0, SEEK_SET);
  printf("off=%d (expected=0)\n", off);
  read(fd, buff, 60);
  printf("read=%s\n", buff);

  off = lseek(fd, -8, SEEK_END);
  printf("off=%d (expected=7)\n", off);
  read(fd, buff, 60);
  printf("read=%s\n", buff);

  off = lseek(fd, 40, SEEK_END);
  printf("off=%d (expected=55)\n", off);

  off = lseek(fd, 0, SEEK_SET);
  printf("off=%d (expected=0)\n", off);
  read(fd, buff, 60);
  printf("read=%s\n", buff);

  exit(0);
}
