#include "../lib.h"

bool	check_valid_chars(char *map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (!ft_strchr(" \n01NEWS", map[i]))
			return (false);
		i++;
	}
	return (true);
}

int ft_str2dlen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	check_none_one_surr(char **map2d, int i, int j)
{
	if (i == 0 || i == ft_str2dlen(map2d) - 1 || j == 0
		|| j == (int)ft_strlen(map2d[i]) - 1)
		return (false);
	if (i > 0)
	{
		if (j >= (int)ft_strlen(map2d[i - 1]) || ft_isspace(map2d[i - 1][j]))
			return (false);
	}
	if (i < ft_str2dlen(map2d) - 1)
	{
		if (j > (int)ft_strlen(map2d[i + 1]) || ft_isspace(map2d[i + 1][j]))
			return (false);
	}
	if (j > 0 && ft_isspace(map2d[i][j - 1]))
		return (false);
	if (j < (int)ft_strlen(map2d[i]) - 1 && ft_isspace(map2d[i][j + 1]))
		return (false);
	return (true);
}

bool	check_valid_map(char **map2d)
{
	int	i;
	int	j;

	i = 0;
	while (map2d[i])
	{
		j = 0;
		if (is_all_whitespace(map2d[i]))
			return (free2d(map2d), false);
		while (map2d[i][j])
		{
			if (ft_strchr("0NEWS", map2d[i][j]))
			{
				if (!check_none_one_surr(map2d, i, j))
					return (free2d(map2d), false);
			}
			j++;
		}
		i++;
	}
	return (free2d(map2d), true);
}

bool	check_map(char *map)
{
	if (!map || !ft_strlen(map))
		return (false);
	if (!check_newline(map))
		return (false);
	if (!check_valid_chars(map))
		return (false);
	if (!check_valid_map(ft_split(map, '\n')))
		return (false);
	return (true);
}

void	parse_colors(t_map *map)
{
	map->floor_color = ft_substr_plus(map->floor_color);
	map->ceiling_color = ft_substr_plus(map->ceiling_color);
	if (!check_comma(map->floor_color) || !check_comma(map->ceiling_color))
		error_print("Invalid color format", map);
	parse_rgb(map);
	if (!check_rgb(map->floor_rgb) || !check_rgb(map->ceiling_rgb))
		error_print("Invalid color range", map);
}
