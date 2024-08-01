#ifndef CUB3D
#define CUB3D

#include <stdio.h>
#include <unistd.h>
#include <mlx.h>
#include <math.h>
#include <stdlib.h>

typedef struct s_player
{
    int p_x;
    int p_y;
}  t_player;

typedef struct s_mlx
{
    t_player *player;
    void *mlx;
    void *mlx_win;
}   t_mlx;

#endif