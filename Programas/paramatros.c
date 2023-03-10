#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int n, char *argv[]) {
	
	for(int j = 0; j < n; j++) {
		fprintf(stdout, "\n%s ", argv[j]);
	}

	return 0;
}
