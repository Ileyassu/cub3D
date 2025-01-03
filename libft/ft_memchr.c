
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ss;

	ss = (const unsigned char *)s;
	while (n-- > 0)
	{
		if (*ss == (unsigned char)c)
			return ((void *)ss);
		ss++;
	}
	return (NULL);
}
