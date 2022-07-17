#include "../libs/mlx/mlx.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#define WIDTH 600
#define HEIGHT 600
#define CELL_SIZE 100

typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_data;

typedef struct s_toinou
{
	void	*mlx;
	void	*win;
	t_data	img;
	t_data	tex;
	int		x_offset;
	int		y_offset;
}	t_toinou;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * 4);
	*(unsigned int*)dst = color;
}

unsigned int	get_pix_color(t_data *data, int x, int y)
{
	char			*dst;
	unsigned int	color;

	dst = data->addr + (y * data->line_length + x * 4);
	color = *(unsigned int*)dst;
	return (color);
}

void	clear_project(t_toinou *cub)
{
	mlx_destroy_image(cub->mlx, cub->tex.img);
	mlx_destroy_image(cub->mlx, cub->img.img);
	mlx_destroy_window(cub->mlx, cub->win);
	mlx_destroy_display(cub->mlx);
	mlx_loop_end(cub->mlx);
	free(cub->mlx);
	exit(0);
}

int	destroy_event(t_toinou *cub)
{
	clear_project(cub);
	return (0);
}

int	keypress_event(int key, t_toinou *cub)
{
	if (key == 65307)
		destroy_event(cub);
	if (key == 97)
	{
		printf("hello\n");
		cub->x_offset -= CELL_SIZE;
	}
	if (key == 100)
		cub->x_offset += CELL_SIZE;
	if (key == 119)
		cub->y_offset -= CELL_SIZE;
	if (key == 115)
		cub->y_offset += CELL_SIZE;
}

int clear_image(t_toinou *cub)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			my_mlx_pixel_put(&cub->img, j, i, 0x00000000);
			j++;
		}
		i++;
	}
}

int	render(t_toinou *cub)
{
	int i = 0;
	int j;

	float x_ratio = (float)cub->tex.width / (float)CELL_SIZE;
	float y_ratio = (float)cub->tex.height / (float)CELL_SIZE;

	clear_image(cub);
	while (i < CELL_SIZE)
	{
		j = 0;
		while (j < CELL_SIZE)
		{
			my_mlx_pixel_put(&cub->img, j + cub->x_offset, i + cub->y_offset, get_pix_color(&cub->tex, (int)(j * x_ratio), (int)(i * y_ratio)));
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.img, 0, 0);
	return (0);
}

int main(void)
{
	t_toinou cub;
	
	bzero(&cub, sizeof(t_toinou));
	cub.mlx = mlx_init();
	cub.win = mlx_new_window(cub.mlx, WIDTH, HEIGHT, "toinou");
	cub.img.img = mlx_new_image(cub.mlx, WIDTH, HEIGHT);
	cub.img.addr = mlx_get_data_addr(cub.img.img, &cub.img.bpp, &cub.img.line_length, &cub.img.endian);

	cub.tex.img = mlx_xpm_file_to_image(cub.mlx, "textures/toto.xpm", &cub.tex.width, &cub.tex.height);
	if (!cub.tex.img)
	{
		printf("LOL\n");
		return (1);
	}
	cub.tex.addr = mlx_get_data_addr(cub.tex.img, &cub.tex.bpp, &cub.tex.line_length, &cub.tex.endian);

	mlx_hook(cub.win, 2, 1L<<0, keypress_event, &cub);
	mlx_hook(cub.win, 17, (1L<<17), destroy_event, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_loop(cub.mlx);

	clear_project(&cub);
	return (0);
}