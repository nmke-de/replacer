#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "replace.h"

extern char **environ;
#define sys(args) execve(*(args), (args), environ)

#define opt(o) (strcmp(argv[i], o) == 0)

int main(int argc, char **argv) {
	// argparsing
	char *tfilename = "../src/template.html";
	char *filename = "../src/index.md";
	char *cname = "./md2html";
	char *title = "Title";
	for (int i = 1; i < argc; i++)
		if (*(argv[i]) != '-')
			filename = argv[i];
		else if (i + 1 == argc)
			return 1;
		else if (opt("-t"))
			tfilename = argv[++i];
		else if (opt("-c"))
			cname = argv[++i];
		else if (opt("-T"))
			title = argv[++i];
		else
			return 1;
	/*
	First, read from the template (hardcoded as ../src/template.html) and write it to stdout.
	Second, if you encounter a line that matches the String "<!-- CONTENT -->", then pipe your input (TODO from arguments, and TODO from stdin) through the compiler (which should also print to stdout). Wait for this to end.
	Third, finish reading the template and write it to stdout.

	TODO make custom <title> possible, perhaps by the first h1 in markdown?
	*/
	int tfd = open(tfilename, 0);
	char buf[20];
	*buf = 0;
	const char *CONTENT = "<!-- CONTENT -->";
	int content_pointer = 0;
	const int content_len = strlen(CONTENT);
	char c;
	while ((c = readc(tfd)) != -1) {
		if (c == CONTENT[content_pointer])
			buf[content_pointer++] = c;
		else if (content_pointer > 0) {
			write(1, buf, content_pointer);
			content_pointer = 0;
			*buf = 0;
		}
		if (content_pointer == content_len) {
			content_pointer = 0;
			*buf = 0;
			pid_t compiler = fork();
			if (compiler == 0)
				sys(((char *[]){cname, filename, NULL}));
			wait(&compiler);
			continue;
		} else if (content_pointer == 0)
			write(1, &c, 1);
	}
	close(tfd);
}
