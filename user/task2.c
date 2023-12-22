#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    //3 forks hence 7 childs + 1 parent = 8 prints of erp
    fork();
    wait(0); 
    fork();
    wait(0);
    fork();
    wait(0);
    
    printf("24443\n");
    
    return 0;
}
