#include "minishell.h"

void	child_sig_handler(int sig)
{
	if (g_sig != 2)
	{
		if (sig == SIGINT)
			write(2, "\n", 1);
		if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
	}
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\033[1;34m\nminishell-> \033[0m", 24);
		g_sig = 1;
	}
}
