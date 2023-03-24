#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main() {
	FILE *mipipe;
	char buffer[128];
	int i = 0;

	if (mkfifo("pipetest",
		S_IWUSR | /* EL usuario puede escribir */
		S_IRUSR | /* El usuario puede leer */
		S_IRGRP | /* El grupo puede leer */
		S_IROTH   /* Otros pueden leer */
	   ) != 0)
		printf("Hubo un problema al crear la pipe \n");
	
	mipipe = fopen("pipetest", "w");

	while (i < 10) {
		sprintf(buffer, "CADENA ENVIADA Numero: %i\n", i + 1);
		fputs(buffer, mipipe);
		i++;
	}

	fclose(mipipe);

}
