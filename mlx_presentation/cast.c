#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
	unsigned char array[4] = {255, 255, 255, 255};
	printf("%d\n", (int)array[0]);
	printf("%d\n", *(int *)&array[0]);
	printf("dst[0]<<0 = %x | dst[1]<<8 = %x | dst[2]<<16 = %x | dst[3]<<24 = %x\n", (unsigned int)(array[0])<<0, (unsigned int)(array[1])<<8, (unsigned int)(array[2])<<16, (unsigned int)(array[3])<<24);
	return (0);
}