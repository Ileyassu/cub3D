#ifndef CUB3D
#define CUB3D

#include <stdio.h>
#include <unistd.h>
#include "../mlx-linux/mlx.h"
#include <math.h>
#include <stdlib.h>

extern char map[6][6];

typedef struct s_player
{
    int p_x;
    int p_y;
    int up;
    int down;
    int left;
    int right;
}  t_player;

typedef struct s_img 
{
    void *img;
    char *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;

} t_img;
typedef struct s_mlx
{
    t_player *player;
    t_img *img;
    void *mlx;
    void *mlx_win;
}   t_mlx;

void find_player_pos(t_mlx *mlx);
void init_image(t_mlx *mlx);
void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color);
void draw_map(t_mlx *mlx);
void draw_player(t_mlx *mlx, int size);
void draw_scene (t_mlx *mlx);
int move_player(unsigned int key, void *ptr);

#endif