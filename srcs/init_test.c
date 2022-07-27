#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


typedef struct s_subchild
{
	int x;
	int y;
}	t_subchild;

typedef struct s_child
{
	t_subchild subchild;
}	t_child;

typedef struct s_parent
{
	t_child child;
}	t_parent;

int main(void)
{
	t_parent parent;

	bzero(&parent, sizeof(t_parent));
	printf("child.subchild.x = %d | child.subchild.y = %d\n", parent.child.subchild.x, parent.child.subchild.y);
	return (0);
}