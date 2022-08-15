#include "../libs/libft/libft.h"
#include "../libs/mlx/mlx.h"
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
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
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

void	ft_memdel(void *ptr)
{
	void **recup;

	recup = ptr;
	if (recup && *recup)
	{
		free(*recup);
		*recup = NULL;
	}
}

void	*ft_memdup(void *ptr, size_t size)
{
	void	*new;

	new = ft_calloc(1, sizeof(size));
	if (!new)
		return (NULL);
//	printf("%p | %p | %zu\n", new, ptr, size);
	*(char *)new = *(char *)ptr;
	return (new);
}

void	push_back(t_list *list, t_sprite *elem)
{
	t_sprite *iter;

	iter = list->first;
	while (iter && iter->next)
		iter = iter->next;
	if (iter)
	{
		iter->next = elem;
	}
	else
	{
		list->first = elem;
	}
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * 4);
	*(unsigned int*)dst = color;
}

unsigned int	get_color(t_data *data, int x, int y)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * 4);
	return (*(unsigned int *)dst);
}

void	clear_image(t_struct *cub)
{
	ft_bzero(cub->img.addr, HEIGHT * WIDTH * 4);
}

void	print_sprites_infos(t_list *sprites)
{
	t_sprite	*iter;
	size_t		i;

	iter = sprites->first;
	i = 0;
	while (iter)
	{
		printf("%zu : angle = %lf | dist = %lf | perpDist = %lf\n", i, iter->angle, iter->dist, iter->perpDist);
		iter = iter->next;
		i++;
	}
}

int	is_left_side(t_vector *dir, t_vector *pos)
{
	return ((pos->y - dir->o.y)*(dir->x) > (pos->x - dir->o.x)*(dir->y));
}

void	vector_add(t_vector *dst, t_vector *to_add)
{
	dst->x += to_add->x;
	dst->y += to_add->y;
}

void	draw_ceiling(t_struct *cub, size_t x)
{
	size_t	i;

	i = 0;
	while (i < (size_t)cub->rndr.drawStart)
	{
		my_mlx_pixel_put(&cub->img, x, i, 0x5050A9);
		i++;
	}
}

void	draw_wall(t_struct *cub, size_t x)
{
	size_t		i;
	t_vector	tex;
	double		ratio;
	int			lineHeight;

	lineHeight = cub->rndr.lineHeight;
	ratio = cub->rndr.ratio;
	ratio = (double)cub->tex.h / (double)cub->rndr.lineHeight;
	tex.x = cub->rndr.texX;
	tex.y = ((double)lineHeight - (double)HEIGHT) / 2.0 * ratio;
	tex.y *= (tex.y > 0);
	i = cub->rndr.drawStart;
	while (i < (size_t)cub->rndr.drawEnd)
	{
		tex.y += ratio;
		tex.y -= (tex.y > cub->tex.h - 1);
		my_mlx_pixel_put(&cub->img, x, i, get_color(&cub->tex, (int)tex.x, (int)tex.y));
		i++;
	}
}

void	draw_floor(t_struct *cub, size_t x)
{
	size_t	i;

	i = cub->rndr.drawEnd;
	while (i < HEIGHT)
	{
		my_mlx_pixel_put(&cub->img, x, i, 0x50A950);
		i++;
	}
}

void	texpos_calc(t_struct *cub)
{
	cub->rndr.texX = (cub->rndr.wallX * (double)cub->tex.w);
	if(cub->rc.side == 0 && cub->rc.ray.x > 0)
		cub->rndr.texX = cub->tex.w - cub->rndr.texX - 1;
	if(cub->rc.side == 1 && cub->rc.ray.y < 0)
		cub->rndr.texX = cub->tex.w - cub->rndr.texX - 1;
}

void	wallX_calc(t_struct *cub)
{
	if (cub->rc.side == 0)
		cub->rndr.wallX = cub->rc.pos.y + cub->rc.perpWallDist * cub->rc.ray.y;
	else
		cub->rndr.wallX = cub->rc.pos.x + cub->rc.perpWallDist * cub->rc.ray.x;
    cub->rndr.wallX -= floor((cub->rndr.wallX));
}
/*
int		is_sprite_int_fov(t_struct *cub, t_sprite *srite)
{
	if (cub->rc.camX < 0)
}*/

t_list	*get_sprites_to_draw(t_struct *cub)
{
	t_list		*list;
	t_sprite	*new_elem;
	t_sprite	*iter;

	(void)new_elem;
	(void)iter;
	new_elem = NULL;
	list = ft_calloc(1, sizeof(t_list));
	if (!list)
		return (NULL);
	iter = cub->sprites.first;
	while (iter)
	{
		if (iter->sp_rayDelta < (double)SPRITE_THEORETICAL_WIDTH / 2.0 && iter->perpDist > (double)MIN_SPRITE_DRAW_RANGE)
		{
			new_elem = (t_sprite *)malloc(sizeof(t_sprite));
			if (new_elem == NULL)
				return (list);
			*new_elem = *iter;
			new_elem->next = NULL;
			push_back(list, new_elem);
			list->size++;
		}
		iter = iter->next;
	}	
	return (list);
}

void	clear_list(t_list *list)
{
	t_sprite *iter;
	t_sprite *tmp;

	iter = list->first;
	while (iter)
	{
		tmp = iter;
		iter = tmp->next;
		ft_memdel(&tmp);
	}
	free(list);
}

void	draw_sprites(t_struct *cub, size_t x)
{
	t_list		*list;
	t_sprite	*iter;
	size_t		i;
	double		ratio;
	double		texY;

	list = get_sprites_to_draw(cub);
	if (!list)
		return ;
	iter = list->first;
	while (iter)
	{
		i = iter->draw_start;
		ratio = (double)iter->tex.w / (double)iter->height;
		texY = ((double)iter->height - (double)HEIGHT) / 2.0 * ratio;
		texY *= (texY > 0);
		while (i < HEIGHT && i < (size_t)iter->draw_end)
		{
			if (iter->spriteX > 0 && iter->spriteX < iter->tex.w)
			{
				my_mlx_pixel_put(&cub->img, x, i, get_color(&iter->tex, iter->spriteX, texY));
			}
			i++;
			texY += ratio;
			texY -= (texY > HEIGHT - 1);
		}
		iter = iter->next;
	}
	clear_list(list);
}

// Thanks to -->> https://alienryderflex.com/point_left_of_ray/ <<-- for the formula
/*
void	spriteX_calc(t_struct *cub)
{
	t_sprite	*iter;
	int left;

	iter = cub->sprites.first;
	while (iter)
	{
		left = (iter->pos.y - cub->rc.pos.y)*(cub->rc.ray.x) > (iter->pos.x - cub->rc.pos.x)*(cub->rc.ray.y);
		if (left)
			iter->spriteX = 0.5 * (double)iter->tex.w - iter->sp_rayDelta;
		else
			iter->spriteX = 0.5 * (double)iter->tex.w + iter->sp_rayDelta;
		printf("spriteX = %d | %lf\n", iter->spriteX, iter->sp_rayDelta);
		iter = iter->next;
	}
}
*/

void	draw_stripe(t_struct *cub, size_t x)
{
	wallX_calc(cub);
	texpos_calc(cub);
	draw_ceiling(cub, x);
	draw_wall(cub, x);
	draw_floor(cub, x);
	draw_sprites(cub, x);
}

void	rotation(t_vector *dir, double angle)
{
	double	old_x;

	old_x = dir->x;
	dir->x = old_x * cos(angle) - dir->y * sin(angle);
	dir->y = old_x * sin(angle) + dir->y * cos(angle);
}

void	translation(t_vector *pos, t_vector *dir)
{
	pos->x += dir->x * 0.08;
	pos->y += dir->y * 0.08;
}

void	pre_dda_calc(t_struct *cub)
{
	if (cub->rc.ray.x < 0)
	{
		cub->rc.step.x = -1;
		cub->rc.sideDist.x = (cub->rc.pos.x - (double)cub->rc.map.x) * cub->rc.deltaDist.x;
	}
	else
	{
		cub->rc.step.x = 1;
		cub->rc.sideDist.x = ((double)cub->rc.map.x + 1.0 - cub->rc.pos.x) * cub->rc.deltaDist.x;
	}
	if (cub->rc.ray.y < 0)
	{
		cub->rc.step.y = -1;
		cub->rc.sideDist.y = (cub->rc.pos.y - (double)cub->rc.map.y) * cub->rc.deltaDist.y;
	}
	else
	{
		cub->rc.step.y = 1;
		cub->rc.sideDist.y = ((double)cub->rc.map.y + 1.0 - cub->rc.pos.y) * cub->rc.deltaDist.y;
	}
}

void	dda(t_struct *cub)
{
	cub->rc.hit = 0;
	while (cub->rc.hit == 0)
	{
		//jump to next map square, either in x-direction, or in y-direction
		if (cub->rc.sideDist.x < cub->rc.sideDist.y)
		{
        	cub->rc.sideDist.x += cub->rc.deltaDist.x;
        	cub->rc.map.x += cub->rc.step.x;
			cub->rc.side = 0;
		}
		else
		{
			cub->rc.sideDist.y += cub->rc.deltaDist.y;
			cub->rc.map.y += cub->rc.step.y;
			cub->rc.side = 1;
		}
		if (map[cub->rc.map.y][cub->rc.map.x] != '0')
			cub->rc.hit = 1;
	}
}

void	perpWall_calc(t_struct *cub)
{
	if(cub->rc.side == 0)
		cub->rc.perpWallDist = (cub->rc.sideDist.x - cub->rc.deltaDist.x);
	else
		cub->rc.perpWallDist = (cub->rc.sideDist.y - cub->rc.deltaDist.y);
}

void	walldim_calc(t_struct *cub)
{
	cub->rndr.lineHeight = (int)((double)HEIGHT / cub->rc.perpWallDist);
	cub->rndr.drawStart = -cub->rndr.lineHeight / 2.0 + (double)HEIGHT / 2.0;
	if(cub->rndr.drawStart < 0)
		cub->rndr.drawStart = 0;
	cub->rndr.drawEnd = cub->rndr.lineHeight / 2.0 + HEIGHT / 2.0;
	if(cub->rndr.drawEnd >= HEIGHT)
		cub->rndr.drawEnd = HEIGHT - 1;
}

/*
void	spritedim_calc(t_sprite *sprite)
{
	sprite->height = (int)((double)HEIGHT / sprite->perpDist);
	printf("height = %zu | %lf\n", sprite->height, sprite->perpDist);
	sprite->draw_start = (double)HEIGHT / 2.0 + - (double)sprite->height / 2.0;
	if(sprite->draw_start < 0)
		sprite->draw_start = 0;
	sprite->draw_end = sprite->height / 2.0 + HEIGHT / 2.0;
	if(sprite->draw_end >= HEIGHT)
		sprite->draw_end = HEIGHT - 1;
}

void	ray_dirAngleCalc(t_struct *cub)
{
	cub->rc.ray_dirAngle = acos(1 / cub->rc.ray.dist);
}
*/

void	rc_calcs(t_struct *cub)
{
	cub->rc.map.x = (int)cub->rc.pos.x;
	cub->rc.map.y = (int)cub->rc.pos.y;
	cub->rc.deltaDist.x = (cub->rc.ray.x <= 0.00001 && cub->rc.ray.x >= -0.00001) ? 1e10 : fabs(1.0 / cub->rc.ray.x);
	cub->rc.deltaDist.y = (cub->rc.ray.y <= 0.00001 && cub->rc.ray.y >= -0.00001) ? 1e10 : fabs(1.0 / cub->rc.ray.y);
	pre_dda_calc(cub);
	dda(cub);
	perpWall_calc(cub);
	walldim_calc(cub);
}


int	is_valid_move(t_vector pos, t_vector dir)
{
	translation(&pos, &dir);
	if (map[(int)(pos.y + dir.y * 0.2)][(int)(pos.x + dir.x * 0.2)] == '0')
		return (1);
	return (0);
}

void	move(t_vector *pos, t_vector *dir, int move)
{
	t_vector move_dir;

	move_dir.x = dir->x;
	move_dir.y = dir->y;
	rotation(&move_dir, move * PI / 2);
	if (is_valid_move(*pos, move_dir))
		translation(pos, &move_dir);
}

void	apply_moves(t_struct *cub)
{
	if (cub->up)
		move(&cub->rc.pos, &cub->rc.dir, 0);
	if (cub->strafe_right)
		move(&cub->rc.pos, &cub->rc.dir, 1);
	if (cub->down)
		move(&cub->rc.pos, &cub->rc.dir, 2);
	if (cub->strafe_left)
		move(&cub->rc.pos, &cub->rc.dir, 3);
	if (cub->left)
		rotation(&cub->rc.dir, -PI/50);
	if (cub->right)
		rotation(&cub->rc.dir, PI/50);
}

void	plane_calc(t_struct *cub)
{
	cub->rc.plane.x = cub->rc.dir.x;
	cub->rc.plane.y = cub->rc.dir.y;
	rotation(&cub->rc.plane, PI / 2);
}

void	cam_calcs(t_struct *cub, size_t x)
{
	cub->rc.camX = 2.0 * (double)x / (double)(WIDTH) - 1.0; //x-coordinate in camera space
	cub->rc.ray.y = cub->rc.dir.y + cub->rc.plane.y * cub->rc.camX;
	cub->rc.ray.x = cub->rc.dir.x + cub->rc.plane.x * cub->rc.camX;
	cub->rc.ray.o.x = cub->rc.pos.x;
	cub->rc.ray.o.y = cub->rc.pos.y;
	cub->rc.ray.dist = sqrt(pow(cub->rc.ray.x, 2) + pow(cub->rc.ray.y, 2));
}

void	sp_dist_calc(t_struct *cub)
{
	t_sprite *iter;
	t_vector pos;

	iter = cub->sprites.first;
	while (iter)
	{
		pos = cub->rc.pos;
		iter->dist = sqrt(pow(iter->pos.x - pos.x, 2) + pow(iter->pos.y - pos.y, 2));
		iter = iter->next;
	}
}

void	sp_angle_calc(t_struct *cub)
{
	t_sprite *iter;
	t_vector a;
	t_vector b;

	a = cub->rc.dir;
	b = cub->rc.pos;
	iter = cub->sprites.first;
	while (iter)
	{
		iter->angle = acos((a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2))));;
		iter = iter->next;
	}
}

void	sp_perpDist_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		iter->perpDist = iter->dist * cos(iter->angle);
		iter = iter->next;
	}
}


void	sp_dim_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		iter->height = (int)((double)HEIGHT / iter->perpDist);
		iter->draw_start = (double)HEIGHT / 2.0 + - (double)iter->height / 2.0;
		if(iter->draw_start < 0)
			iter->draw_start = 0;
		iter->draw_end = iter->height / 2.0 + HEIGHT / 2.0;
		if(iter->draw_end >= HEIGHT)
			iter->draw_end = HEIGHT - 1;
		iter = iter->next;
	}
}

void	sp_dirDelta_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		iter->sp_dirDelta = sin(iter->angle) * iter->dist;
		iter = iter->next;
	}
}

void	sp_calc_debug(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		printf("-------------------------------\n");
		printf("sp_dist = %lf | sp_angle = %lf | sp_perpDist = %lf\n", iter->dist, iter->angle, iter->perpDist);
		printf("sp_dim = %zu\n", iter->height);
		printf("sp_dirDelta = %lf\n", iter->sp_dirDelta);
		iter = iter->next;
	}
}

void	sprite_rc_calcs_debug(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	// while (iter)
	// {
		// printf("------------------------------\n");
		// printf("sp_dist = %lf | sp_angle = %lf | sp_perpDist = %lf\n", iter->dist, iter->angle, iter->perpDist);
		// printf("sp_dim = %zu\n", iter->height);
		// printf("sp_dirDelta = %lf\n", iter->sp_dirDelta);
		// printf("ray_dirDelta = %lf\n", cub->rc.ray_dirDelta);
		// printf("sp_rayDelta = %lf\n", iter->sp_rayDelta);
		// printf("spriteX = %lf\n", iter->spriteX);
		// iter = iter->next;
	// }

	if (iter)
	{
		printf("------------------------------\n");
		printf("sp_dist = %lf | sp_angle = %lf | sp_perpDist = %lf\n", iter->dist, iter->angle, iter->perpDist);
		printf("sp_dim = %zu\n", iter->height);
		printf("sp_dirDelta = %lf\n", iter->sp_dirDelta);
		printf("ray_dirDelta = %lf\n", cub->rc.ray_dirDelta);
		printf("sp_rayDelta = %lf\n", iter->sp_rayDelta);
		printf("is_left = %d\n", is_left_side(&cub->rc.ray, &iter->pos));
		printf("spriteX = %lf\n", iter->spriteX);
		printf("ray.x = %lf | ray.y = %lf\n", cub->rc.ray.x, cub->rc.ray.y);
		printf("iter->pos.x = %lf | iter->pos.y = %lf\n", iter->pos.x, iter->pos.y);
	}
}

void	sprite_calcs(t_struct *cub)
{
	sp_dist_calc(cub);
	sp_angle_calc(cub);
	sp_perpDist_calc(cub);
	sp_dim_calc(cub);
	sp_dirDelta_calc(cub);

//	sp_calc_debug(cub);
}

void	ray_dirDelta_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		cub->rc.ray_dirDelta = cub->rc.camX * iter->perpDist / 1.0;
		iter = iter->next;
	}
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

	// Solving current func return
	if (sp_left == ray_left)
		return (1);
	return (0);
}

void	sp_rayDelta_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		if (both_sp_and_ray_same_side_of_dir(cub, iter))
			iter->sp_rayDelta = fabs(fabs(cub->rc.ray_dirDelta) - fabs(iter->sp_dirDelta));
		else
			iter->sp_rayDelta = fabs(fabs(cub->rc.ray_dirDelta) + fabs(iter->sp_dirDelta));
		iter = iter->next;
	}
}

void	spriteX_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		if (is_left_side(&cub->rc.ray, &iter->pos))
			iter->spriteX = (0.5 - iter->sp_rayDelta) * 64;
		else
			iter->spriteX = (0.5 + iter->sp_rayDelta) * 64;
		iter = iter->next;
	}
}

void	sprite_rc_calcs(t_struct *cub)
{
	ray_dirDelta_calc(cub);
	sp_rayDelta_calc(cub);
	spriteX_calc(cub);
}

void	ray_casting(t_struct *cub)
{
	size_t	i;

	plane_calc(cub);
//	sp_delta_dir_calc(cub);
	i = 0;
	while (i < WIDTH)
	{
		//printf("posx = %lf | posy = %lf | dirx = %lf | diry = %lf | plnx = %lf | plny = %lf\n", cub->rc.pos.x, cub->rc.pos.y, cub->rc.dir.x, cub->rc.dir.y, cub->rc.plane.x, cub->rc.plane.y);
		cam_calcs(cub, i);
		rc_calcs(cub);
		sprite_rc_calcs(cub);
//		sprite_rc_calcs_debug(cub);
		// sp_ray_dist_calc(cub);
		draw_stripe(cub, i);
		i++;
	}
//	printf("-------------------\n");
	// CHECK CAMPLANE VALUES
}

/*
t_sprite	*get_next_farthest(t_struct *cub, t_list *sprites)
{
	t_sprite	*farthest;
	t_sprite	*iter;

	iter = sprites->first;

}

void	make_dist_list(t_struct *cub, t_list *sprites)
{
	t_sprite	*iter;
	t_sprite	*farthest;
	size_t		dist;

	dist = ~0;
	iter = sprites->first;
	while (iter)
	{
		farthest = get_farthest(cub, sprites);
		if (!cub->dist_list.first)
		{
			farthest = iter;

		}
		iter = iter->next;
	}
}*/

void	print_list(t_list *list)
{
	t_sprite *iter;

	iter = list->first;
	while (iter)
	{
		printf("iter.pos.x = %lf | iter.pos.y = %lf\n", iter->pos.x, iter->pos.y);
		iter = iter->next;
	}
}

void	remove_sprite(t_list *list, t_sprite *elem)
{
	t_sprite *iter;
	t_sprite *tmp;

	iter = list->first;
	if (list->first == elem)
	{
		tmp = list->first->next;
		list->first->next = NULL;
		list->first = tmp;
	}
	else
	{
		while (iter && iter->next)
		{
			if (iter->next == elem)
				iter->next = iter->next->next;
			iter = iter->next;
		}
	}
}

void	transfer_farthest(t_list *src, t_list *dst)
{
	t_sprite *farthest;
	t_sprite *iter;

	farthest = src->first;
	iter = src->first;
	while (iter)
	{
		if (farthest->dist < iter->dist)
			farthest = iter;
		iter = iter->next;
	}
	push_back(dst, farthest);
	remove_sprite(src, farthest);
}

void	sort_sprites(t_struct *cub)
{
	while (cub->sprites.first)
	{
		transfer_farthest(&cub->sprites, &cub->sorted_sprites);
	}
	cub->sprites.first = cub->sorted_sprites.first;
	cub->sorted_sprites.first = NULL;
}

/*
void	update_sprites(t_struct *cub)
{
	t_sprite *iter;
	t_vector *pos;
	t_vector a;
	t_vector b;

	iter = cub->sprites.first;
	a.x = cub->rc.dir.x;
	a.y = cub->rc.dir.y;
	pos = &cub->rc.pos;
	while (iter)
	{
		b.x = iter->pos.x - cub->rc.pos.x;
		b.y = iter->pos.y - cub->rc.pos.y;
		iter->dist = sqrt(pow(iter->pos.x - pos->x, 2) + pow(iter->pos.y - pos->y, 2));
		iter->angle = acos((a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2))));;
		iter->perpDist = iter->dist * cos(iter->angle);
		spritedim_calc(iter);
		iter->sp_dirDelta = sin(iter->angle) * iter->dist;
		iter = iter->next;
	}
}
*/

// cos(angle) = dist -->> perpDist = cos(angle) * dist;
void	perpSprite_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		iter = iter->next;
	}
}

int	render_frame(t_struct *cub)
{
	apply_moves(cub);
	sprite_calcs(cub);
//	update_sprites(cub);
//	print_sprites_infos(&cub->sprites);
	sort_sprites(cub);
	clear_image(cub);
	ray_casting(cub);
//	draw_sprites(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.img, 0, 0);
	return (0);
}

void	init_vars(t_struct *cub)
{
	ft_bzero(cub, sizeof(t_struct));
	cub->rc.pos.x = 3.5;
	cub->rc.pos.y = 3.5;
	cub->rc.dir.x = 1;
	cub->rc.dir.y = 0;
	rotation(&cub->rc.dir, PI / 4);
	cub->rc.dir.o.x = cub->rc.pos.x;
	cub->rc.dir.o.y = cub->rc.pos.y;
}

int	destroy_event(t_struct *cub)
{
	mlx_loop_end(cub->mlx);
	return (0);
}

int	keypress(int key, t_struct *cub)
{
	if (key == 119)
		cub->up = 1;
	if (key == 97)
		cub->strafe_left = 1;
	if (key == 115)
		cub->down = 1;
	if (key == 100)
		cub->strafe_right = 1;
	if (key == 65361)
		cub->left = 1;
	if (key == 65363)
		cub->right = 1;
	return (0);
}

int	keyrelease(int key, t_struct *cub)
{
	if (key == 65307)
		destroy_event(cub);
	if (key == 119)
		cub->up = 0;
	if (key == 97)
		cub->strafe_left = 0;
	if (key == 115)
		cub->down = 0;
	if (key == 100)
		cub->strafe_right = 0;
	if (key == 65361)
		cub->left = 0;
	if (key == 65363)
		cub->right = 0;
	return (0);
}

int init_sprite_list(t_list *sprites, size_t x, size_t y)
{
	sprites->first = ft_calloc(1, sizeof(t_sprite));
	sprites->size++;
	sprites->first->pos.x = x + 0.5;
	sprites->first->pos.y = y + 0.5;
	return (0);
}

int	pushback_sprite(t_list *sprites, size_t x, size_t y)
{
	t_sprite *iter;

	iter = sprites->first;
	while (iter->next)
		iter = iter->next;
	iter->next = ft_calloc(1, sizeof(t_sprite));
	sprites->size++;
	iter->next->pos.x = x + 0.5;
	iter->next->pos.y = y + 0.5;
	return (0);
}

int	add_sprite(t_list *sprites, size_t x, size_t y)
{
	if (sprites->size == 0)
		init_sprite_list(sprites, x, y);
	else
		pushback_sprite(sprites, x, y);
	return (0);
}

t_sprite	*last_sprite(t_list *list)
{
	t_sprite *iter;

	iter = list->first;
	while (iter->next)
	{
		iter = iter->next;
	}
	return (iter);
}

void	set_sprites(t_struct *cub)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < 20)
	{
		j = 0;
		while (j < 20)
		{
			if (map[i][j] == 'S')
			{
				add_sprite(&cub->sprites, j, i);
				last_sprite(&cub->sprites)->type = map[i][j];
				map[i][j] = '0';
			}
			j++;
		}
		i++;
	}
}


void	init_sprites(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		// iter->tex.img = cub->tex.img;
		// iter->tex.addr = cub->tex.addr;
		iter->tex = cub->tex;
//		printf("POST_SPRITE_INIT : %p | %p\n", iter->tex.img, iter->tex.addr);
		iter = iter->next;
	}
}

void	destroy_sprites(t_struct *cub)
{
	t_sprite *iter;
	t_sprite *tmp;

	iter = cub->sprites.first;
	while (iter)
	{
		tmp = iter->next;
//		mlx_destroy_image(cub->mlx, iter->tex.img);
		ft_memdel(&iter);
		iter = tmp;
	}
}

int main(void)
{
	t_struct cub;

	// INIT MY STRUCT
	init_vars(&cub);
	set_sprites(&cub);
	// t_sprite *coucou = ft_memdup((void *)cub.sprites.first, sizeof(t_sprite));
	// free(coucou);
	// destroy_sprites(&cub);
	// return (0);
//	print_sprites_infos(&cub.sprites);

	// INIT MLX
	cub.mlx = mlx_init();
	cub.win = mlx_new_window(cub.mlx, WIDTH, HEIGHT, "mlx_test");
	cub.img.img = mlx_new_image(cub.mlx, WIDTH, HEIGHT);
	cub.img.addr = mlx_get_data_addr(cub.img.img, &cub.img.bits_per_pixel, &cub.img.line_length, &cub.img.endian);
	cub.tex.img = mlx_xpm_file_to_image(cub.mlx, "./textures/wall.xpm", &cub.tex.w, &cub.tex.h);
	cub.tex.addr = mlx_get_data_addr(cub.tex.img, &cub.tex.bits_per_pixel, &cub.tex.line_length, &cub.tex.endian);
	init_sprites(&cub);

	// SETUP HOOKS
	mlx_hook(cub.win, 2, 1L<<0, keypress, &cub);
	mlx_hook(cub.win, 3, 1L<<1, keyrelease, &cub);
	mlx_hook(cub.win, 17, 1L<<17, destroy_event, &cub);
	mlx_loop_hook(cub.mlx, render_frame, &cub);

	// LET'S START
	mlx_loop(cub.mlx);

	// CLEAR MLX;
	destroy_sprites(&cub);
	mlx_destroy_image(cub.mlx, cub.tex.img);
	mlx_destroy_image(cub.mlx, cub.img.img);
	mlx_destroy_window(cub.mlx, cub.win);
	mlx_destroy_display(cub.mlx);
	free(cub.mlx);
	return (0);
}
