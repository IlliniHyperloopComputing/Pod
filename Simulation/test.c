#include <stdio.h>

int main() {
	FILE* file = fopen("out.t", "w+");
	fprintf(file, "Test out of my prog\n");
	char* line = NULL;
	size_t len;
	ssize_t read;
	int lineNo = 0;
	int out = 0;
	while ((read = getline(&line, &len, stdin)) != -1) {
		double d = *((double*)(line+1));
		int id = line[0];
		fprintf(file, "#%d:<id:%d,size:%zu>", lineNo, id, read);
		fprintf(file, "%f\n", d);
		fflush(file);
		write(1, "\0a", 2);
		lineNo++;
	}
	fprintf(file, "ENDING");
	
	return 0;
}