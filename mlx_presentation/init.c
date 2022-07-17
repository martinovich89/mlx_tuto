#include "../libs/mlx/mlx.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_struct
{
	void	*mlx;
	void	*win;
	t_data	img;
}	t_struct;

void	put_pixel_to_image(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp / 8));
	*(unsigned int*)dst = color;
}

int	draw_image(t_struct *cub)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			put_pixel_to_image(&cub->img, j, i, 0X000000FF);
			j++;
		}
		i++;
	}
	return (0);
}

int main(void)
{
	t_struct cub;

	cub.mlx = mlx_init();
	cub.win = mlx_new_window(cub.mlx, WIDTH, HEIGHT, "mlx_test");
	cub.img.img = mlx_new_image(cub.mlx, WIDTH, HEIGHT);
	cub.img.addr = mlx_get_data_addr(cub.img.img, &cub.img.bpp, &cub.img.line_length, &cub.img.endian);

	mlx_loop_hook(cub.mlx, draw_image, &cub);
	mlx_loop(cub.mlx);

	mlx_destroy_image(cub.mlx, cub.img.img);
	mlx_destroy_window(cub.mlx, cub.win);
	mlx_destroy_display(cub.mlx);
	free(cub.mlx);
	return (0);
}