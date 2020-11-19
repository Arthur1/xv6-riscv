#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if(argc < 2) {
    fprintf(2, "Usage: readlink files...\n");
    exit(1);
  }

  char buf[128];
  for (int i = 1; i < argc; i++) {
    memset(buf, 0, 128);
    if (readlink(argv[i], buf, 128) == 0) {
      printf("%s\n", buf);
    }
  }
  exit(0);
}
