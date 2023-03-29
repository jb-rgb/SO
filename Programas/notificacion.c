#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void rutinaServicio(int signalNumber);

int main() {
    signal(SIGSTOP, &rutinaServicio);

    while(1) {
        printf("durmiendo\n");
        sleep(2);
    }
    return 0;
}

void rutinaServicio(int signalNumber) {
    printf("La notificacion recibida tiene el numero: %i\n", signalNumber);
}