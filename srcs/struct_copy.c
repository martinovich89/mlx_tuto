#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stddef.h>
#include <strings.h>

typedef struct	s_pos
{
	double x;
	double y;
}	t_pos;

typedef struct	s_intpos
{
	int	x;
	int	y;
}	t_intpos;

typedef struct	s_vector
{
	double	x;
	double	y;
	t_pos	o;
	double	dist;
}	t_vector;

typedef struct	s_rc
{
	t_vector	pos;
	t_vector	dir;
	t_vector	plane;
	double		camX;
	t_vector	ray;
	t_intpos	map;
	t_vector	sideDist;
	t_vector	deltaDist;
	double		perpWallDist;
	t_intpos	step;
	int			hit;
	int			side;
	double		ray_dirDelta;
	double		ray_dirAngle;
}	t_rc;

typedef struct	s_rndr
{
	int	lineHeight;
	int	drawStart;
	int	drawEnd;
	double	wallX;
	double	texX;
	double	texY;
	double	ratio;
}	t_rndr;

typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		w;
	int		h;
}	t_data;

typedef struct	s_sprite
{
	t_data			tex;
	t_vector		pos;
	double			dist;
	double			perpDist;
	size_t			height;
	double			angle;
	double			sp_dirDelta;
	double			ray_spDelta;
	int				draw_start;
	int				draw_end;
	int				spriteX;
	struct s_sprite	*next;
}	t_sprite;

void	print_struct(t_sprite *sprite)
{
	(void)sprite;
}

int main(void)
{
	t_sprite sprite;
	bzero(&sprite, sizeof(t_sprite));
	t_sprite cpy;
	cpy = sprite;
	print_struct(&cpy);
	return (0);
}