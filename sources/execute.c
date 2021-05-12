#include "minishell.h"

void	ft_execute(t_data *data, char *file)
{
	if (!(strncmp(data->argv[0], "./minishell", 11)))
		g_sig = 2;
	signal(SIGINT, child_sig_handler);
	signal(SIGQUIT, child_sig_handler);
	if (!fork())
	{
		if (data->fd0 != STDIN)
			dup2(data->fd0, STDIN);
		if (data->fd1 != STDOUT)
			dup2(data->fd1, STDOUT);
		data->ret = execve(file, data->argv, env_to_arr(data->env));
		if (data->ret == -1 || errno == EACCES)
		{
			data->ret = 127;
			exit(data->ret);
		}
	}
	wait(&data->ret);
	if (g_sig == 2)
		g_sig = 0;
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	if (WIFEXITED(data->ret))
		data->ret = WEXITSTATUS(data->ret);
	free(file);
}

int	execute_pipe(t_data *data, char *file, int builtin)
{
	int	pid;

	data->pl->fdin[RD] = data->pl->fdout[RD];
	data->pl->fdin[WR] = data->pl->fdout[WR];
	if (data->pl->state > 0)
		pipe(data->pl->fdout);
	pid = fork();
	if (pid == 0)
		pipe_fork(data, file, builtin);
	if (data->pl->state > 0)
		close(data->pl->fdout[WR]);
	if (data->pl->state > 1 || data->pl->state < 0)
		close(data->pl->fdin[RD]);
	free (file);
	return (pid);
}

void	ft_binsearch(t_data *data, int cnt, char *dir, char *file)
{
	char		**paths;
	struct stat	buf[4096];

	paths = ft_split_path(data);
	if (ft_strlen(data->argv[0]) == 0)
		return ;
	if (data->argv[0][0] == '.' || data->argv[0][0] == '/')
		ft_abs_path(data, &file);
	else if (paths)
	{
		while (paths[++cnt])
		{
			dir = ft_strjoin(paths[cnt], "/");
			file = ft_strjoin(dir, data->argv[0]);
			free(dir);
			data->ret = stat(file, buf);
			if (!data->ret)
				break ;
			free(file);
			file = NULL;
		}
		ft_exectool(data, &paths, &file);
	}
}

int	is_cmd_bltin(t_data *data)
{
	if (((!(ft_strncmpul(data->argv[0], "echo", 4))) && !data->argv[0][4]) || \
		((!(ft_strncmpul(data->argv[0], "cd", 2))) && !data->argv[0][2]) || \
		((!(ft_strncmpul(data->argv[0], "pwd", 3))) && !data->argv[0][3]) || \
		((!(ft_strncmpul(data->argv[0], "export", 6))) && \
		!data->argv[0][6] && data->argc == 1) || \
		((!(ft_strncmpul(data->argv[0], "export", 6))) && !data->argv[0][6]) || \
		((!(ft_strncmpul(data->argv[0], "unset", 5))) && !data->argv[0][5]) || \
		((!(ft_strncmpul(data->argv[0], "env", 3))) && !data->argv[0][3]) || \
		((!(ft_strncmpul(data->argv[0], "exit", 4))) && !data->argv[0][4]))
		return (1);
	return (0);
}

int	ft_sorter(t_data *data)
{
	int	i;

	i = 1;
	if ((!(ft_strncmpul(data->argv[0], "echo", 4))) && !data->argv[0][4])
		ft_echo(data, 1, 0);
	else if ((!(ft_strncmpul(data->argv[0], "cd", 2))) && !data->argv[0][2])
		ft_cd(data, 0, NULL, NULL);
	else if ((!(ft_strncmpul(data->argv[0], "pwd", 3))) && !data->argv[0][3])
		ft_pwd(data);
	else if ((!(ft_strncmpul(data->argv[0], "export", 6))) && \
		!data->argv[0][6] && (data->argc == 1 || ft_strlen(data->argv[1]) == 0))
		print_export(data->env);
	else if ((!(ft_strncmpul(data->argv[0], "export", 6))) && !data->argv[0][6])
		while (data->argv[i])
			set_var(&data->env, data->argv[i++], 0);
	else if ((!(ft_strncmpul(data->argv[0], "unset", 5))) && !data->argv[0][5])
		while (data->argv[i])
			unset_var(&data->env, data->argv[i++]);
	else if ((!(ft_strncmpul(data->argv[0], "env", 3))) && !data->argv[0][3])
		print_env(data->env);
	else if ((!(ft_strncmpul(data->argv[0], "exit", 4))) && !data->argv[0][4])
		ft_exit(data, -1);
	else
		ft_binsearch(data, -1, NULL, NULL);
	return (ft_free_array(data->argv));
}
