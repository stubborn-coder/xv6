#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAXLEN 30
#define MAXUSERS 5

// Global variables
int fd_passwd = 0;
int users_count = 0;
int setupUsers = 0;
int currentReturnUser = 0;
struct passwd *currentLoggedInUser;

// there can only be 5 users in xv6
struct passwd **UserDetails;

// Reads one line from buf at position i into field
int getpwfield(char *field, char *buf, int i)
{
  int c = 0;
  while (buf[i] != ':' && buf[i] != '\n')
  {
    field[c++] = buf[i++];
  }
  field[c] = '\0';
  return ++i;
}

void initializeUsers(void)
{
  // if (setupUsers)
  //   return;
  
  currentLoggedInUser = malloc(sizeof(struct passwd));
  struct stat *st = malloc(sizeof(struct stat));
  UserDetails = malloc(sizeof(struct passwd) * MAXUSERS);

  
  users_count = 0;
  stat(PASSWD_PATH, st);

  // printf("filesize: %d\n", st->size);

  char buff[st->size];
  // i will store the number of characters read
  read(fd_passwd, buff, sizeof(buff));

  // printf("i:%d\n", i);

  char name[64];
  char password[64];
  char char_gid_t[64];
  char char_uid_t[64];
  char dir[256];
  char shell[256];
  char gecos[256];

  int next = 0;
  while (next < st->size)
  {
    

    next = getpwfield(name, buff, next);
    // printf("username:%s\n", name);
    // printf("next%d\n", next);
    if (next >= st->size)
      break;

    next = getpwfield(password, buff, next);
    // printf("password:%s\n", password);
    // printf("next%d\n", next);
    if (next >= st->size)
    {
      printf("break\n");
      break;
    }

    next = getpwfield(char_gid_t, buff, next);
    // printf("gid:%s\n", char_gid_t);
    // printf("next%d\n", next);
    if (next >= st->size)
      break;

    next = getpwfield(char_uid_t, buff, next);
    // printf("uid:%s\n", char_uid_t);
    // printf("next%d\n", next);
    if (next >= st->size)
      break;

    next = getpwfield(gecos, buff, next);
    // printf("gecos:%s\n", gecos);
    // printf("next%d\n", next);
    if (next >= st->size)
      break;

    next = getpwfield(dir, buff, next);
    // printf("dir:%s\n", dir);
    // printf("next:%d\n", next);
    if (next >= st->size)
      break;

    next = getpwfield(shell, buff, next);
    // printf("next:%d\n", next);
    // printf("shell:%s\n", shell);

    // create the passwd object
    struct passwd *p = malloc(sizeof(struct passwd));
    p->name = malloc(sizeof(name) + 1);
    strcpy(p->name, name);
    p->hashedPassword = malloc(sizeof(password) + 1);
    strcpy(p->hashedPassword, password);
    p->shell = malloc(sizeof(shell));
    strcpy(p->shell, shell);
    p->dir = malloc(sizeof(dir));
    strcpy(p->dir, dir);
    p->gecos = malloc(sizeof(gecos));
    strcpy(p->gecos, gecos);
    p->uid = atoi(char_uid_t);
    p->gid = atoi(char_gid_t);
    
    //  store in users array
    *(UserDetails + users_count) = p;
    users_count++;
    if (next >= st->size)
      break;

    if (users_count > MAXUSERS)
      break;
  }
  // printf("end initializing \n");
  setupUsers = 1;
  
}

// Returns null on error
struct passwd *
getpwent(void)
{
  // if (!fd_passwd)
  // {
  //   setpwent();
  // } // open passwd file if fd_passwd not set

  if (!setupUsers)
  {
    initializeUsers();
  }

  int temp = currentReturnUser % users_count;
  // printf("currentReturn Users:%d\n", currentReturnUser);
  // printf("temp:%d\n", temp);
  // printf("MAX users %d\n", MAXUSERS);
  currentReturnUser++;
  // printf("return value : %s\n", UserDetails[0]->name);

  return UserDetails[temp];
}

void setpwent(void)
{
  if ((fd_passwd = open(PASSWD_PATH, O_CREATE | O_RDWR)) < 0)
    printf("Unable to open file: %s, errno :%d\n", PASSWD_PATH, fd_passwd);

  // printf("file opened: %s, :%d\n", PASSWD_PATH, fd_passwd);
}

void endpwent(void)
{
  close(fd_passwd);
  fd_passwd = 0;
}

struct passwd *
getpwnam(const char *name)
{
  if (!fd_passwd)
  {
    setpwent();
  }
  if (!setupUsers)
  {
    initializeUsers();
  }
  
  int i = 0;
  for (i = 0; i < users_count; i++)
  {
    // printf("user:%s\n", UserDetails[i]->name);
    if (strcmp(UserDetails[i]->name, name) == 0)
    {
      // printf("returning user:%s\n", UserDetails[i]->name);
      endpwent();
      return UserDetails[i];
    }
  }
  // printf("return NULL\n");
  endpwent();
  return 0;
}

struct passwd *
getpwuid(uint uid)
{
  if (!fd_passwd)
  {
    setpwent();
  }
  initializeUsers();
  for (int i = 0; i < users_count; i++)
  {
    if (UserDetails[i]->uid == uid)
    {
      // printf("returning user:%s\n", UserDetails[i]->name);
      endpwent();
      return UserDetails[i];
    }
  }
  endpwent();
  return 0;
}

// Write given passwd entry into passwd file
int putpwent(const struct passwd *p)
{
  initializeUsers();
  // printf("number of users %d\n",noOfUsers());
  if (users_count >= MAXUSERS)
  {
    write(1, "Max users reached\n", 18);
    
    return 0;
  }
  // if(!fd_passwd) setpwent();
  int count = 0;
  struct passwd *temp[users_count];

  

  write(fd_passwd, p->name, strlen(p->name));
  write(fd_passwd, ":", 1);

  write(fd_passwd, p->hashedPassword, strlen(p->hashedPassword));
  write(fd_passwd, ":", 1);

  char *gid_buff = malloc(sizeof(char) * 8);
  char *uid_buff = malloc(sizeof(char) * 8);

  itoa(p->gid, gid_buff, 10);
  itoa(p->uid, uid_buff, 10);

  write(fd_passwd, gid_buff, strlen(gid_buff));
  write(fd_passwd, ":", 1);

  write(fd_passwd, uid_buff, strlen(uid_buff));
  write(fd_passwd, ":", 1);

  write(fd_passwd, p->gecos, strlen(p->gecos));
  write(fd_passwd, ":", 1);

  write(fd_passwd, p->dir, strlen(p->dir));
  write(fd_passwd, ":", 1);

  write(fd_passwd, p->shell, strlen(p->shell));
  write(fd_passwd, ":", 1);

  // endpwent();

  return 1;
}

int noOfUsers(void)
{
  return users_count;
}
