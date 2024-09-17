#include "../libraries/cublib.h"

char map[6][6] = {
        "111111",
        "100001",
        "100001",
        "100001",
        "100001",
        "111111"
    };

void init_image(t_mlx *mlx)
{
    mlx->img = malloc(sizeof(t_img));
    mlx->img->img = mlx_new_image(mlx->mlx, 500, 500);
    mlx->img->addr = mlx_get_data_addr(mlx->img->img, &mlx->img->bits_per_pixel, &mlx->img->line_length, &mlx->img->endian);
}

void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
    char *dst;

    dst = mlx->img->addr + (y * mlx->img->line_length + x * (mlx->img->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

void draw_map(t_mlx *mlx)

{
    int map_width = 6;
    int map_height = 6;
    int cell_size = 500 / map_width; // Assuming the window size is 500x500

    int i = 0;
    int j = 0;

    while(i < map_height)
    {
        j = 0;
        while(j < map_width)
        {
            if (map[i][j] == '1')
            {
                int x = 0;
                while(x < cell_size)
                {
                    int y = 0;
                    while(y < cell_size)
                    {
                        my_mlx_pixel_put(mlx, j * cell_size + x, i * cell_size + y, 0xFFFFF);
                        y++;
                    }
                    x++;
                }   
            } 
            j++;
        }
        i++;
    }
}

void draw_scene (t_mlx *mlx)
{
    int x = 0;
    int y = 0;
    int width = 500;
    int height = 500;

    while(x < height)
    {
        y = 0;
        while(y < width)
        {
            my_mlx_pixel_put(mlx, x, y, 0x0000FF);
            y++;
        }
        x++;
    }
    x = 0;
    y = 0;
    int player_size = 10;
    draw_map(mlx);
    while(y < player_size)
    {
        x = 0;
        while(x < player_size)
        {
            my_mlx_pixel_put(mlx, mlx->player->p_x + x, mlx->player->p_y + y, 0xFF0000);
            x++;
        }
        y++;
    }
    mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->img->img, 0, 0);
}
void draw_player(t_mlx *mlx, void *win, int size, int color)
{
    int i = 0;
    int j = 0;

    while(i < size)
    {
        j = 0;
        while(j < size)
        {
            mlx_pixel_put(mlx->mlx, win, mlx->player->p_x + i, mlx->player->p_y + j, color);
            j++;
        }
        i++;
    }
}

int move_player(unsigned int key, void *ptr)
{
    t_mlx *mlx = (t_mlx *)ptr;
    int cell_size = 500 / 6;
    int new_x = mlx->player->p_x;
    int new_y = mlx->player->p_y;

    if (key == 65307) // ESC key to exit
    {
        exit(0);
    }
    else if (key == 119) // W key (move up)
    {
        new_y -= 10; // Move up
    }
    else if (key == 115) // S key (move down)
    {
        new_y += 10; // Move down
    }
    else if (key == 97)  // A key (move left)
    {
        new_x -= 10; // Move left
    }
    else if (key == 100) // D key (move right)
    {
        new_x += 10; // Move right
    }
    int map_x = new_x / cell_size;
    int map_y = new_y / cell_size;

    if (map[map_x][map_y] != '1')
    {
        mlx->player->p_x = new_x;
        mlx->player->p_y = new_y;
    }
    // Clear the window and redraw the player at the new position
    mlx_clear_window(mlx->mlx, mlx->mlx_win);
    draw_map(mlx);
    draw_scene(mlx);
    //draw_player(mlx, mlx->mlx_win, 10, 0xFF0000);
    printf("key = %u\n", key);
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
    init_image(&mlx);
    mlx.mlx_win = mlx_new_window(mlx.mlx, 500, 500, "test");
    //draw_player(&mlx, mlx.mlx_win, 10, 0xFF0000); // Draw a 10x10 red square
    //draw_scene(&mlx);
    draw_map(&mlx);
    mlx_key_hook(mlx.mlx_win, move_player, &mlx);
    mlx_loop(mlx.mlx);
}