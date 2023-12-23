#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAXLEN 64 
#define PASSWORD_LEN 8

uint
create_uid(void)
{
  struct passwd *p;
  int uid = 0; // first user created will be root (uid 0)
  while((p = getpwuid(uid))){
    // free(p);
    uid++;
  }
  return uid;
}

int
main(int argc, char *argv[])
{
  // TODO: prompt for username

  printf("Please enter username: ");
//   char* username;
  char username_buf[MAXLEN];
  read(0,username_buf, sizeof(username_buf));

  // TODO: check if user already exists

  struct passwd *p = getpwnam(username_buf);
  while(p = getpwnam(username_buf)  > 0){
    printf("Username already exists please enter a new username.\n");
    printf("Please enter username: \n");
    read(0,username_buf, sizeof(username_buf));

  }


  // TODO: prompt for password
  
  printf("Please enter password: ");
//   char* password;
  char password_buff[MAXLEN];
  read(0,password_buff,sizeof(password_buff));

  // TODO: hash and salt password
  //will do later

  // TODO; create user account entry
  //get uuid, write to file
  int unique_uid = create_uid();
  struct passwd *new_entry = malloc(sizeof(struct passwd));
  
  new_entry->name = malloc(sizeof(username_buf));
  strcpy(new_entry->name,username_buf);
  
  new_entry->hashedPassword = malloc(sizeof(password_buff));
  strcpy(new_entry->hashedPassword, password_buff);
  
  new_entry->shell = malloc(sizeof(char));
  strcpy(new_entry->shell, "");
  
  new_entry->gecos = malloc(sizeof(char));
  strcpy(new_entry->gecos, "");
  
  char *dir = malloc( sizeof(char) * (strlen("/home/") + strlen(username_buf)+1));
  concat("/home/",username_buf, dir);
  new_entry->dir = malloc(sizeof(dir));
  strcpy(new_entry->dir, dir);
  
  new_entry->uid = unique_uid;
  new_entry->gid = 1000;
  
  putpwent(new_entry);


  return 1;
}
