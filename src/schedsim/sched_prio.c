#include "sched.h"

static task_t* pick_next_task_prio(runqueue_t* rq,int cpu){

   if (!rq) {
      printf ("Big time error!\n");
      return NULL;
   }
   task_t* t=head_slist(&rq->tasks);

   if (t){
      remove_slist(&rq->tasks,t);
   }
   return t;

}

static int compare_tasks_prio (void *t1,void *t2){

   task_t* tsk1=(task_t*)t1;
   task_t* tsk2=(task_t*)t2;
   if (tsk1 && tsk2) {
      return tsk1->prio-tsk2->prio;
   } else {
      printf ("Big Time error! \n");
      return 0;
   }
}

static void enqueue_task_prio(task_t* t,int cpu, int runnable){

   runqueue_t* rq=get_runqueue_cpu(cpu);
   if (!rq) {
      printf("Big time error!\n");
      return;
   }
   if (!t) {
      printf ("Big time error!\n");
      return;
   }
   //if the task is already on the run queue or its the idle task, do nothing
   if (t->on_rq || is_idle_task(t)){
      return;
   }

   if (t->flags & TF_INSERT_FRONT) {
      //Clear flag
      t->flags&=~TF_INSERT_FRONT;
      sorted_insert_slist_front(&rq->tasks, t, 1, compare_tasks_prio);  //Push task
   } else {
      sorted_insert_slist(&rq->tasks, t, 1, compare_tasks_prio);  //Push task
   }

   /* If the task was not runnable before, update the number of runnable tasks in the rq*/
   if (!runnable) {
      task_t* current=rq->cur_task;
      if (!current) {
         printf ("Big Time error! \n");
         return;
      }

      if (preemptive_scheduler && (t->prio < current->prio)) {
         rq->need_resched=TRUE;
         current->flags|=TF_INSERT_FRONT;
      }
   }
}


static void task_tick_prio(runqueue_t* rq,int cpu){

   task_t* current = rq->cur_task;
   task_t* next = head_slist(&rq->tasks);
  
   if (current && next) {
    	if(current->prio > next->prio && preemptive_scheduler){
        	rq->need_resched=TRUE; //force resched
    	}
   }
}

static task_t* steal_task_prio(runqueue_t* rq,int cpu){

   task_t* t=tail_slist(&rq->tasks);
   if (t) {
      remove_slist(&rq->tasks,t);
      t->on_rq=FALSE;
    }
    return t;
}

sched_class_t prio_sched= {
    .pick_next_task=pick_next_task_prio,
    .enqueue_task=enqueue_task_prio,
    .task_tick=task_tick_prio,
    .steal_task=steal_task_prio
};
