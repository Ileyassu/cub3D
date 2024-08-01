#include <mlx.h>
#include <stdio.h>
#include <stdlib.h>

int key_press(int keycode, void *param)
{
    (void)param;
    printf("Key pressed: %d\n", keycode);
    if (keycode == 53) // ESC key
    {
        exit(0);
    }
    return 0;
}

int main()
{
    void *mlx;
    void *win;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 500, 500, "Keycode Checker");
    mlx_key_hook(win, key_press, NULL);
    mlx_loop(mlx);
    return 0;
}