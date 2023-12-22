#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAXLEN 30

// Global variables
int fd_passwd = 0;

// Reads one line from buf at position i into field
int
getpwfield(char *field, char *buf, int i)
{
  int c = 0;
  while(buf[i] != ':' && buf[i] != '\n'){
    field[c++] = buf[i++];
  }
  //field[c] = 0;
  return ++i;
}

// Returns null on error
struct passwd *
getpwent(void)
{
  if(!fd_passwd)
    setpwent(); // open passwd file if fd_passwd not set

  // general procedure would be to:
  //    1. get fields from passwd file entry

    char buff[32];
    // i will store the number of characters read
    int i = read(fd_passwd,buff,sizeof(buff));
    printf("i:%d\n",i);

    char name[32];
    getpwfield(name,buff,0);
    printf(buff);
    printf("\n");
  
  //    2. write fields into `struct passwd`
  struct passwd *p = malloc(sizeof(struct passwd));
  p->name = name;
  

  return p;
}

void 
setpwent(void)
{
  if((fd_passwd = open(PASSWD_PATH, O_CREATE)) < 0)
    // void return type means failure doesn't matter, I guess
    printf("Unable to open file: %s, errno :%d\n", PASSWD_PATH, fd_passwd); 

  close(fd_passwd);

  if((fd_passwd = open(PASSWD_PATH, O_RDWR)) < 0){
    // void return type means failure doesn't matter, I guess
    printf("Unable to open file: %s, errno :%d\n", PASSWD_PATH, fd_passwd); 
  }
  printf("file opened: %s, :%d\n", PASSWD_PATH, fd_passwd);
  

  
}

void 
endpwent(void)
{
  close(fd_passwd);
  fd_passwd = 0;
}

struct passwd *
getpwnam(const char *name)
{
    struct passwd *p = malloc(sizeof(struct passwd));
    return p;
  // TODO: implement `getpwnam`
}

struct passwd *
getpwuid(uint uid)
{
    struct passwd *p = malloc(sizeof(struct passwd));
    return p;
  // TODO: implement `getpwuid`
}

// Write given passwd entry into passwd file
int 
putpwent(const struct passwd *p)
{
  // TODO: implement `putpwent`
  // tip: can use `fprintf` to write to 

//   char *nameLength = iota(strlen(p->name));

//   write(fd_passwd,nameLength, strlen(nameLength));
//   write(fd_passwd,":", 1);
  write(fd_passwd,p->name, strlen(p->name));
  write(fd_passwd,":", 1);
  write(fd_passwd,"\n", 1);
  //fprintf(fd_passwd,p->name);
  return 1;
}
