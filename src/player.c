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
    char direction;

    direction = mlx->player.starting_face_in_map;
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

int	does_hit_right_bottom_wall(t_mlx *mlx, int x, int y)
{
    int	right_x;
    int	bottom_y;
    int	converting_x_to_grid;
    int	converting_y_to_grid;

    right_x = x + (mlx->player.size / 2);
    bottom_y = y + (mlx->player.size / 2);
    converting_x_to_grid = floor(right_x / TILE_SIZE);
    converting_y_to_grid = floor(bottom_y / TILE_SIZE);
    if (mlx->maps.map[converting_y_to_grid][converting_x_to_grid] == '1')
        return (1);
    return (0);
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

static void	reset_movement_flags(t_mlx *mlx)
{
    mlx->player.turn_direction = 0;
    mlx->player.walk_direction = 0;
    mlx->player.strafe_direction = 0;
}

static void	update_position(t_mlx *mlx, float move_step, float strafe_step)
{
    mlx->player.p_x += cos(mlx->player.rotation_angle) * move_step
        + cos(mlx->player.rotation_angle + M_PI / 2) * strafe_step;
    mlx->player.p_y += sin(mlx->player.rotation_angle) * move_step
        + sin(mlx->player.rotation_angle + M_PI / 2) * strafe_step;
}

void	update_player(t_mlx *mlx)
{
    int		old_x;
    int		old_y;
    float	move_step;
    float	strafe_step;
    float	tmp_walk_direction;
    float	tmp_strafe_direction;
    float	tmp_turn_direction;

    old_x = mlx->player.p_x;
    old_y = mlx->player.p_y;
    move_step = 0;
    strafe_step = 0;
    tmp_walk_direction = mlx->player.walk_direction;
    tmp_strafe_direction = mlx->player.strafe_direction;
    tmp_turn_direction = mlx->player.turn_direction;
    reset_movement_flags(mlx);
    mlx->player.rotation_angle += tmp_turn_direction * mlx->player.rotation_speed;
    move_step = tmp_walk_direction * mlx->player.move_speed;
    strafe_step = tmp_strafe_direction * mlx->player.move_speed;
    update_position(mlx, move_step, strafe_step);
    if (does_hit_right_bottom_wall(mlx, mlx->player.p_x, mlx->player.p_y)
        || does_hit_left_top_wall(mlx, mlx->player.p_x, mlx->player.p_y))
    {
        mlx->player.p_x = old_x;
        mlx->player.p_y = old_y;
    }
}

// int apply_shading(int base_color, float shading_factor)
// {
//     int r;
//     int g;
//     int b;
//     int result;

//     r = (base_color >> 16) & 0xFF;
//     g = (base_color >> 8) & 0xFF;
//     b = base_color & 0xFF;

//     r = (int)(r * shading_factor);
//     g = (int)(g * shading_factor);
//     b = (int)(b * shading_factor);

//     if (r > 255) r = 255;
//     if (g > 255) g = 255;
//     if (b > 255) b = 255;
//     if (r < 0) r = 0;
//     if (g < 0) g = 0;
//     if (b < 0) b = 0;

//     result = (r << 16) | (g << 8) | b;
//     return result;
// }

// uint32_t	get_wall_color(t_ray ray)
// {
//     if (ray.wall_face == NORTH_FACE)
//         return (0xCC0000);
//     else if (ray.wall_face == SOUTH_FACE)
//         return (0x00CC00);
//     else if (ray.wall_face == EAST_FACE)
//         return (0x0000CC);
//     else if (ray.wall_face == WEST_FACE)
//         return (0xCCCC00);
//     return (0xFFFFFF);
// }

// void clearColorBuffer(t_mlx *mlx, uint32_t color) {
//     int x;

//     x = 0;
//     while (x < WINDOW_WIDTH)
//     {
//         int y = 0;
//         while (y < WINDOW_HEIGHT)
//         {
//             ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + x] = color;
//             y++;
//         }
//         x++;
//     }
// }


// void draw_line_3D_helper(t_mlx *mlx, int x, int start_y, int end_y, int color) // return to this function after finishing
// {
//     if (start_y > end_y)
//         return;

//     int y = start_y;
//     while (y < end_y)
//     {
//         my_mlx_pixel_put(&mlx->img, x, y, color);
//         y++;
//     }
// }

// void cpy_pixel(t_img *src, t_img *dst, int src_x, int src_y, int dst_x, int dst_y)
// {
//     char *dst_px;
//     char *src_px;

//     // if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
//     //     return;
//     dst_px = dst->addr + (dst_y * dst->line_length + dst_x * (dst->bits_per_pixel / 8));
//     src_px = src->addr + (src_y * src->line_length + src_x * (src->bits_per_pixel / 8));
//     *(unsigned int*)dst_px = *(unsigned int *)src_px;
// }


// int get_pixel_color(t_tex *tex, int x, int y)
// {
//     int index;

//     if (x < 0) x = 0;
//     if (y < 0) y = 0;
//     if (x >= tex->width) x = tex->width - 1;
//     if (y >= tex->height) y = tex->height - 1;
    
//     index = (int)(y * tex->width + x);
//     if (index < 0 || index >= (tex->width * tex->height))
//         return 0;
        
//     return tex->texture[index];
// }

// void renderColorBuffer(t_mlx *mlx) {
//     mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img.img, 0, 0);
// }

float calculate_perp_distance(t_ray *ray, float player_angle)
{
    float perp_distance = ray->distance * cos(ray->ray_angle - player_angle);
    printf("ray_distance = %f\n", ray->distance);
    printf("player_angle = %f\n", player_angle);

    return perp_distance > 0.001f ? perp_distance : 0.001f;
}

// void render_3D_projection_walls(t_mlx *mlx)
// {
//     int i = 0;
//     while (i < mlx->player.number_of_rays) 
//     {
//         t_ray *ray = &mlx->player.rays[i];
        
//         // Calculate the perpendicular distance to avoid fisheye effect
//         float perpDistance = calculate_perp_distance(ray, mlx->player.rotation_angle);
        
//         // Calculate projected wall height
//         float distanceProjPlane = (WINDOW_WIDTH / 2.0f) / tan(mlx->player.fov / 2.0f);
//         float projectedWallHeight = (TILE_SIZE / perpDistance) * distanceProjPlane;
        
//         // Calculate wall strip boundaries
//         int wallStripHeight = (int)projectedWallHeight;
//         int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
//         int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        
//         // Clamp values
//         wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
//         wallBottomPixel = wallBottomPixel >= WINDOW_HEIGHT ? WINDOW_HEIGHT - 1 : wallBottomPixel;

//         // Draw ceiling
//         int y = 0;
//         while (y < wallTopPixel) {
//             ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + i] = 
//                 (mlx->maps.ceiling_rgb.r << 16) | 
//                 (mlx->maps.ceiling_rgb.g << 8) | 
//                 mlx->maps.ceiling_rgb.b;
//             y++;
//         }

//         // Determine texture X coordinate (where exactly on the wall we hit)
//         float wallHitX = ray->wall_hit_x;
//         float wallHitY = ray->wall_hit_y;
//         float textureOffsetX;

//         if (ray->was_hit_vertical) {
//             textureOffsetX = (int)(wallHitY) % TILE_SIZE;
//         } else {
//             textureOffsetX = (int)(wallHitX) % TILE_SIZE;
//         }
        
//         // Scale texture offset to fit texture size
//         textureOffsetX = textureOffsetX * 64.0f / TILE_SIZE;

//         // Select texture based on wall face
//         int texNum = ray->wall_face;
//         t_tex *texture = &mlx->tex[texNum];

//         // Draw the wall slice
//         float step = 64.0f / (float)wallStripHeight;  // Using fixed texture height of 64
//         float texPos = 0;
//         if (wallStripHeight > WINDOW_HEIGHT) {
//             texPos = (wallStripHeight - WINDOW_HEIGHT) / 2.0f * step;
//         }
//         y = wallTopPixel;
//         while (y < wallBottomPixel) 
//         {
//             int textureOffsetY = (int)texPos & 63;  // Keep within 64 pixel bounds
//             texPos += step;

//             // Get color from texture
//             int texX = (int)textureOffsetX & 63;
//             uint32_t texelColor = texture->texture[64 * textureOffsetY + texX];
            
//             // Apply distance shading
//             float shade = 1.0f - (perpDistance / (WINDOW_WIDTH * 0.8f));
//             shade = fmax(0.3f, shade);  // Limit minimum brightness
            
//             uint32_t r = ((texelColor >> 16) & 0xFF) * shade;
//             uint32_t g = ((texelColor >> 8) & 0xFF) * shade;
//             uint32_t b = (texelColor & 0xFF) * shade;
            
//             texelColor = (r << 16) | (g << 8) | b;
            
//             ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + i] = texelColor;
//             y++;
//         }
//         // printf("rgb === %d\n",mlx->maps.floor_rgb.b);
//         // Draw floor
//         y = wallBottomPixel;
//         while (y < WINDOW_HEIGHT) {
//             ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + i] = 
//                 (mlx->maps.floor_rgb.r << 16) | 
//                 (mlx->maps.floor_rgb.g << 8) | 
//                 mlx->maps.floor_rgb.b;
//                 y++;
//         }
//         i++;
//     }
// }

static void	init_wall_data(t_wall_data *data, t_mlx *mlx, t_ray *ray)
{
    data->perp_distance = calculate_perp_distance(ray, mlx->player.rotation_angle);
    data->proj_plane = (WINDOW_WIDTH / 2.0f) / tan(mlx->player.fov / 2.0f);
    data->proj_height = (TILE_SIZE / data->perp_distance) * data->proj_plane;
    data->strip_height = (int)data->proj_height;
    data->top_pixel = (WINDOW_HEIGHT / 2) - (data->strip_height / 2);
    data->bottom_pixel = (WINDOW_HEIGHT / 2) + (data->strip_height / 2);
    if (data->top_pixel < 0)
        data->top_pixel = 0;
    if (data->bottom_pixel >= WINDOW_HEIGHT)
        data->bottom_pixel = WINDOW_HEIGHT - 1;
}

static void	draw_ceiling(t_mlx *mlx, int top_pixel, int ray_index)
{
    int	y;

    y = 0;
    while (y < top_pixel)
    {
        ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + ray_index]
            = (mlx->maps.ceiling_rgb.r << 16)
            | (mlx->maps.ceiling_rgb.g << 8)
            | mlx->maps.ceiling_rgb.b;
        y++;
    }
}

static void	draw_floor(t_mlx *mlx, int bottom_pixel, int ray_index)
{
    int	y;

    y = bottom_pixel;
    while (y < WINDOW_HEIGHT)
    {
        ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + ray_index]
            = (mlx->maps.floor_rgb.r << 16)
            | (mlx->maps.floor_rgb.g << 8)
            | mlx->maps.floor_rgb.b;
        y++;
    }
}

static uint32_t	apply_shade(uint32_t color, float shade)
{
    uint32_t	r;
    uint32_t	g;
    uint32_t	b;

    r = ((color >> 16) & 0xFF) * shade;
    g = ((color >> 8) & 0xFF) * shade;
    b = (color & 0xFF) * shade;
    return ((r << 16) | (g << 8) | b);
}

static void	draw_wall_slice(t_mlx *mlx, t_wall_data *data,
    t_ray *ray, int ray_index)
{
    int			y;
    int			tex_y;
    uint32_t	color;
    float		shade;
    t_tex		*texture;

    texture = &mlx->tex[ray->wall_face];
    data->tex_x = ray->was_hit_vertical ? (int)(ray->wall_hit_y)
        % TILE_SIZE : (int)(ray->wall_hit_x) % TILE_SIZE;
    data->tex_x = data->tex_x * 64.0f / TILE_SIZE;
    data->step = 64.0f / (float)data->strip_height;
    data->tex_pos = (data->strip_height > WINDOW_HEIGHT)
        ? (data->strip_height - WINDOW_HEIGHT) / 2.0f * data->step : 0;
    y = data->top_pixel;
    while (y < data->bottom_pixel)
    {
        tex_y = (int)data->tex_pos & 63;
        data->tex_pos += data->step;
        color = texture->texture[64 * tex_y + ((int)data->tex_x & 63)];
        shade = fmax(0.3f, 1.0f - (data->perp_distance / (WINDOW_WIDTH * 0.8f)));
        color = apply_shade(color, shade);
        ((uint32_t *)mlx->img.addr)[(WINDOW_WIDTH * y) + ray_index] = color;
        y++;
    }
}

void	render_3D_projection_walls(t_mlx *mlx)
{
    int			i;
    t_wall_data	wall;
    t_ray		*ray;

    i = 0;
    while (i < mlx->player.number_of_rays)
    {
        ray = &mlx->player.rays[i];
        init_wall_data(&wall, mlx, ray);
        draw_ceiling(mlx, wall.top_pixel, i);
        draw_wall_slice(mlx, &wall, ray, i);
        draw_floor(mlx, wall.bottom_pixel, i);
        i++;
    }
}

// void cast(t_mlx *mlx, t_ray *ray)
// {
//     float ray_dir_x = cos(ray->ray_angle);
//     float ray_dir_y = sin(ray->ray_angle);
    
//     float pos_x = mlx->player.p_x / TILE_SIZE;
//     float pos_y = mlx->player.p_y / TILE_SIZE;
    
//     int map_x = (int)pos_x;
//     int map_y = (int)pos_y;
    
//     float delta_dist_x = fabs(1.0f / ray_dir_x);
//     float delta_dist_y = fabs(1.0f / ray_dir_y);

//     int step_x;
//     int step_y;
//     float side_dist_x;
//     float side_dist_y;
    
//     // Calculate step and initial side_dist
//     if (ray_dir_x < 0) {
//         step_x = -1;
//         side_dist_x = (pos_x - map_x) * delta_dist_x;
//     } else {
//         step_x = 1;
//         side_dist_x = (map_x + 1.0f - pos_x) * delta_dist_x;
//     }
//     if (ray_dir_y < 0) {
//         step_y = -1;
//         side_dist_y = (pos_y - map_y) * delta_dist_y;
//     } else {
//         step_y = 1;
//         side_dist_y = (map_y + 1.0f - pos_y) * delta_dist_y;
//     }
    
//     // DDA Algorithm
//     int hit = 0;
//     int side = 0; // 0 for x-side, 1 for y-side
    
//     while (hit == 0) {
//         if (side_dist_x < side_dist_y) {
//             side_dist_x += delta_dist_x;
//             map_x += step_x;
//             side = 0;
//         } else {
//             side_dist_y += delta_dist_y;
//             map_y += step_y;
//             side = 1;
//         }
        
//         // Check if ray has hit a wall
//         if (map_x < 0 || map_y < 0 || map_x >= mlx->maps.width || map_y >= mlx->maps.height) {
//             hit = 1;
//             ray->distance = FLT_MAX;
//             return;
//         }
//         if (mlx->maps.map[map_y][map_x] == '1') {
//             hit = 1;
//         }
//     }
    
//     // Calculate exact hit position and distance
//     if (side == 0) {
//         ray->distance = (map_x - pos_x + (1 - step_x) / 2) / ray_dir_x;
//         ray->wall_hit_x = mlx->player.p_x + ray->distance * TILE_SIZE * ray_dir_x;
//         ray->wall_hit_y = mlx->player.p_y + ray->distance * TILE_SIZE * ray_dir_y;
//         ray->was_hit_vertical = true;
//         ray->wall_face = (step_x > 0) ? EAST_FACE : WEST_FACE;
//     } else {
//         ray->distance = (map_y - pos_y + (1 - step_y) / 2) / ray_dir_y;
//         ray->wall_hit_x = mlx->player.p_x + ray->distance * TILE_SIZE * ray_dir_x;
//         ray->wall_hit_y = mlx->player.p_y + ray->distance * TILE_SIZE * ray_dir_y;
//         ray->was_hit_vertical = false;
//         ray->wall_face = (step_y > 0) ? SOUTH_FACE : NORTH_FACE;
//     }
    
//     ray->distance *= TILE_SIZE; // Convert to pixel distance
// }
static void	init_ray_steps(float pos_x, float pos_y,
				t_ray_calc *calc)
{
	if (calc->ray_dir_x < 0)
	{
		calc->step_x = -1;
		calc->side_dist_x = (pos_x - calc->map_x) * calc->delta_dist_x;
	}
	else
	{
		calc->step_x = 1;
		calc->side_dist_x = (calc->map_x + 1.0f - pos_x) * calc->delta_dist_x;
	}
	if (calc->ray_dir_y < 0)
	{
		calc->step_y = -1;
		calc->side_dist_y = (pos_y - calc->map_y) * calc->delta_dist_y;
	}
	else
	{
		calc->step_y = 1;
		calc->side_dist_y = (calc->map_y + 1.0f - pos_y) * calc->delta_dist_y;
	}
}

static void	calculate_hit_position(t_ray *ray, t_mlx *mlx, t_ray_calc *calc, int side)
{
	if (side == 0)
	{
		ray->distance = (calc->map_x - calc->pos_x + (1 - calc->step_x) / 2)
			/ calc->ray_dir_x;
		ray->wall_hit_x = mlx->player.p_x + ray->distance * TILE_SIZE
			* calc->ray_dir_x;
		ray->wall_hit_y = mlx->player.p_y + ray->distance * TILE_SIZE
			* calc->ray_dir_y;
		ray->was_hit_vertical = true;
		ray->wall_face = (calc->step_x > 0) ? EAST_FACE : WEST_FACE;
	}
	else
	{
		ray->distance = (calc->map_y - calc->pos_y + (1 - calc->step_y) / 2)
			/ calc->ray_dir_y;
		ray->wall_hit_x = mlx->player.p_x + ray->distance * TILE_SIZE
			* calc->ray_dir_x;
		ray->wall_hit_y = mlx->player.p_y + ray->distance * TILE_SIZE
			* calc->ray_dir_y;
		ray->was_hit_vertical = false;
		ray->wall_face = (calc->step_y > 0) ? SOUTH_FACE : NORTH_FACE;
	}
	ray->distance *= TILE_SIZE;
}

static int	perform_dda(t_mlx *mlx, t_ray_calc *calc, int *side)
{
	while (1)
	{
		if (calc->side_dist_x < calc->side_dist_y)
		{
			calc->side_dist_x += calc->delta_dist_x;
			calc->map_x += calc->step_x;
			*side = 0;
		}
		else
		{
			calc->side_dist_y += calc->delta_dist_y;
			calc->map_y += calc->step_y;
			*side = 1;
		}
		if (calc->map_x < 0 || calc->map_y < 0
			|| calc->map_x >= mlx->maps.width
			|| calc->map_y >= mlx->maps.height)
			return (0);
		if (mlx->maps.map[calc->map_y][calc->map_x] == '1')
			return (1);
	}
}

void	cast(t_mlx *mlx, t_ray *ray)
{
	t_ray_calc	calc;
	int			side;

	calc.ray_dir_x = cos(ray->ray_angle);
	calc.ray_dir_y = sin(ray->ray_angle);
	calc.pos_x = mlx->player.p_x / TILE_SIZE;
	calc.pos_y = mlx->player.p_y / TILE_SIZE;
	calc.map_x = (int)calc.pos_x;
	calc.map_y = (int)calc.pos_y;
	calc.delta_dist_x = fabs(1.0f / calc.ray_dir_x);
	calc.delta_dist_y = fabs(1.0f / calc.ray_dir_y);
	init_ray_steps(calc.pos_x, calc.pos_y, &calc);
	if (!perform_dda(mlx, &calc, &side))
	{
		ray->distance = FLT_MAX;
		return ;
	}
	calculate_hit_position(ray, mlx, &calc, side);
}
void adjusting_rays(t_mlx *mlx)
{
    int column;

    column = 0;
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

void player_center_position(t_mlx *mlx, int x, int y)
{
    mlx->player.p_x = x * TILE_SIZE + (TILE_SIZE / 2);
    mlx->player.p_y = y * TILE_SIZE + (TILE_SIZE / 2);
}

void render_all(t_mlx *mlx)
{
    adjusting_rays(mlx);
    render_3D_projection_walls(mlx);
}