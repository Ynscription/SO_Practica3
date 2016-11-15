#include "barrier.h"
#include <errno.h>


#ifdef POSIX_BARRIER

/* Wrapper functions to use pthread barriers */

int sys_barrier_init(sys_barrier_t* barrier, unsigned int nthreads)
{
	return pthread_barrier_init(barrier,NULL,nthreads);
}

int sys_barrier_destroy(sys_barrier_t* barrier)
{
	return pthread_barrier_destroy(barrier);
}

int sys_barrier_wait(sys_barrier_t *barrier)
{
	return pthread_barrier_wait(barrier);
}

#else


/* Barrier initialization function */
int sys_barrier_init(sys_barrier_t *barrier, unsigned int nr_threads)
{
	 barrier->nr_threads_arrived=0; //inicializamos la variable a 0
	 barrier->max_threads = nr_threads; //maximo de hilos de la barrera
	 pthread_mutex_init(&barrier->mutex, NULL); //iniciar una variable de tipo mutex dentro de nuestra estructura en la barrera
	 pthread_cond_init(&barrier->cond, NULL); //iniciar la variable condicional

    return 0;
}

/* Destroy barrier resources */
int sys_barrier_destroy(sys_barrier_t *barrier)
{
	/* Destroy synchronization resources associated with the barrier
	      ... To be completed ....
	*/
	pthread_mutex_destroy(&barrier->mutex); //destruimos mutex
	pthread_cond_destroy(&barrier->cond); //destruimos variable condicional
	return 0;
}

/* Main synchronization operation */
int sys_barrier_wait(sys_barrier_t *barrier)
{
	/* Implementation outline:
	   - Every thread arriving at the barrier adquires the lock and increments the nr_threads_arrived
	    counter atomically
	     * In the event this is not the last thread to arrive at the barrier, the thread
	       must block in the condition variable
	     * Otherwise...
	        1. Reset the barrier state in preparation for the next invocation of sys_barrier_wait() and
	        2. Wake up all threads blocked in the barrier
	   - Don't forget to release the lock before returning from the function

	    ... To be completed ....
	*/
	 pthread_mutex_lock(&barrier->mutex); //el hilo que llega a la barrera adquiere el  mutex

	barrier->nr_threads_arrived++; //incrementa de forma atomica el contador nr_threads_arrived
	if (barrier->nr_threads_arrived != barrier->max_threads) {  //Si aun faltan mas hilos por llegar el hilo se bloquea con la V.cond
			pthread_cond_wait(&barrier->cond, &barrier->mutex);        //hilo bloqueado en la varialbe cond a la espera de que llegue el último
	}
	if (barrier->nr_threads_arrived == barrier->max_threads)//Si es el ultimo hilo en llegar...
	{
	       barrier->nr_threads_arrived=0;  //1 resetea el estado de la barrera para la siguiente sys_barrier_wait()
	       pthread_cond_broadcast(&barrier->cond); //2 despierta a todos los hilso bloqueados en la V.cond

	}

	pthread_mutex_unlock(&barrier->mutex);// Liberar mutex antes de salir de la funcion
    return 0;
}

#endif /* POSIX_BARRIER */
