#include "../lib.h"

float normalize_angle(float angle) {
    angle = remainder(angle, 2 * M_PI);
    if (angle < 0) {
        angle += (2 * M_PI);
    }
    return angle;
}

void ray (t_mlx *mlx, t_ray *ray)
{
    ray->ray_angle = normalize_angle(mlx->player.start_column_angle);
    ray->wall_hit_x = 0;
    ray->wall_hit_y = 0;
    ray->distance = 0;
    ray->vertical_distance = 0;
    ray->horizontal_distance = 0;
    ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle <  M_PI;
    ray->is_ray_facing_up = !ray->is_ray_facing_down;
    ray->is_ray_facing_right = ray->ray_angle < 0.5 * M_PI || ray->ray_angle > 1.5 * M_PI;
    ray->is_ray_facing_left = !ray->is_ray_facing_right;
    ray->was_hit_vertical = false;
}

void ray_init (t_mlx *mlx)
{
    int i;

    i = 0;
    while(i < mlx->player.number_of_rays)
    {
        ray(mlx, &mlx->player.rays[i]);
        i++;
    }
}

void set_starting_direction(t_mlx *mlx)
{
    char direction = mlx->player.starting_face_in_map;
    
    if (direction == 'N')
        mlx->player.rotation_angle = 3 * M_PI / 2;    // 270 degrees (facing up)
    else if (direction == 'S')
        mlx->player.rotation_angle = M_PI / 2;        // 90 degrees (facing down)
    else if (direction == 'E')
        mlx->player.rotation_angle = 0;               // 0 degrees (facing right)
    else if (direction == 'W')
        mlx->player.rotation_angle = M_PI;            // 180 degrees (facing left)
}

void init_player(t_mlx *mlx)
{
    player_position(mlx);
    mlx->player.strafe_direction = 0;
    mlx->player.size = 4;
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    set_starting_direction(mlx);
    mlx->player.move_speed = 10.0 * (TILE_SIZE / 64.0);
    mlx->player.rotation_speed = 2 * (M_PI / 180);
    mlx->player.fov = 60 * (M_PI / 180);
    mlx->player.wall_strip_width = 1;
    mlx->player.number_of_rays = WINDOW_WIDTH/mlx->player.wall_strip_width;
    mlx->player.rays = malloc(sizeof(t_ray) * mlx->player.number_of_rays);
    if (!mlx->player.rays)
        exit(1);
    mlx->player.start_column_angle = 0;
    ray_init(mlx);
}


// int map_has_wall_at(t_mlx *mlx, float x, float y) {
//     if (x < 0 || x >= mlx->maps.width * TILE_SIZE || y < 0 || y >= mlx->maps.height * TILE_SIZE) {
//         return 1; // Outside bounds, treat as a wall
//     }
//     int mapGridIndexX = floor(x / TILE_SIZE);
//     int mapGridIndexY = floor(y / TILE_SIZE);
    
//     // Ensure indices are within the bounds of the map array
//     if ((mapGridIndexY < 0 || mapGridIndexY >= mlx->maps.height) || 
//         (mapGridIndexX < 0 || mapGridIndexX >= mlx->maps.width)) {
//         return 1; // Treat as a wall if out of bounds
//     }
    
//     return mlx->maps.map[mapGridIndexY][mapGridIndexX] == '1'; // Check bounds here
// }

int does_hit_right_Bottom_wall(t_mlx *mlx, int x, int y)
{
    int right_x = x + (mlx->player.size/2);
    int bottom_y = y + (mlx->player.size/2);
    int converting_x_to_grid = floor(right_x / TILE_SIZE);
    int converting_y_to_grid = floor(bottom_y / TILE_SIZE);
    
    if(mlx->maps.map[converting_y_to_grid][converting_x_to_grid] == '1')
    {
        printf("oups !! you're hitting a wall\n");
        return (1);
    }
    return 0;
}

int does_hit_left_top_wall(t_mlx *mlx, int x, int y)
{
    int left_x = x + (mlx->player.size/-2);
    int top_y = y + (mlx->player.size/-2);
    int converting_x_to_grid = floor(left_x / TILE_SIZE);
    int converting_y_to_grid = floor(top_y / TILE_SIZE);

    if(mlx->maps.map[converting_y_to_grid][converting_x_to_grid] == '1')
    {
        return (1);
    }
    return 0;
}

void update_player(t_mlx *mlx)
{
    int old_x = mlx->player.p_x;
    int old_y = mlx->player.p_y;
    float move_step = 0;
    float strafe_step = 0;
    float tmp_walk_direction = mlx->player.walk_direction;
    float tmp_strafe_direction = mlx->player.strafe_direction;
    float tmp_turn_direction = mlx->player.turn_direction;

    // Reset movement flags
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    mlx->player.strafe_direction = 0;

    // Update rotation
    mlx->player.rotation_angle += tmp_turn_direction * mlx->player.rotation_speed;

    // Calculate forward/backward movement
    move_step = tmp_walk_direction * mlx->player.move_speed;
    
    // Calculate strafe movement
    strafe_step = tmp_strafe_direction * mlx->player.move_speed;

    // Update position with both forward/backward and strafe movement
    mlx->player.p_x += cos(mlx->player.rotation_angle) * move_step +
                       cos(mlx->player.rotation_angle + M_PI/2) * strafe_step;
    mlx->player.p_y += sin(mlx->player.rotation_angle) * move_step +
                       sin(mlx->player.rotation_angle + M_PI/2) * strafe_step;

    // Wall collision check
    if(does_hit_right_Bottom_wall(mlx, mlx->player.p_x, mlx->player.p_y) || 
       does_hit_left_top_wall(mlx, mlx->player.p_x, mlx->player.p_y))
    {
        mlx->player.p_x = old_x;
        mlx->player.p_y = old_y;
    }
}


int apply_shading(int base_color, float shading_factor)
{
    int r;
    int g;
    int b;
    int result;

    r = (base_color >> 16) & 0xFF;
    g = (base_color >> 8) & 0xFF;
    b = base_color & 0xFF;

    r = (int)(r * shading_factor);
    g = (int)(g * shading_factor);
    b = (int)(b * shading_factor);

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    result = (r << 16) | (g << 8) | b;
    return result;
}

uint32_t get_wall_color(t_ray ray)
{
    switch(ray.wall_face) {
        case NORTH_FACE:
            return 0xCC0000;  // Darker red for north-facing walls
        case SOUTH_FACE:
            return 0x00CC00;  // Darker green for south-facing walls
        case EAST_FACE:
            return 0x0000CC;  // Darker blue for east-facing walls
        case WEST_FACE:
            return 0xCCCC00;  // Darker yellow for west-facing walls
        default:
            return 0xFFFFFF;  // White for undefined faces
    }
}

void clearColorBuffer(t_mlx *mlx, uint32_t color) {
    int x = 0;
    while (x < WINDOW_WIDTH)
    {
        int y = 0;
        while (y < WINDOW_HEIGHT)
        {
            ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + x] = color;
            y++;
        }
        x++;
    }
}


void draw_line_3D_helper(t_mlx *mlx, int x, int start_y, int end_y, int color) // return to this function after finishing
{
    if (start_y > end_y)
        return;

    int y = start_y;
    while (y < end_y)
    {
        //mlx_destroy_image(mlx->mlx, mlx->img.img);
        my_mlx_pixel_put(&mlx->img, x, y, color);
        y++;
    }
}

void cpy_pixel(t_img *src, t_img *dst, int src_x, int src_y, int dst_x, int dst_y)
{
    char *dst_px;
    char *src_px;

    // if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
    //     return;
    dst_px = dst->addr + (dst_y * dst->line_length + dst_x * (dst->bits_per_pixel / 8));\
    src_px = src->addr + (src_y * src->line_length + src_x * (src->bits_per_pixel / 8));
    *(unsigned int*)dst_px = *(unsigned int *)src_px;
}


int get_pixel_color(t_tex *tex, int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= tex->width) x = tex->width - 1;
    if (y >= tex->height) y = tex->height - 1;
    
    int index = (int)(y * tex->width + x);
    if (index < 0 || index >= (tex->width * tex->height))
        return 0;  // Return black or another default color if out of bounds
        
    return tex->texture[index];
}

void renderColorBuffer(t_mlx *mlx) {
    mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
}

float calculate_perp_distance(t_ray *ray, float player_angle)
{
    float perp_distance = ray->distance * cos(ray->ray_angle - player_angle);
    return perp_distance > 0.001f ? perp_distance : 0.001f;
}

void render_3D_projection_walls(t_mlx *mlx)
{
    int i = 0;
    while (i < mlx->player.number_of_rays) 
    {
        t_ray *ray = &mlx->player.rays[i];
        
        // Calculate the perpendicular distance to avoid fisheye effect
        float perpDistance = calculate_perp_distance(ray, mlx->player.rotation_angle);
        
        // Calculate projected wall height
        float distanceProjPlane = (WINDOW_WIDTH / 2.0f) / tan(mlx->player.fov / 2.0f);
        float projectedWallHeight = (TILE_SIZE / perpDistance) * distanceProjPlane;
        
        // Calculate wall strip boundaries
        int wallStripHeight = (int)projectedWallHeight;
        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        
        // Clamp values
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
        wallBottomPixel = wallBottomPixel >= WINDOW_HEIGHT ? WINDOW_HEIGHT - 1 : wallBottomPixel;

        // Draw ceiling
        int y = 0;
        while (y < wallTopPixel) {
            ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + i] = 
                (mlx->maps.ceiling_rgb.r << 16) | 
                (mlx->maps.ceiling_rgb.g << 8) | 
                mlx->maps.ceiling_rgb.b;
            y++;
        }

        // Determine texture X coordinate (where exactly on the wall we hit)
        float wallHitX = ray->wall_hit_x;
        float wallHitY = ray->wall_hit_y;
        float textureOffsetX;

        if (ray->was_hit_vertical) {
            textureOffsetX = (int)(wallHitY) % TILE_SIZE;
        } else {
            textureOffsetX = (int)(wallHitX) % TILE_SIZE;
        }
        
        // Scale texture offset to fit texture size
        textureOffsetX = textureOffsetX * 64.0f / TILE_SIZE;

        // Select texture based on wall face
        int texNum = ray->wall_face;
        t_tex *texture = &mlx->tex[texNum];

        // Draw the wall slice
        float step = 64.0f / (float)wallStripHeight;  // Using fixed texture height of 64
        float texPos = 0;
        if (wallStripHeight > WINDOW_HEIGHT) {
            texPos = (wallStripHeight - WINDOW_HEIGHT) / 2.0f * step;
        }
        y = wallTopPixel;
        while (y < wallBottomPixel) 
        {
            int textureOffsetY = (int)texPos & 63;  // Keep within 64 pixel bounds
            texPos += step;

            // Get color from texture
            int texX = (int)textureOffsetX & 63;
            uint32_t texelColor = texture->texture[64 * textureOffsetY + texX];
            
            // Apply distance shading
            float shade = 1.0f - (perpDistance / (WINDOW_WIDTH * 0.8f));
            shade = fmax(0.3f, shade);  // Limit minimum brightness
            
            uint32_t r = ((texelColor >> 16) & 0xFF) * shade;
            uint32_t g = ((texelColor >> 8) & 0xFF) * shade;
            uint32_t b = (texelColor & 0xFF) * shade;
            
            texelColor = (r << 16) | (g << 8) | b;
            
            ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + i] = texelColor;
            y++;
        }
        // printf("rgb === %d\n",mlx->maps.floor_rgb.b);
        // Draw floor
        y = wallBottomPixel;
        while (y < WINDOW_HEIGHT) {
            ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + i] = 
                (mlx->maps.floor_rgb.r << 16) | 
                (mlx->maps.floor_rgb.g << 8) | 
                mlx->maps.floor_rgb.b;
                y++;
        }
        i++;
    }
}


void cast(t_mlx *mlx, t_ray *ray)
{
    float ray_dir_x = cos(ray->ray_angle);
    float ray_dir_y = sin(ray->ray_angle);
    
    float pos_x = mlx->player.p_x / TILE_SIZE;
    float pos_y = mlx->player.p_y / TILE_SIZE;
    
    int map_x = (int)pos_x;
    int map_y = (int)pos_y;
    
    float delta_dist_x = fabs(1.0f / ray_dir_x);
    float delta_dist_y = fabs(1.0f / ray_dir_y);

    int step_x;
    int step_y;
    float side_dist_x;
    float side_dist_y;
    
    // Calculate step and initial side_dist
    if (ray_dir_x < 0) {
        step_x = -1;
        side_dist_x = (pos_x - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0f - pos_x) * delta_dist_x;
    }
    if (ray_dir_y < 0) {
        step_y = -1;
        side_dist_y = (pos_y - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0f - pos_y) * delta_dist_y;
    }
    
    // DDA Algorithm
    int hit = 0;
    int side = 0; // 0 for x-side, 1 for y-side
    
    while (hit == 0) {
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0;
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1;
        }
        
        // Check if ray has hit a wall
        if (map_x < 0 || map_y < 0 || map_x >= mlx->maps.width || map_y >= mlx->maps.height) {
            hit = 1;
            ray->distance = FLT_MAX;
            return;
        }
        if (mlx->maps.map[map_y][map_x] == '1') {
            hit = 1;
        }
    }
    
    // Calculate exact hit position and distance
    if (side == 0) {
        ray->distance = (map_x - pos_x + (1 - step_x) / 2) / ray_dir_x;
        ray->wall_hit_x = mlx->player.p_x + ray->distance * TILE_SIZE * ray_dir_x;
        ray->wall_hit_y = mlx->player.p_y + ray->distance * TILE_SIZE * ray_dir_y;
        ray->was_hit_vertical = true;
        ray->wall_face = (step_x > 0) ? EAST_FACE : WEST_FACE;
    } else {
        ray->distance = (map_y - pos_y + (1 - step_y) / 2) / ray_dir_y;
        ray->wall_hit_x = mlx->player.p_x + ray->distance * TILE_SIZE * ray_dir_x;
        ray->wall_hit_y = mlx->player.p_y + ray->distance * TILE_SIZE * ray_dir_y;
        ray->was_hit_vertical = false;
        ray->wall_face = (step_y > 0) ? SOUTH_FACE : NORTH_FACE;
    }
    
    ray->distance *= TILE_SIZE; // Convert to pixel distance
}

void adjusting_rays(t_mlx *mlx)
{
    int column = 0;
    mlx->player.start_column_angle = mlx->player.rotation_angle - (mlx->player.fov / 2);
    mlx->player.start_column_angle = normalize_angle(mlx->player.start_column_angle);
    while(column < mlx->player.number_of_rays)
    {
        mlx->player.rays[column].id = column;
        mlx->player.rays[column].ray_angle = mlx->player.start_column_angle;
        ray(mlx, &mlx->player.rays[column]);
        mlx->player.start_column_angle += mlx->player.fov/mlx->player.number_of_rays;
        mlx->player.start_column_angle = normalize_angle(mlx->player.start_column_angle);
        cast(mlx, &mlx->player.rays[column]);
        column++;
    }
}

// void draw_line (t_mlx *mlx)
// {
//     int i = 0;
//     float x = 0;
//     float y = 0;
//     float angle = mlx->player.rotation_angle;
//     while(i < 30)
//     {
//         x = mlx->player.p_x + i * cos(angle);
//         y = mlx->player.p_y + i * sin(angle);
//         if (x >= 0 && x < mlx->maps.td_map_size && y >= 0 && x < mlx->maps.td_map_size)
//         {
//             my_mlx_pixel_put(&mlx->img, (int)x, (int)y, 0x00eeeee4);  
//         }
//         i++;
//     }
//     // draw_simple_rays(mlx);
// }
void player_center_position(t_mlx *mlx, int x, int y)
{
    mlx->player.p_x = x * TILE_SIZE + (TILE_SIZE / 2);
    mlx->player.p_y = y * TILE_SIZE + (TILE_SIZE / 2);
}

// void draw_player(t_mlx *mlx)
// {
//     int i = 0;
//     int j = 0;

//     int top_left_x = mlx->player.p_x - (mlx->player.size / 2); //added these to center the shit out of the player
//     int top_left_y = mlx->player.p_y - (mlx->player.size / 2);
//     while(i < mlx->player.size)
//     {
//         j = 0;
//         while(j < mlx->player.size)
//         {
//             my_mlx_pixel_put(&mlx->img, j + top_left_x, i + top_left_y, 0x00ffff00);
//             j++;
//         }
//         i++;
//     }
//     draw_line(mlx);
// }
void render_all(t_mlx *mlx)
{
    adjusting_rays(mlx);
    render_3D_projection_walls(mlx);
}