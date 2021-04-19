#include "minishell.h"

void	test_env(t_list *env)
{
	print_env(env, 0);
	printf("\n");

	set_var(&env, "a=a a a", ft_lstsize(env) - 1);
	print_env(env, 0);
	printf("\n");

	set_var(&env, "b=bbb", ft_lstsize(env) - 1);
	print_env(env, 0);
	printf("\n");

	set_var(&env, "c=", ft_lstsize(env) - 1);
	print_env(env, 0);
	printf("\n");

	set_var(&env, "b=b", ft_lstsize(env) - 1);
	print_env(env, 0);
	printf("\n");
	
	unset_var(&env, "b");
	print_env(env, 0);
	printf("\n");

	printf("%s\n", get_var(env, "a"));
	printf("%s\n", get_var(env, "b"));
	printf("%s\n", get_var(env, "c"));
	printf("\n");

	print_env(env, 1);
}

int		main(int argc, char *argv[], char *envp[])
{
	char	str[100];				// буфер для разных символов (1 - анг, 2 - рус, 3 - стрелочки)
	int		len;
	struct	termios term;
	t_list	*env;

	env = envp_to_lst(envp);
	// test_env(env);

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
