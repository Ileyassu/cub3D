#include "../lib.h"

void	destroy_game(t_map *map)
{
	if (map->no_texture)
		free(map->no_texture);
	if (map->so_texture)
		free(map->so_texture);
	if (map->we_texture)
		free(map->we_texture);
	if (map->ea_texture)
		free(map->ea_texture);
	if (map->floor_color)
		free(map->floor_color);
	if (map->ceiling_color)
		free(map->ceiling_color);
	if (map->map)
		free2d(map->map);
}

void	error_print(char *str, t_map *map)
{
	// printf(RED "Error\n" RST);
	printf("%s\n", str);
	destroy_game(map);
	exit(1);
}

void	map_initializer(t_map *map)
{
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->floor_color = NULL;
	map->ceiling_color = NULL;
	map->map = NULL;
	map->player.dir = 0;
	map->player.p_x = -1;
	map->player.p_y = -1;
	map->floor_rgb.r = -1;
	map->floor_rgb.g = -1;
	map->floor_rgb.b = -1;
	map->ceiling_rgb.r = -1;
	map->ceiling_rgb.g = -1;
	map->ceiling_rgb.b = -1;
}

bool	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (true);
	return (false);
}

char	*ft_substr_plus(char *s)
{
	int		i;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && !ft_isspace(s[i]))
		i++;
	while (s[i] && ft_isspace(s[i]))
		i++;
	str = ft_substr(s, i, ft_strlen(s));
	free(s);
	return (str);
}
