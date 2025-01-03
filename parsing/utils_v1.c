#include "../lib.h"

int	ft_atoi_helper(char *str, int *i, long *res)
{
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		if (*res > LLONG_MAX / 10)
			return (1);
		*res = ((*res) * 10) + (str[*i] - 48);
		(*i)++;
	}
	return (0);
}

long	ft_atomic_atoi(char *str)
{
	long	res;
	int		i;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			return (2147483650);
		i++;
	}
	if (str[i] < '0' || str[i] > '9')
		return (2147483650);
	if (ft_atoi_helper(str, &i, &res))
		return (2147483650);
	if (str[i] != '\0')
		return (2147483650);
	return (res * sign);
}

void	free2d(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

bool	is_all_whitespace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}
