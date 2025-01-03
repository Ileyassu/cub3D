
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ss;

	ss = (unsigned char *)s;
	while (n-- > 0)
		*ss++ = 0;
}
