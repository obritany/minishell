#include "minishell.h"

int copy_array(char **dst[], char **src[])
{
	int		i;

	if (!(*src))
		return (1);
	i = 0;
	while ((*src)[i])
		i++;
	*dst = malloc(sizeof(char*) * (i + 1));
	i = 0;
	while ((*src)[i])
	{
		(*dst)[i] = ft_strdup((*src)[i]);
		i++;
	}
	(*dst)[i] = 0;
	return (0);
}

int	print_array(char **array[])
{
	int	i;

	if (!(*array))
		return (1);
	i = 0;
	while ((*array)[i])
	{
		ft_putstr_fd((*array)[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

int	set_key_val(char **array[], char *str)
{
	int		i;
	char 	**key_val;
	char 	**temp_key_val;

	if (!(*array))
		return (1);
	key_val = ft_split(str, '=');
	i = 0;
	while ((*array)[i])
	{
		temp_key_val = ft_split((*array)[i], '=');
		if (!ft_strcmp(temp_key_val[0], key_val[0]))
		{
			free((*array)[i]);
			(*array)[i] = ft_strdup(str);
		}
		ft_clear(temp_key_val);
		i++;
	}
	ft_clear(key_val);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	str[100];				// буфер для разных символов (1 - анг, 2 - рус, 3 - стрелочки)
	int		len;
	struct	termios term;
	char	**env;

	copy_array(&env, &envp);
	print_array(&env);
	set_key_val(&env, "abc=iop");
	print_array(&env);

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
