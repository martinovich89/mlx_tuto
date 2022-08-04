#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

typedef struct s_pos
{
	double x;
	double y;
}	t_pos;

typedef struct s_vector
{
	double x;
	double y;
	t_pos o;
}	t_vector;

int main(void)
{
	t_vector a;
	t_vector b;
	double angle;
	t_vector point;

	a.x = -1; a.y = 1; b.x = -0.8; b.y = 3;
	a.o.x = 10; a.o.y = 10;

	angle = acos((a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2))));
 	printf("angle = %lf\n", angle);

	point.x = 11; point.y = 9.1;

	int left = (point.y - a.o.y)*(a.x) > (point.x - a.o.x)*(a.y);
	printf("%d\n", left);
	return (0);
}