#include "../lib.h"

void load_textures(t_map *map, t_mlx *mlx)
{
    map->no_texture = mlx_xpm_file_to_image(mlx->mlx, "textures/Mona_Lisa.png", (int )mlx->player.wall_strip_width, (int)mlx->player.wall_strip_height);
    map->so_texture = mlx_xpm_file_to_image(mlx->mlx, "textures/Wheat_Field.png", (int )mlx->player.wall_strip_width, (int)mlx->player.wall_strip_height);
    map->we_texture = mlx_xpm_file_to_image(mlx->mlx, "textures/the_starry_night.png", (int )mlx->player.wall_strip_width, (int)mlx->player.wall_strip_height);
    map->ea_texture = mlx_xpm_file_to_image(mlx->mlx, "textures/girl_with_a_pearl_earring.png",(int )mlx->player.wall_strip_width, (int)mlx->player.wall_strip_height);

    if (!map->no_texture || !map->so_texture || !map->we_texture || !map->ea_texture)
    {
        perror("Failed to load textures");
        exit(EXIT_FAILURE);
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