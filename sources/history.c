#include "minishell.h"

int	read_file(char *file, char ***arr)
{
	int	fd;
	int	size;

	fd = open(file, O_RDWR);
	if (fd == -1)
		return (-1);
	size = get_lines(fd, arr);
	close(fd);
	return (size);
}

int	write_str(char *file, char *str, int mode)
{
	int	fd;

	fd = open(file, O_CREAT | O_RDWR | mode, S_IRWXU);
	if (fd == -1)
		return (-1);
	ft_putstr_fd(str, fd);
	close(fd);
	return (0);
}

int	add_hist(t_hist *hist, char *str)
{
	if (ft_arradd(&hist->cmds, str))
		return (1);
	hist->size++;
	hist->pos = hist->size;
	write_str(hist->file, str, O_APPEND);
	write_str(hist->file, "\n", O_APPEND);
	return (0);
}

int	init_hist(t_hist *hist, t_list *env)
{
	char	*temp;
	char	buf[4096];

	temp = ft_strdup(get_var(env, "TMPDIR"));
	if (temp == 0)
		temp = ft_strdup(getcwd(buf, 4096));
	hist->file = ft_strjoin(temp, "/.minishell_history");
	free(temp);
	if (hist->file == 0)
		return (1);
	write_str(hist->file, "", O_APPEND);
	hist->size = read_file(hist->file, &(hist->cmds)) - 1;
	free(hist->cmds[hist->size]);
	hist->cmds[hist->size] = 0;
	hist->pos = hist->size;
	return (0);
}
