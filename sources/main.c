#include "minishell.h"

int		main(int argc, char *argv[], char *envp[])
{
	char	str[100];				// буфер для разных символов (1 - анг, 2 - рус, 3 - стрелочки)
	int		len;
	struct	termios term;
	t_list	*env;
	t_hist	hist;

	env = envp_to_lst(envp);
	hist.size = read_file("minishell_history", &(hist.cmds));
	hist.pos = hist.size;

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
		if (!ft_strncmp(str, "\e[A", len))			// up
		{
			if (hist.pos <= 0)
				continue ;
			tputs(restore_cursor, 1, &ft_putchar);
			tputs(delete_line, 1, ft_putchar);
			ft_putstr_fd(hist.cmds[--hist.pos], 1);
		}
		else if (!ft_strncmp(str, "\e[B", len))		// down
		{
			tputs(restore_cursor, 1, &ft_putchar);
			tputs(delete_line, 1, ft_putchar);
			if (hist.pos < hist.size)
				ft_putstr_fd(hist.cmds[++hist.pos], 1);
		}
		else if (!ft_strncmp(str, "\177", len))		// backspace
		{
			tputs(cursor_left, 1, ft_putchar);
			tputs(delete_character, 1, ft_putchar);
		}
		else if (!ft_strncmp(str, "\4", len))		// ctrl+d
		{
			add_hist(&hist, "command_5");
			add_hist(&hist, "command_6");
			add_hist(&hist, "command_7");
			// break ;
		}
		else
		{
			write(1, str, len);
		}
	}

	// Восстанавливаем настройки при выходе
	term.c_lflag |= ECHO;
	term.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &term);
	return(0);
}
