#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>	//////DEBUG//////
# include "../libft/libft.h"
# include <term.h>

typedef struct s_var
{
	char	*key;
	char	*value;
}				t_var;

t_list	*envp_to_lst(char *envp[]);
void	print_env(t_list *env, int mode);
char	*get_var(t_list *env, char *str);
int 	set_var(t_list **begin, char *str, int n);
int		unset_var(t_list **begin, char *str);

#endif