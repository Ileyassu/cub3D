#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <unistd.h>
#include "mlx/mlx.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define TILE_SIZE 32
#define MAP_WIDTH 7
#define MAP_HEIGHT 7
#define WINDOW_SIZE 7 * 32

extern char *map[7];

typedef struct s_player
{
    float p_x;
    float p_y;
    float size;
} t_player;
typedef struct s_img 
{
    void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
} t_img;
typedef struct s_mlx 
{
    void *mlx;
    void *win;
    t_img img;
    t_player player;
} t_mlx  ;


void player_position(t_mlx *mlx, int x, int y);
void init_player(t_mlx *mlx);
void draw_player(t_mlx *mlx);
void my_mlx_pixel_put(t_img *img, int x, int y, int color);

#endif