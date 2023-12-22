#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int n = 3; //set n
    for(int i = 0; i < n; i ++) {
    	int rc = fork();
    	
    
    	if ( rc < 0 ) {
    		printf("error \n");
    	}else if (rc == 0) {
    		printf("I am the child process(id:%d)\n", getpid());
    		break;
    	} else {
    		wait(0);
    		printf("I am the parent process(id:%d)\n",getpid());
    		
    	}
    
    }
    
    return 0;
}
