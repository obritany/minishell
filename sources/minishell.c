#include "minishell.h"

void	ft_init(t_data **data)
{
	if (!(*(data)))
		*data = (t_data *)malloc(sizeof(t_data));
	if (*data == NULL)
	{
		write(2, strerror(errno), sizeof(strerror(errno)));
		exit(1);
	}
	(*data)->line = NULL;
	(*data)->cmd = NULL;
	(*data)->argc = 0;
	(*data)->argv = NULL;
	(*data)->len = 0;
	(*data)->fd0 = 0;
	(*data)->fd1 = 1;
	(*data)->flg.opwd = 1;
}

void	ft_shell_prompt(void)
{
	write(2, "\033[1;34mminishell-> \033[0m", 23);
}

static void	ft_input(t_data *data)
{
	char	str[3];
	int		len;

	while (1)
	{	
		ft_set_term(1);
		len = read(0, str, 3);
		if (g_sig == 1)
			ft_line_handler(data, str, len, 3);
		if (str[0] == '\e' || !ft_strncmp(str, "\177", len))
			ft_termios(data, str, len);
		else if (str[0] != '\t' && str[0] != '\4')
			ft_line(data, str, len);
		if (str[0] == '\n')
			line_tool(data, 1);
		if (str[0] == '\4' && (data->len == 0 || g_sig == 1))
		{
			line_tool(data, 2);
			break ;
		}
	}
}

int	main(int argc, char **argv, char *envp[])
{
	t_data	*data;
	t_list	*env;
	t_hist	hist;
	t_pl	pl;

	if (argc != 1 && !argv[0])
		return (1);
	env = envp_to_lst(envp);
	init_hist(&hist, env);
	ft_init(&data);
	data->env = env;
	set_var(&data->env, "OLDPWD", 0);
	data->hist = &hist;
	data->pl = &pl;
	data->pl->state = 0;
	data->pl->count = 0;
	data->flg.n = 0;
	g_sig = 0;
	ft_shell_prompt();
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	ft_input(data);
	return (0);
}
