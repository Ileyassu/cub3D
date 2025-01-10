#include "../lib.h"

static int	*load_image(t_mlx *mlx, char *path, t_img *img, int i)
{
	int		x;
	int		y;
	int		*res;

	if (!(img->img = mlx_xpm_file_to_image(mlx->mlx,
					path, &img->img_width, &img->img_height))){
        printf("error while loading image\n");
		exit(1);}
	mlx->tex[i].width = img->img_width;
	mlx->tex[i].height = img->img_height;
	img->addr = mlx_get_data_addr(img->img,
			&img->bits_per_pixel, &img->line_length, &img->endian);
	res = (int *)malloc(sizeof(int) * (img->img_width * img->img_height));
	y = -1;
	while (++y < img->img_height)
	{
		x = -1;
		while (++x < img->img_width)
		{
			res[img->img_width * y + x] = img->addr[img->img_width * y + x];
		}
	}
	mlx_destroy_image(mlx->mlx, img->img);
	return (res);
}

void		load_texture(t_mlx *mlx)
{
	int		i;
	t_img	img;

    mlx->tex[0].tex_path = mlx->maps.ea_texture;
    mlx->tex[1].tex_path = mlx->maps.no_texture;
    mlx->tex[2].tex_path = mlx->maps.so_texture;
    mlx->tex[3].tex_path = mlx->maps.we_texture;


	i = -1;
	while (++i < 4)
	{
		mlx->tex[i].texture =
			load_image(mlx, mlx->tex[i].tex_path, &img, i);
		free(mlx->tex[i].tex_path);
	}
}

void free_textures(t_map *map, t_mlx mlx)
{
    if (map->no_texture)
        mlx_destroy_image(mlx.mlx, map->no_texture);
    if (map->so_texture)
        mlx_destroy_image(mlx.mlx, map->so_texture);
    if (map->we_texture)
        mlx_destroy_image(mlx.mlx, map->we_texture);
    if (map->ea_texture)
        mlx_destroy_image(mlx.mlx, map->ea_texture);
}