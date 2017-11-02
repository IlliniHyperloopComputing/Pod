#include <stdio.h>

int main() {
	printf("Test out of my prog\n");
	char* line = NULL;
	size_t len;
	getline(&line, &len, stdin);
	printf("My line<%s>\n", line);
	return 0;
}
