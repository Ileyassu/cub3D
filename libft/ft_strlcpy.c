
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	src_len;

	i = 0;
	src_len = 0;
	if (size > 0)
	{
		while ((src[i] != '\0') && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	src_len = ft_strlen(src);
	return (src_len);
}
