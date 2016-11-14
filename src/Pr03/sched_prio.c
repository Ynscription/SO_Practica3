#include "sched.h"

/*struct prio_data {
    int remaining_ticks_slice;
};*/

/*static int task_new_prio(task_t* t)
{
    struct prio_data* cs_data=malloc(sizeof(struct prio_data));

    if (!cs_data)
        return 1;  // Cannot reserve memory

    // initialize the quantum
    cs_data->remaining_ticks_slice=rr_quantum;
    t->tcs_data=cs_data;
    return 0;
}

static void task_free_prio(task_t* t)
{
    if (t->tcs_data) {
        free(t->tcs_data);
        t->tcs_data=NULL;
    }
}*/

static task_t* pick_next_task_prio(runqueue_t* rq,int cpu)
{
	if (!rq) {
		printf ("Big time error!\n");
		return NULL;
	}
    task_t* t=head_slist(&rq->tasks);

    /* Current is not on the rq -> let's remove it */
    if (t){
        remove_slist(&rq->tasks,t);
	}
    return t;
}

static int compare_tasks_prio (void *t1,void *t2)
{
    task_t* tsk1=(task_t*)t1;
    task_t* tsk2=(task_t*)t2;
	if (tsk1 && tsk2) {
		return tsk1->prio-tsk2->prio;
	}
    else {
		printf ("Big Time error! \n");
		return 0;
	}
}

static void enqueue_task_prio(task_t* t,int cpu, int runnable)
{
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
	//
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


        /* Trigger a preemption if this task has a higher priority than current */
		/*if (is_idle_task(current)) {
			return;
		}*/
        if (preemptive_scheduler && (t->prio < current->prio)) {
            rq->need_resched=TRUE;
            current->flags|=TF_INSERT_FRONT; /* To avoid unfair situations in the event
                                                another task with the same priority wakes up as well*/
        }
    }
}


static void task_tick_prio(runqueue_t* rq,int cpu)
{
    task_t* current = rq->cur_task;
    task_t* next = head_slist(&rq->tasks);
    //struct rr_data* cs_data=(struct rr_data*) current->tcs_data;

    /*if (is_idle_task(current))
        return;

    if(current->remaining_ticks_slice<=0){
        rq->need_resched=TRUE; //force resched
    }

    if(current->runnable_ticks_left==1){
        rq->nr_runnable--; //la tarea o bien está saliendo o se va a dormir justo ahora
    }*/
	if (current && next) {
    	if(current->prio > next->prio && preemptive_scheduler){
        	rq->need_resched=TRUE; //force resched
    	}
	}

}

static task_t* steal_task_prio(runqueue_t* rq,int cpu)
{
    task_t* t=tail_slist(&rq->tasks);
    if (t) {
        remove_slist(&rq->tasks,t);
        t->on_rq=FALSE;
        //rq->nr_runnable--;
    }
    return t;
}

sched_class_t prio_sched= {
    //.task_new=task_new_prio,
    //.task_free=task_free_prio,
    .pick_next_task=pick_next_task_prio,
    .enqueue_task=enqueue_task_prio,
    .task_tick=task_tick_prio,
    .steal_task=steal_task_prio
};
