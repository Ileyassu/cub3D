#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <unistd.h>
#include "mlx/mlx.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <fcntl.h>
#include "libft/libft.h"
#include <stdint.h>
#define RED 0x00FF0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define TILE_SIZE 64
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define EPSILON 0.0001
#define M_PI       3.14159265358979323846
#define NORTH_FACE 0
#define SOUTH_FACE 1
#define EAST_FACE  2
#define WEST_FACE  3
#define NORTH_WALL_COLOR 0xCC0000
#define SOUTH_WALL_COLOR 0x00CC00
#define EAST_WALL_COLOR  0x0000CC
#define WEST_WALL_COLOR  0xCCCC00
// extern char *map[7];


typedef struct s_pos 
{
    double x;
    double y;
}   t_pos;

typedef struct s_texture {
    char    *addr;
    int     len;
    int     bpp;
} t_texture;


typedef struct	s_tex
{
	char		*tex_path;
	uint32_t			*texture;
	int		width;
	int		height;
    char    *addr;
    int     len;
    int     bpp;
}				t_tex;

typedef struct s_ray 
{
    int id;
    float ray_angle;
    float wall_hit_x;
    float wall_hit_y;
    int foundHorzWallHit;
    int foundVerticalHit;
    int horizontal_wall_content;
    int vertical_wall_content;
    float horizontal_wall_hit_x;
    float horizontal_wall_hit_y;
    float vertical_wall_hit_x;
    float vertical_wall_hit_y;
    bool was_hit_vertical;
    bool was_hit_horizontal;
    float vertical_distance;
    float distance;
    float horizontal_distance;
    int is_ray_facing_up;
    int is_ray_facing_down;
    int is_ray_facing_right;
    int is_ray_facing_left;
    int wall_hit_content;
    float next_vertical_touch_x;
    float next_vertical_touch_y;
    float next_horizontal_touch_x;
    float next_horizontal_touch_y;
    int wall_face;      // For texture selection (NORTH/SOUTH/EAST/WEST)
    double wall_x;      // Exact hit position for texture mapping
    int tex_x;          // X coordinate on the texture
    int line_height;    // Height of wall slice
    double perp_wall_dist; // Perpendicular distance to wall
} t_ray;

typedef struct s_player
{
    float p_x;
    float p_y;
    float size;
    int   dir;
    float radius;
    float turn_direction;
    float walk_direction;
    float rotation_angle;
    float move_speed;
    float rotation_speed;
    float fov;
    float wall_strip_width;
    float wall_strip_height;
    int   number_of_rays;
    float start_column_angle;
    t_ray *rays;
} t_player;


typedef struct s_rgb
{
    int r;
    int g;
    int b;
} t_rgb;

typedef struct s_img 
{
    void	*img;
    int		img_width;
	int		img_height;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
} t_img;

//parsing oualid
typedef struct s_map
{
    char    *no_texture;
    char    *so_texture;
    char    *we_texture;
    char    *ea_texture;
    char    *floor_color;
    char   *ceiling_color;
    char	**map;
    int     height;
    int     width;
    int     td_map_size;
    struct s_player player;
    struct s_rgb    floor_rgb;
    struct s_rgb    ceiling_rgb;
} t_map;


typedef struct s_mlx 
{
    void *mlx;
    void *win;
    t_img img;
    t_map maps;
    t_tex tex[4];
    t_player player;
} t_mlx  ;

void player_position(t_mlx *mlx);
void init_player(t_mlx *mlx);
void draw_player(t_mlx *mlx);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
void draw_line (t_mlx *mlx);
void update_player(t_mlx *mlx);
int does_hit_right_Bottom_wall(t_mlx *mlx, int x, int y);
void player_center_position(t_mlx *mlx, int x, int y);
void render_3D_projection_walls(t_mlx *mlx);
void draw_line_3D_helper(t_mlx *mlx, int x, int start_y, int end_y, int color);
void render_all(t_mlx *mlx);


//functions parsing oualid
void	start_parsing(char *av, t_map *map);
void	map_initializer(t_map *map);
void	error_print(char *str, t_map *map);
void	file_parser(int fd, t_map *map);
void	parse_textures(t_map *map);
void	save_textures(char *line, t_map *map);
void	map_parser(int fd, t_map *map);
void	parse_colors(t_map *map);
bool	check_file_extension(char *av);
void	free2d(char **str);
bool	ft_isspace(char c);
char	*ft_substr_plus(char *s);
bool	is_all_whitespace(char *str);
bool	check_newline(char *map);
bool	check_comma(char *str);
bool	check_rgb(t_rgb rgb);
void	parse_rgb(t_map *map);
bool    check_map(char *map);
long	ft_atomic_atoi(char *str);
bool    check_path(char *path);
void	save_colors(char *line, t_map *map);
void    save_player(t_map *map);
// void print_map(t_map *maps);
void cleanup_textures(t_mlx *mlx);
void cleanup_mlx(t_mlx *mlx);
void cleanup_map(t_map *map);
// loading textures 
void	load_texture(t_mlx *mlx);

#endif