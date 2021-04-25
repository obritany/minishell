#include "libft.h"

int		ft_arrsort(char ***arr)
{
	int		i;
	char	*temp;

	i = 0;
	while ((*arr)[i + 1])
	{
		if (ft_strcmp((*arr)[i], (*arr)[i + 1]) > 0)
		{
			temp = (*arr)[i];
			(*arr)[i] = (*arr)[i + 1];
			(*arr)[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	return (0);
}