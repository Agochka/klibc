#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
  int i;

  /* Verify envp == environ */
  printf("Verifying envp == environ... %s\n",
	 (envp == environ) ? "ok" : "ERROR");

  /* Test environ */
  printf("PATH = %s\n", getenv("PATH"));
  printf("HOME = %s\n", getenv("HOME"));
  printf("TERM = %s\n", getenv("TERM"));
  printf("USER = %s\n", getenv("USER"));

  /* Test argc/argv */
  printf("argc = %d\n", argc);
  for ( i = 0 ; i < argc ; i++ ) {
    printf("argv[%2d] = %s\n", i, argv[i]);
  }

  return 0;
}
