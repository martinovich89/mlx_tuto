#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	printf("En unsigned int\n");

	unsigned char array1[4] = {0, 0, 0, 1};
	printf("%x\n", array1[3]);

	unsigned char array2[4] = {0, 0, 0, 255};
	printf("%x\n", (unsigned int)array2[3]);

	unsigned char array3[4] = {1, 0, 0, 0};
	printf("%x\n", (unsigned int)array3[0]);

	printf("\nEn unsigned int *\n");

	unsigned int color = *(unsigned int *)&array1[0];
	printf("%x\n", color);

	color = *(unsigned int *)&array2[0];
	printf("%x\n", color);

	color = *(unsigned int *)&array3[0];
	printf("%x\n", color);

	printf("\nEn unsigned char *\n");

	unsigned int *ptr = &color;
	printf("%x\n", ((unsigned char *)(ptr))[0]);
	printf("%x\n", ((unsigned char *)(ptr))[1]);
	printf("%x\n", ((unsigned char *)(ptr))[2]);
	printf("%x\n", ((unsigned char *)(ptr))[3]);
	return (0);
}