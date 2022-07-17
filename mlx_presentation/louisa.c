#include "../libs/mlx/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define WIDTH 800
#define HEIGHT 600

#define SLOT_WIDTH 100
#define SLOT_HEIGHT 100

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_cub
{
	void *mlx;
	void *win;
	t_data img;

	t_data tex;
	int	width;
	int	height;
}	t_cub;

void	clear_project(t_cub *cub)
{
	mlx_destroy_image(cub->mlx, cub->tex.img);
	mlx_destroy_image(cub->mlx, cub->img.img);
	mlx_destroy_window(cub->mlx, cub->win);
	mlx_destroy_display(cub->mlx);
	free(cub->mlx);
}

unsigned int	get_color(t_data *data, int x, int y)
{
	unsigned char	*dst;
	unsigned int	color;
//	unsigned int	color1;

	dst = (unsigned char *)(data->addr) + (y * data->line_length + x * 4);
//	color1 = (unsigned int)(dst[0]<<0) | (unsigned int)(dst[1]<<8) | (unsigned int)(dst[2]<<16) | (unsigned int)(dst[3]<<24);
	color = (dst[0]<<0) + (dst[1]<<8) + (dst[2]<<16) + (dst[3]<<24);
	return (color);
}

void	put_color(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * 4);
	*(unsigned int*)dst = color;
}

void	draw_background(t_cub *cub)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			put_color(&cub->img, j, i, 0x00FF0000);
			j++;
		}
		i++;
	}
}

void	draw_tex(t_cub *cub, int x_offset, int y_offset)
{
	size_t	i;
	size_t	j;
	int	color;
	float	x_ratio;
	float	y_ratio;

	x_ratio = cub->width / (float)SLOT_WIDTH;
	y_ratio = cub->height / (float)SLOT_HEIGHT;
	i = 0;
	while (i < (size_t)SLOT_WIDTH)
	{
		j = 0;
		while (j < (size_t)SLOT_HEIGHT)
		{
			color = get_color(&cub->tex, j * x_ratio, i * y_ratio);
			if (color != 0x0)
				put_color(&cub->img, j + x_offset, i + y_offset, color);
			j++;
		}
		i++;
	}
}

int	destroy_event(t_cub *cub)
{
	printf("trolol\n");
	clear_project(cub);
	exit(0);
}

int keypress_event(int key, t_cub *cub)
{
	(void)cub;
	printf("key = %d\n", key);
	if (key == 65307)
		destroy_event(cub);
	return (0);
}

int	render(t_cub *cub)
{
	draw_background(cub);
	draw_tex(cub, WIDTH / 2 - SLOT_WIDTH / 2, HEIGHT / 2 - SLOT_HEIGHT / 2);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.img, 0, 0);
	return (0);
}

int main(void)
{
	t_cub cub;

	cub.mlx = mlx_init();
	cub.win = mlx_new_window(cub.mlx, WIDTH, HEIGHT, "louisa");
	cub.img.img = mlx_new_image(cub.mlx, WIDTH, HEIGHT);
	cub.img.addr = mlx_get_data_addr(cub.img.img, &cub.img.bpp, &cub.img.line_length, &cub.img.endian);

	cub.tex.img = mlx_xpm_file_to_image(cub.mlx, "textures/toto.xpm", &cub.width, &cub.height);
	cub.tex.addr = mlx_get_data_addr(cub.tex.img, &cub.tex.bpp, &cub.tex.line_length, &cub.tex.endian);

	mlx_hook(cub.win, 2, 1L<<0, keypress_event, &cub);
	mlx_hook(cub.win, 17, 1L<<17, destroy_event, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_loop(cub.mlx);

	printf("test\n");
	clear_project(&cub);
	return (0);
}

/*
int main(void)
{
	int i = 4;
	char c;

	c = (char)i;
	c = *(char *)&i;
}*/