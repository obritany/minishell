#include "minishell.h"

int		read_file(char *file, char ***arr)
{
	char	*line;
	int		fd;
	int 	i;

	fd = open(file, O_RDWR);
	if (fd == -1)
		return (-1);
	i = 0;
	while (get_next_line(fd, &line))
	{
		ft_arradd(arr, line);
		free(line);
		i++;
	}
	free(line);
	return (i);
}

int		write_str(char *file, char *line, int mode)		// mode = O_APPEND, O_TRUNC
{
	int	fd;

	fd = open(file, O_CREAT | O_RDWR | mode, S_IRWXU);
	if (fd == -1)
		return (-1);
	ft_putstr_fd(line, fd);
	close(fd);
	return (0);
}

int		add_hist(t_hist *hist, char *str)
{
	if (ft_arradd(&hist->cmds, str))
		return (1);
	hist->size++;
	hist->pos++;
	write_str("minishell_history", str, O_APPEND);
	write_str("minishell_history", "\n", O_APPEND);
	return (0);
}