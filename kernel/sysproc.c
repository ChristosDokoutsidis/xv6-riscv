#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
//#include "user.h"

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

// In sysproc.c or another related file
// int argint(int n, int *ip) {
//   struct proc *p = myproc();

//     if (n >= 0 && n < 6) {
//         *ip = p->trapframe->a0 + n;
//         return 0;
//     }

//     return -1;  // Invalid argument index
// }


int sys_setpriority(void) {
    int num;

    if (argint(0, &num) < 0) {
        return -1;
    }

    if (num < 1 || num > 20) {
        return -1;  // Invalid priority value
    }

    myproc()->priority = num;
    return 0;
}

int sys_getpinfo(void) {
    struct pstat *pstat;

    if (argstr(0, (void *)&pstat, sizeof(struct pstat)) < 0) {
        return -1;
    }

    // Your implementation to fill the pstat structure with process information

    return 0;
}
