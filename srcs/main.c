#include "../libs/libft/libft.h"
#include "../libs/mlx/mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <stdbool.h>

#define WIDTH 1920
#define HEIGHT 1080

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

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

typedef struct	s_intpos
{
	int	x;
	int	y;
}				t_intpos;

typedef struct	s_vector
{
	double	x;
	double	y;
}				t_vector;

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
}				t_rc;

typedef struct	s_rndr
{
	int	lineHeight;
	int	drawStart;
	int	drawEnd;
	double	wallX;
	double	texX;
	double	texY;
	double	ratio;
}				t_rndr;

typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		w;
	int		h;
}				t_data;

typedef struct	s_sprite
{
	t_data			tex;
	t_vector		pos;
	double			dist;
	struct s_sprite	*next;
}				t_sprite;

typedef struct	s_list
{
	t_sprite	*first;
	size_t		size;
}				t_list;

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
}				t_struct;

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

void	draw_background(t_struct *cub)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			my_mlx_pixel_put(&cub->img, j, i, cub->background);
			j++;
		}
		i++;
	}

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

void	draw_stripe(t_struct *cub, size_t x)
{
	wallX_calc(cub);
	texpos_calc(cub);
	draw_ceiling(cub, x);
	draw_wall(cub, x);
	draw_floor(cub, x);

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
	// PRINT ALL VARS HERE
	if (cub->rc.camX > -0.3 && cub->rc.camX < 0.3)
	{
//		printf("camx = %lf | rayx = %lf | rayy = %lf | mapx = %d | mapy = %d | sideDistX = %lf | sideDistY = %lf | delaDistX = %lf | deltaDistY = %lf | perpWallDist = %lf | stepx = %d | stepy = %d | hit = %d | side = %d\n", cub->rc.camX, cub->rc.ray.x, cub->rc.ray.y, cub->rc.map.x, cub->rc.map.y, cub->rc.sideDist.x, cub->rc.sideDist.y, cub->rc.deltaDist.x, cub->rc.deltaDist.y, cub->rc.perpWallDist, cub->rc.step.x, cub->rc.step.y, cub->rc.hit, cub->rc.side);
//		printf("lineheight = %d | drawStart = %d | drawEnd = %d\n", cub->rndr.lineHeight, cub->rndr.drawStart, cub->rndr.drawEnd);
	}
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

void	cam_calc(t_struct *cub, size_t x)
{
	cub->rc.camX = 2.0 * (double)x / (double)(WIDTH) - 1.0; //x-coordinate in camera space
	cub->rc.ray.y = cub->rc.dir.y + cub->rc.plane.y * cub->rc.camX;
	cub->rc.ray.x = cub->rc.dir.x + cub->rc.plane.x * cub->rc.camX;
}

void	ray_casting(t_struct *cub)
{
	size_t	i;

	plane_calc(cub);
	i = 0;
	while (i < WIDTH)
	{
		//printf("posx = %lf | posy = %lf | dirx = %lf | diry = %lf | plnx = %lf | plny = %lf\n", cub->rc.pos.x, cub->rc.pos.y, cub->rc.dir.x, cub->rc.dir.y, cub->rc.plane.x, cub->rc.plane.y);
		cam_calc(cub, i);
		rc_calcs(cub);
		draw_stripe(cub, i);
		i++;
	}
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

void	push_back(t_list *list, t_sprite *elem)
{
	t_sprite *iter;

	iter = list->first;
	while (iter && iter->next)
		iter = iter->next;
	if (list->first)
		iter->next = elem;
	else
		list->first = elem;
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

void	update_sprites(t_struct *cub)
{
	t_sprite *iter;
	t_vector *pos;

	iter = cub->sprites.first;
	pos = &cub->rc.pos;
	while (iter)
	{
		iter->dist = sqrt(pow(iter->pos.x - pos->x, 2) + pow(iter->pos.y - pos->y, 2));
		iter = iter->next;
	}
}
/*
void	perpSprites_calc(t_struct *cub)
{
	t_sprite *iter;

	iter = cub->sprites.first;
	while (iter)
	{
		iter->dist
	}
}*/

void	draw_sprites(t_struct *cub)
{
	update_sprites(cub);
	sort_sprites(cub);
//	perpSprite_calc(cub);
}

int	render_frame(t_struct *cub)
{
	apply_moves(cub);
	clear_image(cub);
	ray_casting(cub);
	draw_sprites(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.img, 0, 0);
	return (0);
}

void	init_vars(t_struct *cub)
{
	ft_bzero(cub, sizeof(t_struct));
	cub->rc.pos.x = 3.5;
	cub->rc.pos.y = 3.5;
	cub->rc.dir.y = -1;
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
	sprites->size++;
	return (0);
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
				map[i][j] = '0';
			}
			j++;
		}
		i++;
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
		ft_memdel(&iter);
		iter = tmp;
	}
}

void	print_sprites_infos(t_list *sprites)
{
	t_sprite	*iter;
	size_t		i;

	iter = sprites->first;
	i = 0;
	while (iter)
	{
		iter = iter->next;
		i++;
	}
}

int main(void)
{
	t_struct cub;

	// INIT MY STRUCT
	init_vars(&cub);
	set_sprites(&cub);
	print_sprites_infos(&cub.sprites);

	// INIT MLX
	cub.mlx = mlx_init();
	cub.win = mlx_new_window(cub.mlx, WIDTH, HEIGHT, "mlx_test");
	cub.img.img = mlx_new_image(cub.mlx, WIDTH, HEIGHT);
	cub.img.addr = mlx_get_data_addr(cub.img.img, &cub.img.bits_per_pixel, &cub.img.line_length, &cub.img.endian);
	cub.tex.img = mlx_xpm_file_to_image(cub.mlx, "./textures/wall.xpm", &cub.tex.w, &cub.tex.h);
	cub.tex.addr = mlx_get_data_addr(cub.tex.img, &cub.tex.bits_per_pixel, &cub.tex.line_length, &cub.tex.endian);

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
