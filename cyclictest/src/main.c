#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <math.h>


#define SINGLE_TICK 1e-9 
#define TICKS_IN_PERIOD 1e7 


RT_TASK cyclic_task;

void cyclic_task_thread(void *arg)
{
  RT_TASK_INFO taskinfo;
  RT_TASK *thistask;
  RTIME t0;
  const int NLOOPS = 100;
  int ret = 0;

  thistask = rt_task_self();
  rt_task_inquire(thistask, &taskinfo);
  int ctr = 0;

  //Print task info
  printf("Initializing real-time task %s with period of %f ms ....\n", taskinfo.name, 
	 TICKS_IN_PERIOD*SINGLE_TICK*1000);

  //Make task periodic at specified period
  rt_task_set_periodic(NULL, TM_NOW, TICKS_IN_PERIOD);

  //Read timer at start to measure loop times
  t0 = rt_timer_read();

  //Start real-time loop
  while(ctr < NLOOPS){
    printf("# Iters: %d, delta: %2.4f ms\n", ctr, (rt_timer_read() - t0)/1000000.0);
    ctr++;
    rt_task_wait_period(NULL);
  }
}

int main(int argc, char **argv)
{
  char task_name[32];
  const int cyclic_task_prio = 50;

  //Lock memory to prevent program being swapped out.
  mlockall(MCL_CURRENT | MCL_FUTURE);
    
  printf("\n************* Starting task **************\n");

  //Create struct for xenomai task.
  sprintf(task_name, "cyclictask");
  rt_task_create(&cyclic_task, task_name, 0, cyclic_task_prio, 0);

  //Start task
  rt_task_start(&cyclic_task, &cyclic_task_thread, 0);

  //Wait for the task to complete or exit.
  pause();

  return 0;
}
