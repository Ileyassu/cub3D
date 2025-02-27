
#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (*little == '\0')
		return ((char *)big);
	while (little[j] != '\0')
		j++;
	while (i < len && big[i] != '\0' && (i + j) <= len)
	{
		if (big[i] == little[0] && ft_strncmp(&big[i], little, j) == 0)
		{
			return ((char *)&big[i]);
		}
		i++;
	}
	return (0);
}
