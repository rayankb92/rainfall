#include <stdio.h>

int main() {
	int width = 5;
	int num = 123;

	printf("%122d\n",num);
	printf("%2$*1$d\n", width, num);

	width = 10;
int precision = 2;
printf("%*.*f\n", width, precision, 123.45678);

		return 0;
}