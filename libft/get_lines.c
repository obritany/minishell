#include "libft.h"

int		get_lines(int fd, char ***arr)
{
	int 	i;
	char	*line;
	int		rslt;

	i = 0;
	while (1)
	{
		rslt = get_next_line(fd, &line);
		if (rslt == -1)
		{
			ft_arrclear(*arr);
			return (-1);
		}
		ft_arradd(arr, line);
		free(line);
		i++;
		if (rslt == 0)
			return (i);
	}
}