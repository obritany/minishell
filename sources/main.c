#include "minishell.h"

typedef struct s_var
{
	char	*key;
	char	*value;
}				t_var;

void	print_content(void *content)
{
	ft_putstr_fd(((t_var*)content)->key, 1);
	ft_putstr_fd("=", 1);
	ft_putstr_fd(((t_var*)content)->value, 1);
	ft_putstr_fd("\n", 1);
}

int 	add_env(t_list **begin, char *str)
{
	ft_lstadd_back(begin, ft_lstnew(malloc(sizeof(t_var))));
	last = ft_lstlast(begin);
	middle = ft_strchr(envp[i], '=');
	*middle = '\0';
	((t_var*)(last->content))->key = ft_strdup(envp[i]);
	((t_var*)(last->content))->value = ft_strdup(middle + 1);
	*middle = '=';
	return (0);
}

t_list	*envp_to_lst(char *envp[])
{
	int		i;
	t_list	*begin;
	t_list	*last;
	char	*middle;

	if (!envp)
		return (0);
	i = 0;
	begin = 0;
	while (envp[i])
	{
		add_env(&begin, envp[i]);
		i++;
	}
	return (begin);
}

int		main(int argc, char *argv[], char *envp[])
{
	char	str[100];				// буфер для разных символов (1 - анг, 2 - рус, 3 - стрелочки)
	int		len;
	struct	termios term;
	t_list	*env;

	env = envp_to_lst(envp);
	ft_lstiter(env, print_content);

	tcgetattr(0,&term);
	term.c_lflag &= ~ECHO;			// отключаем вывод служебных символов
	term.c_lflag &= ~ICANON;		// переходим в неканонический посимвольный режим ввода
	tcsetattr(0, TCSANOW, &term);	// применяем настройки
	tgetent(0, getenv("TERM"));		// загружаем базу данных текущего терминала

	tputs(save_cursor, 1, &ft_putchar);
	str[0] = '0';
	while (1)
	{
		len = read(0, str, 100);
		if (!ft_strncmp(str, "\e[A", len))
		{
			tputs(restore_cursor, 1, &ft_putchar);
			tputs(delete_line, 1, ft_putchar);
			ft_putstr_fd("previous", 1);
		}
		else if (!ft_strncmp(str, "\e[B", len))
		{
			tputs(restore_cursor, 1, &ft_putchar);
			tputs(delete_line, 1, ft_putchar);
			ft_putstr_fd("next", 1);
		}
		else if (!ft_strncmp(str, "\177", len))
		{
			tputs(cursor_left, 1, ft_putchar);
			tputs(delete_character, 1, ft_putchar);
		}
		else if (!ft_strncmp(str, "\4", len))
		{
			break ;
		}
		else
			write(1, str, len);
	}

	// Восстанавливаем настройки при выходе
	term.c_lflag |= ECHO;
	term.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &term);
	return(0);
}
