#include "../lib.h"

void	parse_rgb(t_map *map)
{
	char	**split;
	int		i;

	split = ft_split(map->floor_color, ',');
	i = 0;
	while (split[i])
		i++;
	if (i != 3)
		error_print("Invalid color format", map);
	map->floor_rgb.r = ft_atomic_atoi(split[0]);
	map->floor_rgb.g = ft_atomic_atoi(split[1]);
	map->floor_rgb.b = ft_atomic_atoi(split[2]);
	free2d(split);
	split = ft_split(map->ceiling_color, ',');
	i = 0;
	while (split[i])
		i++;
	if (i != 3)
		error_print("Invalid color format", map);
	map->ceiling_rgb.r = ft_atomic_atoi(split[0]);
	map->ceiling_rgb.g = ft_atomic_atoi(split[1]);
	map->ceiling_rgb.b = ft_atomic_atoi(split[2]);
	free2d(split);
}

void	parse_textures(t_map *map)
{
	map->no_texture = ft_substr_plus(map->no_texture);
	map->so_texture = ft_substr_plus(map->so_texture);
	map->we_texture = ft_substr_plus(map->we_texture);
	map->ea_texture = ft_substr_plus(map->ea_texture);
	if (!check_path(map->no_texture) || !check_path(map->so_texture)
		|| !check_path(map->we_texture) || !check_path(map->ea_texture))
		error_print("Invalid path", map);
}

void    file_parser(int fd, t_map *map)
{
    char    *line;
    while (1)
    {
        char *s = get_next_line(fd);
        if (!s)
            break ;
        line = ft_strtrim(s, "\n");
        free(s);
        if (!line)
            break ;
        if (!map->no_texture || !map->so_texture || !map->we_texture
            || !map->ea_texture)
            save_textures(line, map);
        else if (!map->floor_color || !map->ceiling_color)
            save_colors(line, map);
        free(line);
        if (map->no_texture && map->so_texture && map->we_texture
            && map->ea_texture && map->floor_color && map->ceiling_color)
            break ;
    }
    if (!map->no_texture || !map->so_texture || !map->we_texture
        || !map->ea_texture || !map->floor_color || !map->ceiling_color)
        error_print("Missing Textures/Color, or map empty", map);
}

char *ft_strjoin_gnl(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	free(s1);
	return (str);
}

void	map_parser(int fd, t_map *map)
{
	char	*line;
	char	*map1d;
	int		i;
	i	= 0; 
	map1d = ft_strdup("");
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		map1d = ft_strjoin_gnl(map1d, line);
		free(line);
	}
	if (!check_map(map1d))
		error_print("Invalid map", map);
	map->map = ft_split(map1d, '\n');
  map->height = 0;
    while (map->map[map->height])
        map->height++;
    map->width = 0;
    while (i < map->height)
    {
        int line_length = ft_strlen(map->map[i]);
        if (line_length > map->width)
            map->width = line_length;
		i++;
    }
	save_player(map);
	free(map1d);
}
