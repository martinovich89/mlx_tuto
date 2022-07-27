#include "../libs/mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define WIDTH 1000
#define HEIGHT 1000
#define CELL_SIZE 100
#define BIG_CELL_SIZE 1000
#define MAP_WIDTH 10
#define MAP_HEIGHT 10

typedef struct s_data
{
	void *img;
	char *addr;
	int bpp;
	int line_length;
	int endian;
	int width;
	int height;
}	t_data;


typedef struct s_vars
{
	void *mlx;
	void *win;
	t_data img;
	t_data tex;
}	t_vars;

char map[HEIGHT][WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void	put_color(t_data *data, int x, int y, int color)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * 4);
	*(unsigned int *)dst = color;
}

unsigned int	get_color(t_data *data, int x, int y)
{
	char *dst;

	dst = data->addr + (y * data->line_length + x * 4);
	return (*(unsigned int *)dst);
}

void	draw_background(t_vars *vars)
{
	size_t i;
	size_t j;

	i = 0;
	while ( i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			put_color(&vars->img, j, i, 0x2020a9);
			j++;
		}
		i++;
	}
}

void	draw_tex(t_vars *vars, int x, int y)
{
	size_t i;
	size_t j;
	double ratio;
	int color;

	printf("%d | %d\n", x, y);
	ratio = (double)vars->tex.width / (double)CELL_SIZE;
	i = 0;
	while (i < CELL_SIZE)
	{
		j = 0;
		while (j < CELL_SIZE)
		{
			color = get_color(&vars->tex, j * ratio, i * ratio);
			if (color != 0x0)
				put_color(&vars->img, j + x, i + y, color);
			j++;
		}
		i++;
	}
}

void	draw_big_tex(t_vars *vars)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < BIG_CELL_SIZE)
	{
		j = 0;
		while (j < BIG_CELL_SIZE)
		{
			
			j++;
		}
		i++;
	}
}

void	draw_map(t_vars *vars)
{
	size_t i;
	size_t j;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (map[i][j] == 1)
				draw_tex(vars, j * CELL_SIZE, i * CELL_SIZE);
			j++;
		}
		i++;
	}
}


int	render_frame(t_vars *vars)
{
	draw_background(vars);
//	draw_map(vars);
	draw_big_tex(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
	return (0);
}

int	destroy_event(t_vars *vars)
{
	mlx_loop_end(vars->mlx);
	return (0);
}

int keypress_event(int key, t_vars *vars)
{
	if (key == 65307)
		mlx_loop_end(vars->mlx);
	return (0);
}

int main(void)
{
	t_vars vars;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, WIDTH, HEIGHT, "test");
	vars.img.img = mlx_new_image(vars.mlx, WIDTH, HEIGHT);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bpp, &vars.img.line_length, &vars.img.endian);
	vars.tex.img = mlx_xpm_file_to_image(vars.mlx, "./textures/wall.xpm", &vars.tex.width, &vars.tex.height);
	vars.tex.addr = mlx_get_data_addr(vars.tex.img, &vars.tex.bpp, &vars.tex.line_length, &vars.tex.endian);

	mlx_hook(vars.win, 2, 1L<<0, keypress_event, &vars);
	mlx_hook(vars.win, 17, 1L<<17, destroy_event, &vars);
	mlx_loop_hook(vars.mlx, render_frame, &vars);
	mlx_loop(vars.mlx);

	mlx_destroy_image(vars.mlx, vars.tex.img);
	mlx_destroy_image(vars.mlx, vars.img.img);
	mlx_destroy_window(vars.mlx, vars.win);
	mlx_destroy_display(vars.mlx);
	free(vars.mlx);
	return (0);
}