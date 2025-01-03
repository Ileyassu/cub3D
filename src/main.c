#include "../lib.h"

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char *dst;
    if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
        return;
    dst = img->addr + (y  * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void mlx_initializer(t_mlx *mlx)
{
    mlx->mlx = mlx_init();
    mlx->win = mlx_new_window(mlx->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Purgatory");
    mlx->img.img = mlx_new_image(mlx->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    mlx->img.addr = mlx_get_data_addr(mlx->img.img, &mlx->img.bits_per_pixel
        ,&mlx->img.line_length, &mlx->img.endian);
}

#include <stdlib.h>
#include <string.h>

char **map_calculator(t_map *maps)
{
    int i, tmp;
    char **new_map;

    if (!maps || !maps->map || maps->height <= 0 || maps->width <= 0)
        return NULL;

    new_map = malloc(sizeof(char *) * maps->height);
    if (!new_map)
        return NULL;

    for (i = 0; i < maps->height; i++)
    {
        new_map[i] = malloc(sizeof(char) * (maps->width + 1)); // +1 for null-terminator
        if (!new_map[i])
        {
            while (--i >= 0)
                free(new_map[i]);
            free(new_map);
            return NULL;
        }

        strncpy(new_map[i], maps->map[i], maps->width); // Copy up to maps->width characters
        tmp = strlen(maps->map[i]);

        // Fill with '1' for the rest of the row, if needed
        while (tmp < maps->width)
            new_map[i][tmp++] = '1';

        new_map[i][maps->width] = '\0'; // Null-terminate the string
    }

    return new_map;
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

    while(i < (mlx->maps.td_map_size))
    {
        j = 0;
        while(j < (mlx->maps.td_map_size))
        {
            my_mlx_pixel_put(&mlx->img, j, i, 0x000000ff);
            j++;
        }
        i++;
    }
}

void player_position(t_mlx *mlx)
{
    int i = 0;
    int j = 0;
    while(i < mlx->maps.height)
    {
        j = 0;
        while (mlx->maps.map[i][j])
        {
            printf("stetetet\n");
            if(mlx->maps.map[i][j] == 'N')
            {
                printf("i = %d ---- j = %d\n", i,j);
                player_center_position(mlx, j, i);
                break;
            }
            j++;
        }
        i++;
    }
}

// void draw_map(t_mlx *mlx)
// {
//     int i = 0;
//     int j = 0;

//     draw_background(mlx);
//     while(i < mlx->maps.height)
//     {
//         j = 0;
//         while(mlx->maps.map[i][j] != '\0')  // Check for end of string
//         {
//             if(mlx->maps.map[i][j] == '1')
//                 draw_wall(mlx, j, i);
//             j++;
//         }
//         i++;
//     }
// }

void draw_scene(t_mlx *mlx)
{
    render_all(mlx);
    // load_textures(map, mlx);
    // draw_map(mlx);
    // draw_player(mlx);
}

void refreshing(t_mlx *mlx)
{
    
    update_player (mlx);
    mlx_clear_window(mlx->mlx, mlx->win);
    draw_scene(mlx);
    mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
}

int key_press(int key_code, void *mlx_ptr)
{
    t_mlx *mlx;

        mlx = (t_mlx *)mlx_ptr;
        if (key_code == 65307)
        {
            free(mlx->player.rays);
            exit(0);
        }
        else if (key_code == 119)
        {
            mlx->player.walk_direction = 1;
            refreshing(mlx);
        }
        else if(key_code == 115)
        {
            mlx->player.walk_direction = -1;
            refreshing(mlx);
        }
        else if(key_code == 97)
        {
            mlx->player.turn_direction = -1;
            refreshing(mlx);
        }
        else if(key_code == 100)
        {
            mlx->player.turn_direction = 1;
            refreshing(mlx);
        }
        printf("keypress = %d\n", key_code);
    return 0;
}

int key_release(int key_code, void *mlx_ptr)
{
    t_mlx *mlx;

    mlx = (t_mlx *)mlx_ptr;
    if (key_code == 65307)
        exit(0);
    else if (key_code == 119)
    {
        mlx->player.walk_direction = 0; 
        printf("keyPressed = w released\n");
    }
    else if(key_code == 115)
    {
        mlx->player.walk_direction = 0;
        printf("keyPressed = s released\n");
    }
    else if(key_code == 97)
    {
        mlx->player.turn_direction = 0;
        printf("keyPressed = a released\n");
    }
    else if(key_code == 100)
    {
        mlx->player.turn_direction = 0;
        printf("keyPressed = d released\n");
    }
    return 0;
}

void printmap(char **map, int height)
{
    int i = 0;
    int j = 0;
    while(i < height)
    {
        j = 0;
        while (map[i][j])
        {
            printf("%c", map[i][j]);
            j++;
        }
            printf("\n");
        i++;
    }
}
int main(int ac, char **av)
{
    (void)ac;
    t_map maps;
    t_mlx mlx;
    char **new_map; //free this mora manssaliw
    start_parsing(av[1], &maps);
    mlx_initializer(&mlx);
    new_map = map_calculator(&maps);
    free2d(maps.map);
    maps.map = new_map;
    // printmap(new_map, maps.height);
    mlx.maps = maps;
    init_player(&mlx);
    draw_scene(&mlx);
    mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img.img, 0, 0);
    mlx_hook(mlx.win, 02, 1L<<0, key_press, &mlx);
    mlx_hook(mlx.win, 3, 1L<<1, key_release, &mlx);
    mlx_loop(mlx.mlx);
} 