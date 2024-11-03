#include "lib.h"

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void make_player (t_data *data)
{
	int i, j, size;
	i = 0;
	j = 0;
	size = 123;
	int x, y = 300;
	while(i < size)
	{
		j = 0;
		while(j < size)
		{
			my_mlx_pixel_put(data, 300 + i, y + j, 0x00ff0000);
			j++;
		}
		i++;
	}
}

int	main(void)
{
	int i = 0;
	char *map2[6] = {  
        "110000",
        "100001",
        "101001",
        "10N001",
        "100101",
        "111111"
    };
	
	// void	*mlx;
	// void	*mlx_win;
	// t_data	img;

	// mlx = mlx_init();
	// mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
	// img.img = mlx_new_image(mlx, 1920, 1080);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
	// 							&img.endian);
	// make_player (&img);
	// mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	// mlx_loop(mlx);
}