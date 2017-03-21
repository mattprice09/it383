#include <pthread.h>
void* thread_function (void* thread_arg)
{
	/* Do work here... */
	int i = 0;
	while (i < 1000000000) {
		i++;
	}
	return NULL;
}
int main ()
{
	pthread_attr_t attr;
	pthread_t thread;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	pthread_create (&thread, &attr, &thread_function, NULL);
	pthread_attr_destroy (&attr);
	/* Do work here... */
	/* No need to join the second thread. */
	return 0;
}
