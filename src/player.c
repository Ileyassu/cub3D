#include "../lib.h"

void init_player(t_mlx *mlx)
{
    mlx->player.size = 10;
}

void player_position(t_mlx *mlx, int x, int y)
{
    mlx->player.p_x = x * TILE_SIZE + (TILE_SIZE / 2);
    mlx->player.p_y = y * TILE_SIZE + (TILE_SIZE / 2);
    printf("x = %f ---- y = %f\n", mlx->player.p_x, mlx->player.p_y);
}

void draw_player(t_mlx *mlx)
{
    // my_mlx_pixel_put(&mlx->img, mlx->player.p_x, mlx->player.p_y, 0x0000ff00);
    int i = 0;
    int j = 0;

    int top_left_x = mlx->player.p_x - (mlx->player.size / 2); //added these to center the shit out of the plater
    int top_left_y = mlx->player.p_y - (mlx->player.size / 2);
    while(i < mlx->player.size)
    {
        j = 0;
        while(j < mlx->player.size)
        {
            my_mlx_pixel_put(&mlx->img, j + top_left_x, i + top_left_y, 0x0000ff00);
            printf("player size = %f\n", mlx->player.size);
            j++;
        }
        i++;
    }
}