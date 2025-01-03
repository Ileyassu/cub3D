#include "../lib.h"
                              
void	set_texture(char **texture, char *line, t_map *map)
{
	if (!*texture)
		*texture = ft_strdup(line);
	else
		error_print("Duplicate texture", map);
}

void	save_textures(char *line, t_map *map)
{
	if (!line || !ft_strlen(line))
		return ;
	if (ft_strnstr(line, "NO ", 3))
		set_texture(&map->no_texture, line, map);
	else if (ft_strnstr(line, "SO ", 3))
		set_texture(&map->so_texture, line, map);
	else if (ft_strnstr(line, "WE ", 3))
		set_texture(&map->we_texture, line, map);
	else if (ft_strnstr(line, "EA ", 3))
		set_texture(&map->ea_texture, line, map);
	else
		error_print("Invalid texture", map);
}

void	save_colors(char *line, t_map *map)
{
	if (!line || !ft_strlen(line))
		return ;
	if (ft_strnstr(line, "F ", 2))
	{
		if (!map->floor_color)
			map->floor_color = ft_strdup(line);
		else
			error_print("Duplicate floor color", map);
	}
	else if (ft_strnstr(line, "C ", 2))
	{
		if (!map->ceiling_color)
			map->ceiling_color = ft_strdup(line);
		else
			error_print("Duplicate ceiling color", map);
	}
	else
		error_print("Invalid color", map);
}

void	save_player(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (map->map[i])
	{
		j = 0;
		while (map->map[i][j])
		{
			if (map->map[i][j] == 'N' || map->map[i][j] == 'S'
				|| map->map[i][j] == 'W' || map->map[i][j] == 'E')
			{
				if (map->player.dir)
					error_print("Duplicate player", map);
				map->player.dir = map->map[i][j];
				map->player.p_x = j;
				map->player.p_y = i;
			}
			j++;
		}
		i++;
	}
	if (!map->player.dir)
		error_print("Player not found", map);
}

void	start_parsing(char *av, t_map *map)
{
	int	fd;

	if (!av || !ft_strlen(av))
	{
		printf("Error\nNo file specified\n");
		exit(1);
	}
	if (!check_file_extension(av))
	{
		printf("Error\nInvalid file extension\n");
		exit(1);
	}
	fd = open(av, O_RDONLY);
	if (fd < 0)
	{
		printf("Error\nCan't open the file\n");
		exit(1);
	}
	map_initializer(map);
	file_parser(fd, map);
	parse_textures(map);
	parse_colors(map);
	map_parser(fd, map);
	close(fd);
}
