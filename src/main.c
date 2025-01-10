#include "../lib.h"

void cleanup_textures(t_mlx *mlx)
{
    // Free texture data
    for (int i = 0; i < 4; i++) {
        if (mlx->tex[i].texture) {
            free(mlx->tex[i].texture);
            mlx->tex[i].texture = NULL;
        }
    }
}

// void cleanup_map(t_map *map)
// {
//     if (!map)
//         return;
        
//     // Free texture paths
//     if (map->no_texture)
//         free(map->no_texture);
//     if (map->so_texture)
//         free(map->so_texture);
//     if (map->we_texture)
//         free(map->we_texture);
//     if (map->ea_texture)
//         free(map->ea_texture);
    
//     // Free colors
//     if (map->floor_color)
//         free(map->floor_color);
//     if (map->ceiling_color)
//         free(map->ceiling_color);
    
//     // Free the map array
//     if (map->map) {
//         for (int i = 0; i < map->height; i++) {
//             if (map->map[i])
//                 free(map->map[i]);
//         }
//         free(map->map);
//     }
// }

// void cleanup_mlx(t_mlx *mlx)
// {
//     if (!mlx || !mlx->mlx)
//         return;

//     // Free ray array
//     if (mlx->player.rays) {
//         free(mlx->player.rays);
//         mlx->player.rays = NULL;
//     }

//     // Destroy the image if it exists
//     if (mlx->img.img) {
//         mlx_destroy_image(mlx->mlx, mlx->img.img);
//         mlx->img.img = NULL;
//     }

//     // Destroy the window if it exists
//     if (mlx->win) {
//         mlx_destroy_window(mlx->mlx, mlx->win);
//         mlx->win = NULL;
//     }

//     // Free MLX itself
//     if (mlx->mlx) {
//         mlx_destroy_display(mlx->mlx);
//         free(mlx->mlx);
//         mlx->mlx = NULL;
//     }
// }

// // Function to handle exit
// int handle_exit(t_mlx *mlx)
// {
//     // Clean up in the correct order
//     cleanup_textures(mlx);
//     cleanup_mlx(mlx);
//     cleanup_map(&mlx->maps);
//     exit(0);
//     return (0);
// }

void free_rays(t_mlx *mlx)
{
    free(mlx->player.rays);
}
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

void load_texture(t_mlx *mlx)
{
    t_img img;
    const char *texture_paths[4] = {
        mlx->maps.ea_texture,
        mlx->maps.no_texture,
        mlx->maps.so_texture,
        mlx->maps.we_texture
    };

    for (int i = 0; i < 4; i++) {
        // Load image
        img.img = mlx_xpm_file_to_image(mlx->mlx, (char*)texture_paths[i], 
                                       &img.img_width, &img.img_height);
        if (!img.img) {
            printf("Failed to load texture: %s\n", texture_paths[i]);
            exit(1);
        }

        // Verify texture dimensions
        if (img.img_width != 64 || img.img_height != 64) {
            printf("Warning: Texture %s is not 64x64 (%dx%d)\n", 
                   texture_paths[i], img.img_width, img.img_height);
            exit(1);
        }

        // Store dimensions (force 64x64)
        mlx->tex[i].width = 64;
        mlx->tex[i].height = 64;

        // Get image data
        img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, 
                                    &img.line_length, &img.endian);

        // Allocate texture buffer
        mlx->tex[i].texture = (uint32_t *)malloc(sizeof(uint32_t) * 64 * 64);
        if (!mlx->tex[i].texture) {
            printf("Failed to allocate memory for texture %d\n", i);
            exit(1);
        }

        // Copy and scale texture data to 64x64
        for (int y = 0; y < 64; y++) {
            for (int x = 0; x < 64; x++) {
                // Scale coordinates to original texture size
                int src_x = x * img.img_width / 64;
                int src_y = y * img.img_height / 64;
                
                unsigned int *ptr = (unsigned int*)(img.addr + 
                                  (src_y * img.line_length + src_x * (img.bits_per_pixel / 8)));
                mlx->tex[i].texture[y * 64 + x] = *ptr;
            }
        }

        mlx_destroy_image(mlx->mlx, img.img);
        if (texture_paths[i]) {
            free((void*)texture_paths[i]);
        }
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

void player_position(t_mlx *mlx)
{
    int i = 0;
    int j = 0;
    while(i < mlx->maps.height)
    {
        j = 0;
        while (mlx->maps.map[i][j])
        {
            if(mlx->maps.map[i][j] == 'N')
            {
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
            mlx_destroy_window(mlx, mlx->win);
            free_textures(&mlx->maps, *mlx);
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
    start_parsing(av[1], &maps);
    printmap(maps.map, maps.height);
    mlx_initializer(&mlx);
    mlx.maps = maps;
    printf("Loading texture: %s\n", mlx.maps.ea_texture);
    load_texture(&mlx);
    init_player(&mlx);
    draw_scene(&mlx);
    mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img.img, 0, 0);
    mlx_hook(mlx.win, 02, 1L<<0, key_press, &mlx);
    mlx_hook(mlx.win, 3, 1L<<1, key_release, &mlx);
    mlx_loop(mlx.mlx);
} 