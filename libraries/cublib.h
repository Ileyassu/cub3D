#ifndef CUB3D
#define CUB3D

#include <stdio.h>
#include <unistd.h>
#include "../mlx-linux/mlx.h"
#include <math.h>
#include <stdlib.h>

typedef struct s_player
{
    int p_x;
    int p_y;
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

#endif