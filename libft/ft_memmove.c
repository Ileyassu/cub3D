
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*destination;
	unsigned char	*source;

	if (!dest && !src)
		return (NULL);
	if (dest == src)
		return ((void *)src);
	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	if (source < destination)
	{
		while (n > 0)
		{
			destination[n - 1] = source[n - 1];
			n--;
		}
	}
	else
	{
		while (n-- > 0)
			*destination++ = *source++;
	}
	return (dest);
}
