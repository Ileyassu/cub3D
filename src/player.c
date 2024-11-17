#include "../lib.h"

float normalize_angle(float angle) {
    angle = remainder(angle, 2 * M_PI);
    if (angle < 0) {
        angle = (2 * M_PI) + angle;
    }
    return angle;
}

void ray (t_mlx *mlx, t_ray *ray)
{
    ray->ray_angle = normalize_angle(mlx->player.start_column_angle);
    ray->wall_hit_x = 0;
    ray->wall_hit_y = 0;
    ray->distance = 0;

    ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle <  M_PI; //condition
    ray->is_ray_facing_up = !ray->is_ray_facing_down;
    ray->is_ray_facing_right = ray->ray_angle < 0.5 * M_PI || ray->ray_angle > 1.5 * M_PI;
    ray->is_ray_facing_left = !ray->is_ray_facing_right;
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

void init_player(t_mlx *mlx)
{
    player_position(mlx);
    mlx->player.size = 4;
    mlx->player.radius = 3;
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    mlx->player.rotation_angle = M_PI/2;
    mlx->player.move_speed = 3.0;
    mlx->player.rotation_speed = 10 * (M_PI / 180); //formula to get radius angle from degrees
    mlx->player.fov = 60 * (M_PI / 180);
    mlx->player.wall_strip_width = 10;
    mlx->player.number_of_rays = TD_MAP_SIZE/mlx->player.wall_strip_width;
    mlx->player.rays = malloc(sizeof(t_ray) * mlx->player.number_of_rays);
    if (!mlx->player.rays)
        exit(1);
    mlx->player.start_column_angle = 0;
    ray_init (mlx);
}

int map_has_wall_at(float x, float y) {
    if (x < 0 || x > TD_MAP_SIZE|| y < 0 || y > TD_MAP_SIZE) {
        return 1;
    }
    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);
    return map[mapGridIndexY][mapGridIndexX] != 0;
}

int does_hit_right_Bottom_wall(t_mlx *mlx, int x, int y)
{
    int right_x = x + (mlx->player.size/2);
    int bottom_y = y + (mlx->player.size/2);
    int converting_x_to_grid = floor(right_x / TILE_SIZE);
    int converting_y_to_grid = floor(bottom_y / TILE_SIZE);

    if(map[converting_y_to_grid][converting_x_to_grid] == '1')
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

    if(map[converting_y_to_grid][converting_x_to_grid] == '1')
    {
        printf("oups !! you're hitting a wall\n");
        return (1);
    }
    return 0;
}

void update_player(t_mlx *mlx)
{
    int old_x = mlx->player.p_x;
    int old_y = mlx->player.p_y;

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
    if(does_hit_right_Bottom_wall(mlx ,mlx->player.p_x, mlx->player.p_y) || 
        does_hit_left_top_wall(mlx, mlx->player.p_x, mlx->player.p_y))
    {
        mlx->player.p_x = old_x;
        mlx->player.p_y = old_y;
    }
    printf("new rotation angle = %f\n", mlx->player.rotation_angle);
    printf("p_x = %d ----- p_y = %d\n", (int)mlx->player.p_x, (int)mlx->player.p_y);
    printf("p_x Math floor test = %d\n", (int)floor(mlx->player.p_x / 32));
    printf("p_y Math floor test = %d\n", (int)floor(mlx->player.p_y / 32));
}

void cast(t_mlx *mlx, t_ray *ray)
{
    float xintercept;
    float yintercept;
    float xstep;
    float ystep;

    yintercept = floor(mlx->player.p_y / TILE_SIZE) * TILE_SIZE;
    yintercept += ray->is_ray_facing_down ? TILE_SIZE : 0;

    xintercept = mlx->player.p_x + (yintercept - mlx->player.p_y) / tan(ray->ray_angle);
    ystep = TILE_SIZE;
    ystep *= ray->is_ray_facing_up ? -1 : 1;

    xstep = TILE_SIZE / tan(ray->ray_angle);
    xstep *= (ray->is_ray_facing_left && xstep > 0) ? -1 : 1;
    xstep *= (ray->is_ray_facing_right && xstep < 0) ? -1 : 1;
}

void draw_simple_rays(t_mlx *mlx)
{
    int i = 0;
    int column = 0;
    float x;
    float y;
    mlx->player.start_column_angle = mlx->player.rotation_angle - (mlx->player.fov / 2);
    mlx->player.start_column_angle = normalize_angle(mlx->player.start_column_angle);
    while(column < mlx->player.number_of_rays)
    {
        mlx->player.rays[column].ray_angle = mlx->player.start_column_angle;
        ray(mlx, &mlx->player.rays[column]);
        printf("is_ray_facing_right = %d\n", mlx->player.rays[column].is_ray_facing_right);
        printf("mlx->player.rays[%d].ray_angle %f\n",column, mlx->player.rays[column].ray_angle);
        i = 0;
        while(i < 60)
        {
            x = mlx->player.p_x + i * cos(mlx->player.rays[column].ray_angle);
            y = mlx->player.p_y + i * sin(mlx->player.rays[column].ray_angle);
            if (x >= 0 && x < TD_MAP_SIZE && y >= 0 && x < TD_MAP_SIZE)
            {
                my_mlx_pixel_put(&mlx->img, (int)x, (int)y, 0x00eeeee4);
            }
            i++;
        }
        mlx->player.start_column_angle += mlx->player.fov/mlx->player.number_of_rays;
        mlx->player.start_column_angle = normalize_angle(mlx->player.start_column_angle);
        column++;
    }
}

void draw_line (t_mlx *mlx)
{
    int i = 0;
    float x = 0;
    float y = 0;
    float angle = mlx->player.rotation_angle;
    
    printf("cos(angle) = %f\n",mlx->player.p_x + 50 * cos(angle));
    printf("sin(angle) = %f\n",mlx->player.p_y + 50 * sin(angle));
    printf("angle = %f\n", angle);
    while(i < 60)
    {
        x = mlx->player.p_x + i * cos(angle);
        y = mlx->player.p_y + i * sin(angle);
        if (x >= 0 && x < TD_MAP_SIZE && y >= 0 && x < TD_MAP_SIZE)
        {
            my_mlx_pixel_put(&mlx->img, (int)x, (int)y, 0x00eeeee4);  
        }
        i++;
    }
    draw_simple_rays(mlx);
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