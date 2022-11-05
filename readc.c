#include <unistd.h>
#include "replace.h"

#define BUFLEN 512

/*
read a single character from file with file descriptor fd. If no characters could be read, then return -1.
*/
char readc(int fd) {
	static char c[BUFLEN];
	static int ptr = BUFLEN;
	static int rl = BUFLEN;
	if (rl == ptr) {
		rl = read(fd, c, BUFLEN);
		ptr = 0;
	} else if (rl < 1)
		return -1;
	return c[ptr++];
}
