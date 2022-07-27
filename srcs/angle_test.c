#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

typedef struct s_vector
{
	double x;
	double y;
}	t_vector;

int main(void)
{
	t_vector a;
	t_vector b;
	double angle;

	a.x = 0.0; a.y = 1.0; b.x = 1.0; b.y = 0.0;

	angle = acos((a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2))));
 	printf("angle = %lf\n", angle);
	return (0);
}