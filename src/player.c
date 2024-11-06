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

void update_player(t_mlx *mlx)
{
    printf("-------------------------------------------------\n");
    printf("turn direction = %f\n", mlx->player.turn_direction);
    float move_step = 0;
    float tmp_walk_direction = mlx->player.walk_direction;
    float tmp_turn_direction = mlx->player.turn_direction;
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    // Accumulate the rotation angle over time
    mlx->player.rotation_angle += tmp_turn_direction * mlx->player.rotation_speed;
    move_step = tmp_walk_direction * mlx->player.move_speed;
    mlx->player.p_x += cos(mlx->player.rotation_angle) * move_step;
    mlx->player.p_y += sin(mlx->player.rotation_angle) * move_step;
    printf("new rotation angle = %f\n", mlx->player.rotation_angle);
}

void draw_line (t_mlx *mlx)
{
    int i = 0;
    float x = 0;
    float y = 0;
    float angle = mlx->player.rotation_angle;
    // int j = 0;
    // int k = 0;

    printf("cos(angle) = %f\n",mlx->player.p_x + 50 * cos(angle));
    printf("sin(angle) = %f\n",mlx->player.p_y + 50 * sin(angle));
    printf("angle = %f\n", angle);
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

    // while(j < 10)
    // {
    //     k = 0;
    //     while(k < 10)
    //     {
    //         my_mlx_pixel_put(&mlx->img, k + (x - (10 / 2)), j + (y - (10 / 2)), 0x00000000);
    //         k++;
    //     }
    //     j++;
    // }
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
    draw_line(mlx);
    printf("turn direction = %f\n", mlx->player.turn_direction);
    printf("turn direction = %f\n", mlx->player.walk_direction);

}