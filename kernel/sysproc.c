#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

// TODO: write getuid, setuid, getgid, setgid
//done 
uid_t sys_getuid(void){

  return myproc()->uid;
}

uint64 sys_setuid(){

  int uid = -1;

  //load a0 into uid
	argint(0, &uid);
		if(uid == -1){
      return -1;
    }

	struct proc* currProc=myproc();
	if(currProc->uid != 0)
		return -1;
	
	currProc->uid=uid;
		
	return 1;
}

uint64 sys_setgid(void){

  int gid = -1;

  //load a0 into uid
	argint(0, &gid);
		if(gid == -1){
      return -1;
    }

	struct proc* currProc=myproc();
	if(currProc->gid != 0)
		return -1;
	
	currProc->gid=gid;
		
	return 1;
}

gid_t sys_getgid(void){
  
  return myproc()->gid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
