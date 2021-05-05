#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <term.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <signal.h>

#define STDIN	0
#define STDOUT	1
#define RD	0
#define WR	1

typedef struct s_var
{
	char	*key;
	char	*value;
}				t_var;

typedef struct s_hist
{
	char	*file;
	int		size;
	int		pos;
	char	**cmds;
}				t_hist;

typedef struct	s_pl
{
	int fdin[2];
	int fdout[2];
	int pids[100];
	int pnum;
}				t_pl;


t_list	*envp_to_lst(char *envp[]);
void	print_env(t_list *env);
void	print_export(t_list *env);
char	*get_var(t_list *env, char *str);
int 	set_var(t_list **begin, char *str, int n);
int		unset_var(t_list **begin, char *str);

int		read_file(char *file, char ***arr);
int		write_str(char *file, char *str, int mode);

int		init_hist(t_hist *hist);
int		add_hist(t_hist *hist, char *str);

//DEBUG
void	test_env(t_list **env);
void	test_fork();
void	test_pipe();
void	test_dup();
void	test_signal();
void	test_exe_pipe();

#endif