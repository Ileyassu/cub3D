#include "../../libraries/cublib.h"

void find_player_pos(t_mlx *mlx)
{
    int x = 0;
    int y = 0;
    int cell_size = 500 / 6; // Assuming your map is 6x6 and each cell is equally sized
    int player_size = 10; // Same size you used when drawing the player

    while (x < 6)
    {
        y = 0;
        while (y < 6)
        {
            if (map[x][y] == 'N')
            {
                // Calculate the center of the cell for the player
                mlx->player->p_x = (cell_size * y) + (cell_size / 2) - (player_size / 2);
                mlx->player->p_y = (cell_size * x) + (cell_size / 2) - (player_size / 2);
                return;
            }
            y++;
        }
        x++;
    }
}