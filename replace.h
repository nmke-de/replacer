#pragma once
char readc(int);

typedef struct {
	const char *pattern;
	const int len;
	char *buf;
	int ptr;
} substring;

// This init macro requires string.h and alloca.h
#define subpattern(str) \
	(substring) { (str), strlen((str)), alloca(sizeof(char) * strlen((str))), 0 }
