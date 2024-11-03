#include "../lib.h"

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char *dst;

    dst = img->addr + (y  * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void mlx_initializer(t_mlx *mlx)
{
    mlx->mlx = mlx_init();
    mlx->win = mlx_new_window(mlx->mlx, WINDOW_SIZE, WINDOW_SIZE, "Cubix danger bcp");
    mlx->img.img = mlx_new_image(mlx->mlx, WINDOW_SIZE, WINDOW_SIZE);
    mlx->img.addr = mlx_get_data_addr(mlx->img.img, &mlx->img.bits_per_pixel
        ,&mlx->img.line_length, &mlx->img.endian);
}

void draw_wall (t_mlx *mlx, int x, int y)
{
    int i = 0;
    int j = 0;
    (void)x;
    (void)y;
    while(i < TILE_SIZE - 1)
    {
        j = 0;
        while(j < TILE_SIZE - 1)
        {
            my_mlx_pixel_put(&mlx->img, j + (x * TILE_SIZE), i + (y * TILE_SIZE), 0x00ff0000);
            j++;
        }
        i++;
    }
}

void draw_background (t_mlx *mlx)
{
    int i = 0;
    int j = 0;

    while(i < (WINDOW_SIZE))
    {
        j = 0;
        while(j < (WINDOW_SIZE))
        {
            my_mlx_pixel_put(&mlx->img, j, i, 0x000000ff);
            j++;
        }
        i++;
    }
}

void draw_map(t_mlx *mlx)
{
    int i = 0;
    int j = 0;

    draw_background(mlx);
    while(i < MAP_HEIGHT)
    {
        j = 0;
        while(map[i][j])
        {
            if(map[i][j] == '1')
                draw_wall(mlx, j, i);
            if(map[i][j] == 'N')
            {
                player_position(mlx, j, i);
                init_player(mlx);
                draw_player(mlx);
            }
            j++;
        }
        i++;
    }
}
int key_hook(int key_code, void *mlx)
{
    (void)mlx;
    if (key_code == 53)
        exit(0);
    else if (key_code == 13)
        printf("w\n");
    else if(key_code == 1)
        printf("s\n");
    else if(key_code == 0)
        printf("a\n");
    else if(key_code == 2)
        printf("d\n");
    return 0;
}
int main()
{
    t_mlx mlx;
    
    mlx_initializer(&mlx);
    draw_map(&mlx);
    mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img.img, 0, 0);
    mlx_key_hook(mlx.win, key_hook, &mlx);
    mlx_loop(mlx.mlx);
}