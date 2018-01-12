#include <stdio.h>

int main() {
	FILE* file = fopen("out.t", "w+");
	fprintf(file, "Test out of my prog\n");
	char* line = NULL;
	size_t len;
	ssize_t read;
	int lineNo = 0;
	while ((read = getline(&line, &len, stdin)) != -1) {
		double d = *((double*)(line+1));
		fprintf(file, "%d:<%zu>", lineNo, read);
		fprintf(file, "%f\n", d);
		fflush(file);
		lineNo++;
	}
	fprintf(file, "ENDING");
	
	return 0;
}
