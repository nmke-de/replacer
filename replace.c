#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <alloca.h>
#include "replace.h"

#ifndef alloca
#define alloca __builtin_alloca
#endif

extern char **environ;
#define sys(args) execve(*(args), (args), environ)

#define opt(o) (strcmp(argv[i], o) == 0)

int main(int argc, char **argv) {
	// argparsing
	char *tfilename = "../src/template.html";
	char *filename = "../src/index.md";
	char *cname = "./md2html";
	char *title = "Title";
	char *date = "1970-01-01 00:00";
	char *uri = "about:blank";
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
		else if (opt("-D"))
			date = argv[++i];
		else if (opt("-U"))
			uri = argv[++i];
		else
			return 1;
	/*
	First, read from the template <tfilename> and write it to stdout.
	Second, if you encounter a line that matches the String "<!-- CONTENT -->", then pipe your input (TODO from stdin) through the compiler (which should also print to stdout).
	Similar procedure for the other strings ("<!-- URI -->". "<!-- TITLE -->", "<!-- URI -->")
	Wait for this to end.
	Third, finish reading the template and write it to stdout.
	*/
	// The following algorithm assumes that all substring patterns begin with the same character and end with the same character.
	int tfd = open(tfilename, 0);
	char buf[20];
	int ptr = 0;
	substring _content = subpattern("<!-- CONTENT -->");
	substring _title = subpattern("<!-- TITLE -->");
	substring _date = subpattern("<!-- DATE -->");
	substring _uri = subpattern("<!-- URI -->");
	char c;
	while ((c = readc(tfd)) != -1) {
		if (c == _content.pattern[ptr]) {
			_content.matching = 1;
			buf[ptr++] = c;
		} else if (c == _title.pattern[ptr]) {
			_content.matching = 0;
			_title.matching = 1;
			buf[ptr++] = c;
		} else if (c == _date.pattern[ptr]) {
			_title.matching = 0;
			_date.matching = 1;
			buf[ptr++] = c;
		} else if (c == _uri.pattern[ptr]) {
			_date.matching = 0;
			_uri.matching = 1;
			buf[ptr++] = c;
		} else if (ptr > 0) {
			write(1, buf, ptr);
			ptr = 0;
			//*buf = 0;
		}
		if (ptr == _content.len && _content.matching) {
			ptr = 0;
			_content.matching = 0;
			//*buf = 0;
			pid_t compiler = fork();
			if (compiler == 0)
				sys(((char *[]){cname, filename, NULL}));
			wait(&compiler);
		} else if (ptr == _title.len && _title.matching) {
			ptr = 0;
			_title.matching = 0;
			write(1, title, strlen(title));
		} else if (ptr == _date.len && _date.matching) {
			ptr = 0;
			_date.matching = 0;
			write(1, date, strlen(date));
		} else if (ptr == _uri.len && _uri.matching) {
			ptr = 0;
			_uri.matching = 0;
			write(1, uri, strlen(uri));
		} else if (ptr == 0)
			write(1, &c, 1);
	}
	close(tfd);
}
