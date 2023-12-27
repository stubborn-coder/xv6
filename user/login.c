#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAX_ATTEMPTS 3
#define MAXLEN 30

char *argv[] = {"sh", 0};

void run_shell(void)
{
}

void create_root_user()
{
  setpwent();
  char *root = "root";

  struct passwd *p = malloc(sizeof(struct passwd));

  p->name = malloc(sizeof(root));
  strcpy(p->name, root);

  p->hashedPassword = malloc(sizeof(root));
  strcpy(p->hashedPassword, root);

  p->gecos = malloc(sizeof(char));
  strcpy(p->gecos, "");

  p->shell = malloc(sizeof(char));
  strcpy(p->shell, "");

  char *dir = malloc(sizeof(char) * (strlen("/home/") + strlen(root) + 1));
  concat("/home/", root, dir);
  p->dir = malloc(sizeof(dir));
  strcpy(p->dir, dir);

  p->uid = 0;
  p->gid = 0;

  // write root user to the file
  putpwent(p);
  // close file
  endpwent();
}

int main(void)
{

  // TODO: check if pwd file exists, if not, create root user

  if (open(PASSWD_PATH, O_RDWR) < 0)
  {
    // file does not exists
    printf("password file does not exist, creating default root user\n");
    char *useradd_argv[] = {"useradd", 0};
    exec("/useradd", useradd_argv);
    
    printf("number of users:%d\n", noOfUsers());
    // create_root_user();
    wait(0);
  }

  // create root users
  //

  // after writing root to file open file as read
  // open file as read/write
  setpwent();
  // initialize users from files
  getpwent();

  // close file
  endpwent();
  // printf("number of users:%d\n", noOfUsers());
  if(noOfUsers() == 0){
    char *useradd_argv[] = {"useradd", 0};
    exec("/useradd", useradd_argv);
    
    // printf("number of users:%d\n", noOfUsers());
    // create_root_user();
    wait(0);
  }

  
  // TODO: prompt for username

  char *username = malloc(sizeof(char) * 32);
  printf("username: ");
  gets(username, 32);
  username[strlen(username) - 1] = 0;

  // TODO: prompt for password

  

  // TODO: authenticate user given input
  // check username

  struct passwd *find_username = getpwnam(username);
  // printf("find_user_name:%d\n", find_username);

  while (!find_username)
  {

    
    printf("Username does not exists\n");
    printf("username: ");
    gets(username, 64);
    username[strlen(username) - 1] = 0;
    find_username = getpwnam(username);
    // check for password
    
  }

  char *password = malloc(sizeof(char) * 32);
  printf("password: ");

  while (find_username)
  {

    gets(password, 64);
    password[strlen(password) - 1] = 0;
    // check for password
    if (strcmp(find_username->hashedPassword, password) != 0)
    {
      printf("incorrect password. \n");
      printf("Re-enter password: ");
    }
    else
    {
      printf("correct password login successful \n");
      
      break;
    }
  }

  // TODO: upon successful authentication: set process UID, GID to identity

  int print_gid = getgid();
  // printf("gid:%d\n", print_gid);
  int print_pid = getuid();
  // printf("uid:%d\n", print_pid);

  setuid(find_username->uid);
  setgid(find_username->gid);
  // setuid(69);
  // setgid(420);


  print_gid = getgid();
  printf("gid:%d\n", print_gid);
  print_pid = getuid();
  printf("uid:%d\n", print_pid);

  // printf("no of users:%d\n", noOfUsers());

  // upon successful authentication: execute shell
  exec("sh", argv);
  int pid = fork();
  if (pid < 0)
  {
    printf("login: fork failed\n");
    exit(0);
  }
  if (pid == 0)
  {
    printf("login: starting sh with uid %d\n", find_username->uid);

    // int gids[1] = { uidtogid(uid) };
    // setgroups(1, gids);
    setuid(find_username->uid);
    // chdir(uidtodir(uid));
    
    printf("login: exec sh failed\n");
    exit(0);
  }

  int wpid;
  while ((wpid = wait(&pid)) >= 0 && wpid != pid)
    printf("zombie!\n");
  return 0;
}
