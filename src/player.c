#include "../lib.h"

void init_player(t_mlx *mlx)
{
    player_position(mlx);
    mlx->player.size = 10;
    mlx->player.radius = 3;
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    mlx->player.rotation_angle = M_PI/2;
    mlx->player.move_speed = 3.0;
    mlx->player.rotation_speed = 3 * (M_PI / 180); //formula to get radius angle from degrees
}

void update_line_angle (t_mlx *mlx)
{

    mlx->player.rotation_angle = mlx->player.turn_direction * mlx->player.rotation_speed;
    printf("new rotation angle = %f\n", mlx->player.rotation_angle);
}

void draw_line (t_mlx *mlx)
{
    int i = 0;
    float x = 0;
    float y = 0;
    float angle = mlx->player.rotation_angle;
    // printf("cos(angle) = %f\n",mlx->player.p_x + 50 * cos(angle));
    // printf("sin(angle) = %f\n",mlx->player.p_y + 50 * sin(ngle));
    while(i < 50)
    {
        x = mlx->player.p_x + i * cos(angle);
        y = mlx->player.p_y + i * sin(angle);
        if (x >= 0 && x < 1920 && y >= 0 && x < 1080)
        {
            my_mlx_pixel_put(&mlx->img, (int)x, (int)y, 0x00eeeee4);  
        }
        i++;
    }
}

void player_center_position(t_mlx *mlx, int x, int y)
{
    mlx->player.p_x = x * TILE_SIZE + (TILE_SIZE / 2);
    mlx->player.p_y = y * TILE_SIZE + (TILE_SIZE / 2);
}

void draw_player(t_mlx *mlx)
{
    // my_mlx_pixel_put(&mlx->img, mlx->player.p_x, mlx->player.p_y, 0x0000ff00);
    int i = 0;
    int j = 0;

    int top_left_x = mlx->player.p_x - (mlx->player.size / 2); //added these to center the shit out of the player
    int top_left_y = mlx->player.p_y - (mlx->player.size / 2);
    while(i < mlx->player.size)
    {
        j = 0;
        while(j < mlx->player.size)
        {
            my_mlx_pixel_put(&mlx->img, j + top_left_x, i + top_left_y, 0x00ffff00);
            j++;
        }
        i++;
    }
    update_line_angle (mlx);
    draw_line(mlx);
}