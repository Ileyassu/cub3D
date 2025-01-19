#include "../lib.h"

static void safe_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
    }
}

void cleanup_textures(t_mlx *mlx)
{
    if (!mlx) return;
    
    for (int i = 0; i < 4; i++) {
        safe_free((void**)&mlx->tex[i].texture);
    }
}

void cleanup_map(t_map *map)
{
    if (!map) return;
    
    if (map->map) {
        for (int i = 0; i < map->height; i++) {
            free(map->map[i]);
        }
        free(map->map);
    }
    
    // safe_free((void**)&map->no_texture);
    // safe_free((void**)&map->so_texture);
    // safe_free((void**)&map->we_texture);
    // safe_free((void**)&map->ea_texture);
    // safe_free((void**)&map->floor_color);
    // safe_free((void**)&map->ceiling_color);
}

void cleanup_mlx(t_mlx *mlx)
{
    if (!mlx || !mlx->mlx) return;

    cleanup_textures(mlx);

    // safe_free((void**)&mlx->player.rays);

    if (mlx->img.img) {
        mlx_destroy_image(mlx->mlx, mlx->img.img);
        mlx->img.img = NULL;
    }
    if (mlx->win) {
        mlx_destroy_window(mlx->mlx, mlx->win);
        mlx->win = NULL;
    }
    free(mlx->mlx);
}

// Modify the exit handlers
int handle_exit(t_mlx *mlx)
{
    cleanup_mlx(mlx);
    cleanup_map(&mlx->maps);
    exit(0);
    return (0);
}

// Modify close_window function
int close_window(void *mlx_ptr)
{
    t_mlx *mlx = (t_mlx*)mlx_ptr;
    printf("exited using close window button\n");
    handle_exit(mlx);
    return 0;
}

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

int mlx_initializer(t_mlx *mlx)
{
    mlx->mlx = mlx_init();
    if (!mlx->mlx)
        return (0);
    mlx->win = mlx_new_window(mlx->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "CUBED");
    mlx->img.img = mlx_new_image(mlx->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    mlx->img.addr = mlx_get_data_addr(mlx->img.img, &mlx->img.bits_per_pixel
        ,&mlx->img.line_length, &mlx->img.endian);
    return (1);
}

// char **map_calculator(t_map *maps)
// {
//     int tmp;
//     char **new_map;

//     if (!maps || !maps->map || maps->height <= 0 || maps->width <= 0)
//         return NULL;

//     new_map = malloc(sizeof(char *) * maps->height);
//     if (!new_map)
//         return NULL;
//     int i = 0;
//     for (i = 0; i < maps->height; i++)
//     {
//         new_map[i] = malloc(sizeof(char) * (maps->width + 1)); // +1 for null-terminator
//         if (!new_map[i])
//         {
//             while (--i >= 0)
//                 free(new_map[i]);
//             free(new_map);
//             return NULL;
//         }

//         strncpy(new_map[i], maps->map[i], maps->width); // Copy up to maps->width characters
//         tmp = strlen(maps->map[i]);

//         // Fill with '1' for the rest of the row, if needed
//         while (tmp < maps->width)
//             new_map[i][tmp++] = '1';

//         new_map[i][maps->width] = '\0'; // Null-terminate the string
//     }

//     return new_map;
// }


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
    // Map direction identifiers to face indices
    // t_texture_map texture_mapping[] = {
    //     {"NO", NORTH_FACE},
    //     {"SO", SOUTH_FACE},
    //     {"EA", EAST_FACE},
    //     {"WE", WEST_FACE}
    // };

    // Create texture path array in correct order
    const char *texture_paths[4] = {NULL, NULL, NULL, NULL};
    
    // Map textures to their correct faces
    if (mlx->maps.no_texture)
        texture_paths[NORTH_FACE] = mlx->maps.no_texture;
    if (mlx->maps.so_texture)
        texture_paths[SOUTH_FACE] = mlx->maps.so_texture;
    if (mlx->maps.ea_texture)
        texture_paths[EAST_FACE] = mlx->maps.ea_texture;
    if (mlx->maps.we_texture)
        texture_paths[WEST_FACE] = mlx->maps.we_texture;

    // Verify all textures are present
    for (int i = 0; i < 4; i++) {
        if (!texture_paths[i]) {
            printf("Error: Missing texture for face %d\n", i);
            exit(1);
        }
    }

    // Load each texture into its correct position
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

        // Store dimensions
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

        // Copy texture data
        int y = 0;
        while (y < 64) {
            int x = 0;
            while (x < 64) {
                int src_x = x * img.img_width / 64;
                int src_y = y * img.img_height / 64;
                
                unsigned int *ptr = (unsigned int*)(img.addr + 
                                  (src_y * img.line_length + src_x * (img.bits_per_pixel / 8)));
                mlx->tex[i].texture[y * 64 + x] = *ptr;
                x++;
            }
            y++;
        }

        mlx_destroy_image(mlx->mlx, img.img);
    }

    // Free the path strings if they were dynamically allocated
    for (int i = 0; i < 4; i++) {
        if (texture_paths[i]) {
            free((void*)texture_paths[i]);
        }
    }
}

void free_textures(t_map *map, t_mlx *mlx)
{
    if (map->no_texture)
        mlx_destroy_image(mlx->mlx, map->no_texture);
    if (map->so_texture)
        mlx_destroy_image(mlx->mlx, map->so_texture);
    if (map->we_texture)
        mlx_destroy_image(mlx->mlx, map->we_texture);
    if (map->ea_texture)
        mlx_destroy_image(mlx->mlx, map->ea_texture);
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
            if(mlx->maps.map[i][j] == 'N' || mlx->maps.map[i][j] == 'S' || 
                mlx->maps.map[i][j] == 'E' || mlx->maps.map[i][j] == 'W')
            {
                mlx->player.starting_face_in_map = mlx->maps.map[i][j];
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

    if (key_code == 65307)  // ESC key
    {
        free(mlx->player.rays);
        handle_exit(mlx);
        exit(0);
    }
    else if (key_code == 65361)  // Left arrow - rotate left
    {
        mlx->player.turn_direction = -1;
        refreshing(mlx);
    }
    else if (key_code == 65363)  // Right arrow - rotate right
    {
        mlx->player.turn_direction = 1;
        refreshing(mlx);
    }
    else if (key_code == 119 || key_code == 65362)  // W key or Up arrow - move forward
    {
        mlx->player.walk_direction = 1;
        refreshing(mlx);
    }
    else if (key_code == 115 || key_code == 65364)  // S key or Down arrow - move backward
    {
        mlx->player.walk_direction = -1;
        refreshing(mlx);
    }
    else if (key_code == 97)  // A key - strafe left
    {
        mlx->player.strafe_direction = -1;
        refreshing(mlx);
    }
    else if (key_code == 100)  // D key - strafe right
    {
        mlx->player.strafe_direction = 1;
        refreshing(mlx);
    }
    return 0;
}

int key_release(int key_code, void *mlx_ptr)
{
    t_mlx *mlx;

    mlx = (t_mlx *)mlx_ptr;
    if (key_code == 65307)
    {
        // handle_exit(mlx);
        exit(0);
    }
    else if (key_code == 119)
        mlx->player.walk_direction = 0; 
    else if(key_code == 115)
        mlx->player.walk_direction = 0;
    else if(key_code == 97)
        mlx->player.turn_direction = 0;
    else if(key_code == 100)
        mlx->player.turn_direction = 0;
    return 0;
}

// void printmap(char **map, int height)
// {
//     int i = 0;
//     int j = 0;
//     while(i < height)
//     {
//         j = 0;
//         while (map[i][j])
//         {
//             printf("%c", map[i][j]);
//             j++;
//         }
//             printf("\n");
//         i++;
//     }
// }

int main(int ac, char **av)
{
    (void)ac;
    t_map maps;
    t_mlx mlx;
    start_parsing(av[1], &maps);
    // printmap(maps.map, maps.height);
    if(!mlx_initializer(&mlx))
    {
        cleanup_map(&maps);
        return 0;
    }
    mlx.maps = maps;
    load_texture(&mlx);
    init_player(&mlx);
    // printf("NO = %s\n", maps.no_texture);
    draw_scene(&mlx);
    mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.img.img, 0, 0);
    mlx_hook(mlx.win, 02, 1L<<0, key_press, &mlx);
    mlx_hook(mlx.win, 3, 1L<<1, key_release, &mlx);
    mlx_hook(mlx.win, 17, 0, close_window, &mlx);
    mlx_loop(mlx.mlx);
    handle_exit(&mlx);
}