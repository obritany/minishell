#include "libft.h"

int	ft_arradd(char ***arr, char *str)
{
	char	*newstr;
	int		i;

	i = 0;
	if (*arr)
		while ((*arr)[i])
			i++;
	*arr = ft_realloc(*arr, sizeof(char*) * (i + 2));
	if (*arr == 0)
		return (1);
	newstr = ft_strdup(str);
	if (!newstr)
		return (1);
	(*arr)[i] = newstr;
	(*arr)[i + 1] = 0;
	return (0);
}