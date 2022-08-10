#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 600

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define SPRITE_THEORETICAL_WIDTH 1
#define MIN_SPRITE_DRAW_RANGE 0.05

#define S_SPRITE "textures/toto.xpm"

#define PI 3.1415926535

char	map[MAP_HEIGHT][MAP_WIDTH] = {
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'S', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'S', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}
};

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
	double			x;
	double			y;
	t_pos			o;
	double			dist;
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
	bool		hit;
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
	double			sp_rayDelta;
	int				draw_start;
	int				draw_end;
	double			spriteX;
	char			type;
	struct s_sprite	*next;
}	t_sprite;

typedef struct	s_list
{
	t_sprite	*first;
	size_t		size;
}	t_list;

typedef struct	s_struct
{
	void		*mlx;
	void		*win;
	t_data		img;
	int			background;
	t_rc		rc;
	int			up;
	int			strafe_left;
	int			down;
	int			strafe_right;
	int			left;
	int			right;
	t_list		sprites;
	t_list		sorted_sprites;
	t_data		tex;
	t_rndr		rndr;
}	t_struct;

int	is_left_side(t_vector *dir, t_vector *pos)
{
	return ((pos->y - dir->o.y)*(dir->x) > (pos->x - dir->o.x)*(dir->y));
}

void	rotation(t_vector *dir, double angle)
{
	double	old_x;

	old_x = dir->x;
	dir->x = old_x * cos(angle) - dir->y * sin(angle);
	dir->y = old_x * sin(angle) + dir->y * cos(angle);
}

void	cam_calc(t_struct *cub, size_t x)
{
	cub->rc.camX = 2.0 * (double)x / (double)(WIDTH) - 1.0; //x-coordinate in camera space
	cub->rc.ray.y = cub->rc.dir.y + cub->rc.plane.y * cub->rc.camX;
	cub->rc.ray.x = cub->rc.dir.x + cub->rc.plane.x * cub->rc.camX;
	cub->rc.ray.o.x = cub->rc.pos.x;
	cub->rc.ray.o.y = cub->rc.pos.y;
}

void	sp_dist_calc(t_struct *cub, t_sprite *sprite)
{
	t_vector pos;

	pos = cub->rc.pos;
	sprite->dist = sqrt(pow(sprite->pos.x - pos.x, 2) + pow(sprite->pos.y - pos.y, 2));
}

void	sp_angle_calc(t_struct *cub, t_sprite *sprite)
{
	t_vector a;
	t_vector b;

	a = cub->rc.dir;
	b = cub->rc.pos;
	sprite->angle = acos((a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2))));;
}

void	sp_perpDist_calc(t_sprite *sprite)
{
	sprite->perpDist = sprite->dist * cos(sprite->angle);
}

void	sp_dim_calc(t_sprite *sprite)
{
	sprite->height = (int)((double)HEIGHT / sprite->perpDist);
	sprite->draw_start = (double)HEIGHT / 2.0 + - (double)sprite->height / 2.0;
	if(sprite->draw_start < 0)
		sprite->draw_start = 0;
	sprite->draw_end = sprite->height / 2.0 + HEIGHT / 2.0;
	if(sprite->draw_end >= HEIGHT)
		sprite->draw_end = HEIGHT - 1;
}

void	sp_dirDelta_calc(t_sprite *sprite)
{
	sprite->sp_dirDelta = sin(sprite->angle) * sprite->dist;
}

void	ray_dirDelta_calc(t_struct *cub, t_sprite *sprite)
{
	cub->rc.ray_dirDelta = cub->rc.camX * sprite->perpDist / 1.0;
}

void	vector_add(t_vector *dst, t_vector *to_add)
{
	dst->x += to_add->x;
	dst->y += to_add->y;
}

// Here we have to adapt our t_vector objects to fit the "is_left_side()" parameters.
int	both_sp_and_ray_same_side_of_dir(t_struct *cub, t_sprite *sprite)
{
	t_vector ray_coord;
	int sp_left;
	int ray_left;

	// Adjusting ray coordinates
	ray_coord = cub->rc.ray;
	vector_add(&ray_coord, &cub->rc.pos);
	// Checking whether second arg is left from first arg
	sp_left	= is_left_side(&cub->rc.dir, &sprite->pos);
	ray_left = is_left_side(&cub->rc.dir, &ray_coord);
	printf("sp_left		= %d\n", sp_left);
	printf("ray_left	= %d\n", ray_left);
	// Solving current func return
	if (sp_left == ray_left)
		return (1);
	return (0);
}

void	sp_rayDelta_calc(t_struct *cub, t_sprite *sprite)
{
	if (both_sp_and_ray_same_side_of_dir(cub, sprite))
		sprite->sp_rayDelta = fabs(fabs(cub->rc.ray_dirDelta) - fabs(sprite->sp_dirDelta));
	else
		sprite->sp_rayDelta = fabs(fabs(cub->rc.ray_dirDelta) + fabs(sprite->sp_dirDelta));
}

void	spriteX_calc(t_struct *cub, t_sprite *sprite)
{
	if (is_left_side(&cub->rc.ray, &sprite->pos))
		sprite->spriteX = (0.5 - sprite->sp_rayDelta) * 64;
	else
		sprite->spriteX = (0.5 + sprite->sp_rayDelta) * 64;
}

int main(void)
{
	t_struct cub;
	t_sprite sprite;

	cub.rc.pos.x = 3.5;
	cub.rc.pos.y = 3.5;

	sprite.pos.x = 10.5;
	sprite.pos.y = 10.5;

	cub.rc.dir.x = 1.0;
	cub.rc.dir.y = 0.0;
	cub.rc.dir.o.x = cub.rc.pos.x;
	cub.rc.dir.o.y = cub.rc.pos.y;
	rotation(&cub.rc.dir, PI / 4);
	printf("%lf | %lf\n", cub.rc.dir.x, cub.rc.dir.y);

	// PRE RAY CASTING

	cub.rc.plane.x = cub.rc.dir.x;
	cub.rc.plane.y = cub.rc.dir.y;
	rotation(&cub.rc.plane, PI / 2);

	sp_angle_calc(&cub, &sprite);
	printf("angle		= %lf\n", sprite.angle);

	sp_dist_calc(&cub, &sprite);
	printf("dist		= %lf\n", sprite.dist);

	sp_perpDist_calc(&sprite);
	printf("perpDist	= %lf\n", sprite.perpDist);

	sp_dim_calc(&sprite);
	printf("height		= %zu\n", sprite.height);

	sp_dirDelta_calc(&sprite);
	printf("sp_dirDelta	= %lf\n", sprite.sp_dirDelta);

	// RAY CASTING

	for (int i = 0; i < WIDTH; i++)
	{
		printf("----------------------------------\n");
		cam_calc(&cub, i);
		printf("camX		= %lf\n", cub.rc.camX);

		ray_dirDelta_calc(&cub, &sprite);
		printf("ray_dirDelta	= %lf\n", cub.rc.ray_dirDelta);

		sp_rayDelta_calc(&cub, &sprite);
		printf("sp_rayDelta	= %lf\n", sprite.sp_rayDelta);

		spriteX_calc(&cub, &sprite);
		printf("spriteX		= %lf\n", sprite.spriteX);
	}
	return (0);
}