#include "libft.h"

int	ft_putarr(char **arr)
{
	int	i;

	if (!arr)
		return (1);
	i = 0;
	while (arr[i])
	{
		ft_putstr_fd(arr[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}