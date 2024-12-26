#include <stdlib.h>
#include <string.h>
#include <stdio.h>	


void main(void)
{
	int m = 0;
	printf("m = %d\n", m);
	printf("change m%n\n", &m);
	int a,b = 0;
	b = 12;
	printf("a = %2$d -- b = %1$d\n", a, b);
	printf("m = %d\n", m);
  return;
}