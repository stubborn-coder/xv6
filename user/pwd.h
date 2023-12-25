#define PASSWD_PATH "./password.txt"


struct details {
  uint hash;
  uint rand;
};
struct passwd {

  char    *name;   //User's login name. 
  struct details p_passwd; //contains *hash and rand
  uid_t    uid;   //Numerical user ID. 
  gid_t    gid;   //Numerical group ID. privleges
  char *gecos;  // user details
  char    *dir;    //Initial working directory. 
  char    *shell; //Program to use as shell. 
  char *hashedPassword; // hashedpassword
  
 
};



// taken from man://pwd.h (POSIX)
struct passwd *getpwent(void);
void setpwent(void);
void endpwent(void);
struct passwd *getpwnam(const char *);
// int getpwnam_r(const char *, struct passwd *, char *, size_t, struct passwd **);
struct passwd *getpwuid(uint);
// int getpwuid_r(uid_t, struct passwd *, char *, size_t, struct passwd **);
int putpwent(const struct passwd *);
int noOfUsers(void);

