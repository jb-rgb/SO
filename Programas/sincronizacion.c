#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int dato = 10;
pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;

void *incrementa(void *p);
void *decrementa(void *p);

int main() {
	int k = 25;	
	pthread_t hilo_1, hilo_2;
	pthread_create(&hilo_1, NULL, incrementa, NULL);
	pthread_create(&hilo_2, NULL, decrementa, NULL);
	pthread_join(hilo_1, NULL);
	pthread_join(hilo_2, NULL);
	printf("dato = %i\n", dato);
	return 0;
}

void *incrementa(void *p) {
	for(int k = 0; k < 10000; k++) {
		pthread_mutex_lock(&candado);
		dato++;
		pthread_mutex_unlock(&candado);
	}
	pthread_exit(NULL);
}

void *decrementa(void *p) {
	for(int k = 0; k < 10000; k++) {
		pthread_mutex_lock(&candado);
		dato--;
		pthread_mutex_unlock(&candado);
	}
	pthread_exit(NULL);
}
