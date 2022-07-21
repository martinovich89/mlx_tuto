#include "../libs/mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 50

typedef struct s_pos
{
	int x;
	int y;
}	t_pos;

typedef struct s_data
{
	void *img;
	char *addr;
	int bpp;
	int size_line;
	int endian;
	int width;
	int height;
}	t_data;

typedef struct s_vars
{
	void *mlx;
	void *win;
	t_data image;
	t_data tex;
	t_pos pos;
}	t_vars;

void	put_color(t_data *image, int x, int y, unsigned int color)
{
		char *dst;

		dst = image->addr + y * image->size_line + x * image->bpp / 8;
		*(unsigned int *)dst = color;
}

unsigned int get_color(t_data *image, int x, int y)
{
	char *dst;

	dst = image->addr + y * image->size_line + x * 4;
	return (*(unsigned int *)dst);
}

void	draw_background(t_data *image)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			put_color(image, j, i, 0x2020a9);
			j++;
		}
		i++;
	}
	
}

void	draw_sprite(t_data *image, t_data *sprite, int x, int y)
{
	size_t i;
	size_t j;
	unsigned int color;
	double	ratio;

	ratio = (double)sprite->width / (double)CELL_SIZE;
	i = 0 ;
	while (i < CELL_SIZE)
	{
		j = 0;
		while (j < CELL_SIZE)
		{
			color = get_color(sprite, (double)j * ratio, (double)i * ratio);
			if (color != 0x0)
				put_color(image, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

void	clear_window(t_vars *vars)
{
	bzero(vars->image.addr, HEIGHT * WIDTH * 4);
}

int	render_frame(t_vars *vars)
{
	clear_window(vars);
	//draw_background(&vars->image);
	draw_sprite(&vars->image, &vars->tex, vars->pos.x, vars->pos.y);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->image.img, 0, 0);
	return (0);
}

void	move(t_vars *vars, int dir)
{
	if (dir == 0 && vars->pos.y - CELL_SIZE >= 0)
		vars->pos.y -= CELL_SIZE;
	if (dir == 1 && vars->pos.x - CELL_SIZE >= 0)
		vars->pos.x -= CELL_SIZE;
	if (dir == 2 && vars->pos.y + CELL_SIZE <= HEIGHT)
		vars->pos.y += CELL_SIZE;
	if (dir == 3 && vars->pos.x + CELL_SIZE <= WIDTH)
		vars->pos.x += CELL_SIZE;
}

int keypress(int key, t_vars *vars)
{
	(void)vars;
	printf("key = %d\n", key);
	if (key == 65307)
		mlx_loop_end(vars->mlx);
	if (key == 119)
		move(vars, 0);
	if (key == 97)
		move(vars, 1);
	if (key == 115)
		move(vars, 2);
	if (key == 100)
		move(vars, 3);

	return (0);
}

int	destroy(t_vars *vars)
{
	mlx_loop_end(vars->mlx);
	return (0);
}

int main(void)
{
	t_vars vars;

	vars.pos.x = 0;
	vars.pos.y = 0;
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, WIDTH, HEIGHT, "endian");
	vars.image.img = mlx_new_image(vars.mlx, WIDTH, HEIGHT);
	vars.image.addr = mlx_get_data_addr(vars.image.img, &vars.image.bpp, &vars.image.size_line, &vars.image.endian);
	vars.tex.img = mlx_xpm_file_to_image(vars.mlx, "./textures/toto.xpm", &vars.tex.width, &vars.tex.height);
	vars.tex.addr = mlx_get_data_addr(vars.tex.img, &vars.tex.bpp, &vars.tex.size_line, &vars.tex.endian);


	mlx_hook(vars.win, 2, 1L<<0, keypress, &vars);
	mlx_hook(vars.win, 17, 1L<<17, destroy, &vars);
	mlx_loop_hook(vars.mlx, render_frame, &vars);
	mlx_loop(vars.mlx);

	mlx_destroy_image(vars.mlx, vars.tex.img);
	mlx_destroy_image(vars.mlx, vars.image.img);
	mlx_destroy_window(vars.mlx, vars.win);
	mlx_destroy_display(vars.mlx);
	free(vars.mlx);
	return (0);
}