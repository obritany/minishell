#include "minishell.h"

int		read_file(char *file, char ***arr)
{
	int		fd;
	int		size;

	fd = open(file, O_RDWR);
	if (fd == -1)
		return (-1);
	size = get_lines(fd, arr);
	close(fd);
	return (size);
}

int		write_str(char *file, char *str, int mode)		// mode = O_APPEND, O_TRUNC
{
	int	fd;

	fd = open(file, O_CREAT | O_RDWR | mode, S_IRWXU);
	if (fd == -1)
		return (-1);
	ft_putstr_fd(str, fd);
	close(fd);
	return (0);
}

int		add_hist(t_hist *hist, char *str)
{
	if (ft_arradd(&hist->cmds, str))
		return (1);
	hist->size++;
	hist->pos++;
	write_str(hist->file, "\n", O_APPEND);
	write_str(hist->file, str, O_APPEND);
	return (0);
}