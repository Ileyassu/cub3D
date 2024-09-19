#include "../../libraries/cublib.h"

void find_player_pos(t_mlx *mlx)
{
    int x = 0;
    int y = 0;
    int cell_size = 500 / 6;
    while(x < 6)
    {
        y = 0;
        while(y < 6)
        {
            if(map[x][y] == 'N')
            {
                mlx->player->p_x = cell_size * x;
                mlx->player->p_y = cell_size * y;
            }
            y++;
        }
        x++;
    }
}