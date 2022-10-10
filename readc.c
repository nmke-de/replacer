#include <unistd.h>
#include "replace.h"

/*
read a single character from file with file descriptor fd. If no characters could be read, then return -1.
*/
char readc(int fd) {
	static char c[2];
	if (read(fd, c, 1) < 1)
		return -1;
	return *c;
}
