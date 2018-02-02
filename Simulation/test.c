#include <stdio.h>

int main() {
	FILE* file = fopen("out.t", "w+");
	fprintf(file, "Test out of my prog\n");
	char* line = NULL;
	size_t len;
	ssize_t read;
	int lineNo = 0;
	int out = 0;
	
	char msg[2];
	msg[0] = 1;
	msg[1] = 1;
	write(1, msg, 2);
	int counter = 0;
	while ((read = getline(&line, &len, stdin)) != -1) {
		double d = *((double*)(line+1));
		int id = line[0];
		fprintf(file, "#%d:<id:%d,size:%zu>", lineNo, id, read);
		fprintf(file, "%f\n", d);
		fflush(file);
		
		msg[0] = 2;
		if (counter > 10000) {
			msg[0] = 3;
			msg[1] = 1;
		}
		else if (counter > 5000) {
			msg[1] = 0;
		} else {
			msg[1] = 255;
		}
		write(1, msg, 2);
		lineNo++;
		counter++;
	}
	fprintf(file, "ENDING");
	
	return 0;
}
