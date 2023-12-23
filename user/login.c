#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/pwd.h"
#include "kernel/fcntl.h"

#define MAX_ATTEMPTS 3
#define MAXLEN 30

char *argv[] = {"sh", 0};

int main(void)
{

    // TODO: check if pwd file exists, if not, create root user

    setpwent();

    // create root users
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

    // after writing root to file open file as read
    // open file as read/write
    setpwent();
    // initialize users from files
    getpwent();
    // close file
    endpwent();
    setpwent();

    // TODO: prompt for username
    // char username[5];
    char* username = malloc(sizeof(char)* 64);
    // for(int i = 0; i < 64; i++){
    //     username[i] = 0;
    // }
    printf("username: ");
    gets(username, 64);
    printf(username);
    printf("%d",strlen(username));
    username[strlen(username)-1] = 0;
    // read(0, username, sizeof(username));

    // TODO: prompt for password
    // char password[64];
    char *password = malloc(sizeof(char)*64);
    for(int i = 0; i < 64; i++){
        password[i] = 0;
    }
    printf("password: ");
    // read(0, password, sizeof(password));
    //get up to 64 characters from the command line
    gets(password,64);
    password[strlen(password)-1] = 0;
    // TODO: authenticate user given input
    // check username
    

    struct passwd *find_username = getpwnam(username);
    if (find_username)
    {
        printf("username found \n");
        //check for password
        if (strcmp(find_username->hashedPassword, password) != 0)
        {
            printf("incorrect password. \n");
        }else{
            printf("correct password login successful \n");
        }
    }

    printf("address: %d\n", find_username);

    // TODO: upon successful authentication: set process UID, GID to identity
    // dk how to do that
    int print_gid = getgid();
    printf("gid:%d\n", print_gid);
    int print_pid = getuid();
    printf("uid:%d\n", print_pid);

    

    // setuid(find_username->uid);
    // setgid(find_username->gid);
    setuid(69);
    setgid(420);

    print_gid = getgid();
    printf("gid:%d\n", print_gid);
    print_pid = getuid();
    printf("uid:%d\n", print_pid);

    // upon successful authentication: execute shell
    printf("exiting to sh\n");
    exec("sh", argv);
}