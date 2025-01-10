void render_3D_projection_walls(t_mlx *mlx)
{
    int i = 0;
    float wall_strip_width = WINDOW_WIDTH / mlx->player.number_of_rays;

    // Define colors for each face
    uint32_t north_face = 0xFF0000;  // Red
    uint32_t south_face = 0x00FF00;  // Green
    uint32_t east_face = 0x0000FF;   // Blue
    uint32_t west_face = 0xFFFF00;   // Yellow

    while (i < mlx->player.number_of_rays)
    {
        t_ray ray = mlx->player.rays[i];
        float distance = ray.distance * cos(ray.ray_angle - mlx->player.rotation_angle);
        float distance_projection_plane = (WINDOW_WIDTH / 2) / tan(mlx->player.fov / 2);
        float wall_strip_height = (TILE_SIZE / distance) * distance_projection_plane;

        float wall_top_pixel = (WINDOW_HEIGHT / 2) - (wall_strip_height / 2);
        float wall_bottom_pixel = (WINDOW_HEIGHT / 2) + (wall_strip_height / 2);

        if (wall_top_pixel < 0)
            wall_top_pixel = 0;
        if (wall_bottom_pixel > WINDOW_HEIGHT)
            wall_bottom_pixel = WINDOW_HEIGHT;

        int x = i * wall_strip_width;
        float shading_factor = 1.0f / (1.0f + (distance * 0.01f));
        uint32_t wall_color;

        // Draw ceiling
        draw_line_3D_helper(mlx, x, 0, wall_top_pixel, 0x87CEEB);

        // Determine wall face based on grid hit
        if (ray.was_hit_vertical)
        {
            if (ray.is_ray_facing_right)
                wall_color = east_face;  // East face
            else
                wall_color = west_face;  // West face
        }
        else
        {
            if (ray.is_ray_facing_down)
                wall_color = south_face;  // South face
            else
                wall_color = north_face;  // North face
        }

        // Apply shading
        int shaded_color = apply_shading(wall_color, shading_factor);

        // Draw wall strip
        draw_line_3D_helper(mlx, x, wall_top_pixel, wall_bottom_pixel, shaded_color);

        // Draw floor
        draw_line_3D_helper(mlx, x, wall_bottom_pixel, WINDOW_HEIGHT, 0x8B4513);

        i++;
    }
}