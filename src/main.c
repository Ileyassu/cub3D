#include "../libraries/cublib.h"

void draw_map(t_mlx *mlx)

{
    char map[6][6] = {
        "111111",
        "100001",
        "100001",
        "100001",
        "100001",
        "111111"
    };

    int map_width = 6;
    int map_height = 6;
    int cell_size = 500 / map_width; // Assuming the window size is 500x500

    for (int i = 0; i < map_height; i++)
    {
        for (int j = 0; j < map_width; j++)
        {
            if (map[i][j] == '1')
            {
                for (int x = 0; x < cell_size; x++)
                {
                    for (int y = 0; y < cell_size; y++)
                    {
                        mlx_pixel_put(mlx->mlx, mlx->mlx_win, j * cell_size + x, i * cell_size + y, 0xFFFFFF);
                    }
                }
            }
        }
    }
}


void draw_player(t_mlx *mlx, void *win, int size, int color)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mlx_pixel_put(mlx->mlx, win, mlx->player->p_x + i, mlx->player->p_y + j, color);
        }
    }
}

int move_player(unsigned key, void *ptr)
{
    t_mlx *mlx = (t_mlx *)ptr;
    if (key == 65307) // ESC key
    {
        exit(0);
    }
    else if (key == 122) // Z key
    {
        mlx->player->p_y -= 10; // Move player forward
    }
    else if (key == 115) // S key
    {
        mlx->player->p_y += 10; // Move player backward
    }
    else if (key == 113) // Q key
    {
        mlx->player->p_x -= 10; // Move player left
    }
    else if (key == 100) // D key
    {
        mlx->player->p_x += 10; // Move player right
    }
    // Clear the window and redraw the player at the new position
    mlx_clear_window(mlx->mlx, mlx->mlx_win);
    //draw_map(mlx);
    draw_player(mlx, mlx->mlx_win, 10, 0xFF0000);
    return 0;
}

int main()
{
    t_mlx mlx;
    t_player player;

    player.p_x = 0;
    player.p_y = 0;
    mlx.player = &player;
    mlx.mlx = mlx_init();
    mlx.mlx_win = mlx_new_window(mlx.mlx, 500, 500, "test");
    draw_player(&mlx, mlx.mlx_win, 10, 0xFF0000); // Draw a 10x10 red square
    mlx_key_hook(mlx.mlx_win, move_player, &mlx);
    draw_map(&mlx);
    mlx_loop(mlx.mlx);
}