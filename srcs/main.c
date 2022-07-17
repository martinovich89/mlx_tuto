#include "../libs/libft/libft.h"
#include "../libs/mlx/mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct	s_struct
{
	void		*mlx;
	void		*win;
	t_data		img;
	int			background;
}				t_struct;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	clear_image(t_struct *cub)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			//printf("%zu | %zu\n", i, j);
			my_mlx_pixel_put(&cub->img, j, i, 0x0);
			j++;
		}
		i++;
	}
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

int	render_frame(t_struct *cub)
{
	clear_image(cub);
//	update_background_color();
	draw_background(cub);
//	draw_stuff(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.img, 0, 0);
	return (0);
}

int main(void)
{
	t_struct cub;

	cub.background = 0x00802020;
	cub.mlx = mlx_init();
	cub.win = mlx_new_window(cub.mlx, WIDTH, HEIGHT, "mlx_test");
	cub.img.img = mlx_new_image(cub.mlx, WIDTH, HEIGHT);
	cub.img.addr = mlx_get_data_addr(cub.img.img, &cub.img.bits_per_pixel, &cub.img.line_length, &cub.img.endian);
	//printf("%p %p\n",cub.img.addr, cub.img.img);

	mlx_loop_hook(cub.mlx, render_frame, &cub);
	mlx_loop(cub.mlx);

	mlx_destroy_image(cub.mlx, cub.img.img);
	mlx_destroy_window(cub.mlx, cub.win);
	mlx_destroy_display(cub.mlx);
	free(cub.mlx);
	return (0);
}
