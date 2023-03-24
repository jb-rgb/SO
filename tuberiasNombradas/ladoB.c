#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main() {
	FILE *mipipe;
	char buffer[128];
	int i = 0;
	
	mipipe = fopen("pipetest", "r");

	while (!feof(mipipe)) {
		if(fgets(buffer, 128, mipipe))
			printf("RECIBIDO: %s", buffer);
	}

	fclose(mipipe);

}
