#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;
int bandera = 0;

void *jefe(void *p);
void *empleado1(void *p);

int main() {
	pthread_t hilo1, hilo2, hilo3, hilo4, hilo5, hilo6;
	pthread_create(&hilo1, NULL, jefe, NULL);
	pthread_create(&hilo2, NULL, empleado1, NULL);
	pthread_create(&hilo3, NULL, empleado1, NULL);
	pthread_create(&hilo4, NULL, empleado1, NULL);
	pthread_create(&hilo5, NULL, empleado1, NULL);
	pthread_create(&hilo6, NULL, empleado1, NULL);
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);

	return 0;
}

void *jefe(void *p) {
	pthread_mutex_lock(&candado);
	printf("Hola Mundo\n");
	bandera = 1;
	pthread_mutex_unlock(&candado);
	pthread_exit(NULL);
}

void *empleado1(void *p) {
	pthread_mutex_lock(&candado);
	if(bandera == 1) {
		printf("Buenos dias Jefe\n");
	} else {
		printf("Buenos dias\n");
	}
	pthread_mutex_unlock(&candado);
	pthread_exit(NULL);
}
