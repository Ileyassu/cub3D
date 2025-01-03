#include "../lib.h"

bool	check_file_extension(char *av)
{
	int	i;

	i = ft_strlen(av) - 4;
	if (i < 0)
		return (false);
	if (!ft_strncmp(av + i, ".cub", 4))
		return (true);
	return (false);
}

bool	check_path(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

bool	check_comma(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == ',')
			count++;
		i++;
	}
	if (count != 2)
		return (false);
	return (true);
}

bool	check_rgb(t_rgb rgb)
{
	if (rgb.r < 0 || rgb.r > 255 || rgb.g < 0 || rgb.g > 255 || rgb.b < 0
		|| rgb.b > 255)
		return (false);
	return (true);
}

bool	check_newline(char *map)
{
	int	i;

	i = 0;
	while (map[i] && map[i] == '\n')
		i++;
	while (map[i])
	{
		if (map[i] == '\n' && map[i + 1] == '\n')
			return (false);
		i++;
	}
	return (true);
}
