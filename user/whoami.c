#include "kernel/types.h"
#include "user/user.h"
#include "user/pwd.h"

int main(int argc, char *argv[])
{
  // TODO: implement `whoami`
  // shell forks a child process and  gid & uid will be inherited by whoami program

  // initialize users
  // getpwent();
  
  struct passwd *user = getpwuid(getuid());
  // printf("argc : %d", argc);
  // printf("number of users:%d\n", noOfUsers());
  printf("%s\n", user->name);

  //   printf("%s\n", user->name);
  // exit(0);
  // return 0;
}
