/*
 * strncasecmp.c
 */

#include <string.h>
#include <ctype.h>

int strncasecmp(const char *s1, const char *s2, size_t n)
{
	char *n1, *n2;
	int i, retval;

	n1 = strndup(s1, n);
	n2 = strndup(s2, n);

	for (i = 0; i < strlen(n1); i++)
		n1[i] = toupper(n1[i]);
	for (i = 0; i < strlen(n2); i++)
		n2[i] = toupper(n2[i]);
	retval = strcmp(n1, n2);
	free(n1);
	free(n2);
	return retval;
}
