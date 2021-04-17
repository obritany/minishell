#include "minishell.h"

typedef struct s_var
{
	char	*key;
	char	*value;
}				t_var;

void	print_env(t_list *env)
{
	while (env)
	{
		ft_putstr_fd(((t_var*)(env->content))->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(((t_var*)(env->content))->value, 1);
		ft_putstr_fd("\n", 1);
		env = env->next;
	}
}

int 	add_var(t_list **begin, char *str, int n)
{
	t_list	*temp;
	char	**key_val;

	key_val = ft_split(str, '=');
	temp = *begin;
	while (temp)
	{
		if (!ft_strcmp(((t_var*)temp->content)->key, key_val[0]))
		{
			free(((t_var*)(temp->content))->key);
			free(((t_var*)(temp->content))->value);
			((t_var*)(temp->content))->key = key_val[0];
			((t_var*)(temp->content))->value = key_val[1];
			free(key_val);
			return (0);
		}
		temp = temp->next;
	}
	temp = ft_lstnew(malloc(sizeof(t_var)));
	((t_var*)(temp->content))->key = key_val[0];
	((t_var*)(temp->content))->value = key_val[1];
	ft_lstadd_at(begin, temp, n);
	free(key_val);
	return (0);
}

int		del_var(t_list **begin, char *str)
{
	t_list	*temp;
	t_list	*prev;

	prev = 0;
	temp = *begin;
	while (temp)
	{
		if (!ft_strcmp(((t_var*)temp->content)->key, str))
		{
			if (prev)
				prev->next = temp->next;
			else
				*begin = temp->next;
			free(((t_var*)(temp->content))->key);
			free(((t_var*)(temp->content))->value);
			free(temp->content);
			free(temp);
			return (0);
		}
		prev = temp;
		temp = temp->next;
	}
	return (0);
}

t_list	*envp_to_lst(char *envp[])
{
	int		i;
	t_list	*begin;

	if (!envp)
		return (0);
	i = 0;
	begin = 0;
	while (envp[i])
	{
		add_var(&begin, envp[i], ft_lstsize(begin));
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

	print_env(env);
	printf("\n");

	add_var(&env, "a=a", ft_lstsize(env) - 1);
	print_env(env);
	printf("\n");

	add_var(&env, "b=b", ft_lstsize(env) - 1);
	print_env(env);
	printf("\n");

	add_var(&env, "c=c", ft_lstsize(env) - 1);
	print_env(env);
	printf("\n");

	add_var(&env, "b=bbb", ft_lstsize(env) - 1);
	print_env(env);
	printf("\n");

	del_var(&env, "b");
	print_env(env);
	printf("\n");

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
