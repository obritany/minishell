#include "libft.h"

char	**ft_arrdup(char **arr)
{
	int		i;
	char	**dest;

	if (!arr)
		return (0);
	i = 0;
	while (arr[i])
		i++;
	dest = malloc(sizeof(char*) * (i + 1));
	i = 0;
	while (arr[i])
	{
		dest[i] = ft_strdup(arr[i]);
		i++;
	}
	dest[i] = 0;
	return (dest);
}