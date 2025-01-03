
#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		size;

	size = ft_strlen(s);
	str = (char *)malloc((size + 1) * sizeof(char));
	if (!str)
		return (0);
	str[size] = '\0';
	while (size--)
		str[size] = s[size];
	return (str);
}
