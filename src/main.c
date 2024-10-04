#include "../libraries/cublib.h"

void init_player(t_player *player)
{
    player->angle = 0;  // Initial angle (facing right)
    player->dir_x = cos(player->angle);
    player->dir_y = sin(player->angle);
    player->move_speed = 2.0;
    player->rot_speed = 0.1;
}

void init_image(t_mlx *mlx)
{
    mlx->img = malloc(sizeof(t_img));
    mlx->img->img = mlx_new_image(mlx->mlx, 500, 500);
    mlx->img->addr = mlx_get_data_addr(mlx->img->img, &mlx->img->bits_per_pixel, &mlx->img->line_length, &mlx->img->endian);
}

void draw_line(t_mlx *mlx, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        my_mlx_pixel_put(mlx, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}


void draw_player(t_mlx *mlx, int size)
{
    int i, j;
    int x0, y0, x1, y1;

    // Draw player square
    for (i = -size/2; i < size/2; i++) {
        for (j = -size/2; j < size/2; j++) {
            my_mlx_pixel_put(mlx, mlx->player->p_x + i, mlx->player->p_y + j, 0xFF0000);
        }
    }

    // Draw direction line
    x0 = mlx->player->p_x;
    y0 = mlx->player->p_y;
    x1 = x0 + mlx->player->dir_x * (size * 2);
    y1 = y0 + mlx->player->dir_y * (size * 2);
    draw_line(mlx, x0, y0, x1, y1, 0x00FF00);
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
    draw_player(mlx, player_size);
    mlx_put_image_to_window(mlx->mlx, mlx->mlx_win, mlx->img->img, 0, 0);
}

int move_player(int key, void *ptr)
{
    t_mlx *mlx = (t_mlx *)ptr;
    float new_x, new_y;
    float move_x = 0, move_y = 0;
    int cell_size = 500 / 6;

    if (key == 65307) // ESC key to exit
        exit(0);
    else if (key == 119) // W key (move forward)
    {
        move_x = mlx->player->dir_x * mlx->player->move_speed;
        move_y = mlx->player->dir_y * mlx->player->move_speed;
    }
    else if (key == 115) // S key (move backward)
    {
        move_x = -mlx->player->dir_x * mlx->player->move_speed;
        move_y = -mlx->player->dir_y * mlx->player->move_speed;
    }
    else if (key == 97)  // A key (move left)
    {
        move_x = -mlx->player->dir_y * mlx->player->move_speed;
        move_y = mlx->player->dir_x * mlx->player->move_speed;
    }
    else if (key == 100) // D key (move right)
    {
        move_x = mlx->player->dir_y * mlx->player->move_speed;
        move_y = -mlx->player->dir_x * mlx->player->move_speed;
    }
    else if (key == 65361) // Left arrow key (rotate left)
    {
        mlx->player->angle -= mlx->player->rot_speed;
        mlx->player->dir_x = cos(mlx->player->angle);
        mlx->player->dir_y = sin(mlx->player->angle);
    }
    else if (key == 65363) // Right arrow key (rotate right)
    {
        mlx->player->angle += mlx->player->rot_speed;
        mlx->player->dir_x = cos(mlx->player->angle);
        mlx->player->dir_y = sin(mlx->player->angle);
    }
    else
        return 0;

    // Apply movement if any
    if (move_x != 0 || move_y != 0)
    {
        new_x = mlx->player->p_x + move_x;
        new_y = mlx->player->p_y + move_y;

        // Check for collisions
        if (new_x >= 0 && new_x < 500 && new_y >= 0 && new_y < 500 &&
            map[(int)new_y / cell_size][(int)new_x / cell_size] != '1')
        {
            mlx->player->p_x = new_x;
            mlx->player->p_y = new_y;
        }
    }

    // Redraw the scene
    mlx_clear_window(mlx->mlx, mlx->mlx_win);
    draw_scene(mlx);
    return 0;
}

int main()
{
    t_mlx mlx;
    t_player player;

    mlx.player = &player;
    init_player(&player);
    find_player_pos(&mlx);
    mlx.mlx = mlx_init();
    init_image(&mlx);
    mlx.mlx_win = mlx_new_window(mlx.mlx, 500, 500, "Cub3D");
    draw_scene(&mlx);
    mlx_key_hook(mlx.mlx_win, move_player, &mlx);
    mlx_loop(mlx.mlx);
}