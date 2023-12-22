#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int n = 3; //set n
    for(int i = 0; i < n; i ++) {
    	printf("I am the parent process(id:%d)\n",getpid());
    	int rc = fork();
    	
    
    	if ( rc < 0 ) {
    		printf("error \n");
    	} if (rc > 0) {	
    		wait(0);
    	
    	} else {
    		
    		printf("I am the child process(id:%d)\n",getpid());	
    		break;
    	}
    	
    	
    
    }
    
    return 0;
}
