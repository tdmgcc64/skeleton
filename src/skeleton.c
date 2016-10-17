/*
  skeleton.c

  > mingw32-make -f makefile.tdmgcc64
*/

#include <stdio.h>

int main(int ac, char **av)
{
  int i;
  printf("sizeof(size_t): %08x\n", sizeof(size_t));
  for(i = 0; i < ac; ++i)
    printf("%d %s\n", i, av[i]);
  return 0;
}
