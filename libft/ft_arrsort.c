#include "libft.h"

void	change(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int		ft_arrsort(char ***arr)
{
	int	i;

	i = 0;
	while (*arr)
	{
		if (ft_strcmp((*arr)[i], (*arr)[i + 1]) > 0)
		{
			change(&(*arr)[i], &(*arr)[i + 1]);
			i = 0;
		}
		else
			i++;
	}
	return (0);
}