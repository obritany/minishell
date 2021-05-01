#include "minishell.h"

void	print_env(t_list *env)
{
	while (env)
	{
		if (((t_var *)(env->content))->value == 0)
		{
			env = env->next;
			continue ;
		}
		ft_putstr_fd(((t_var *)(env->content))->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(((t_var *)(env->content))->value, 1);
		ft_putstr_fd("\n", 1);
		env = env->next;
	}
}

void	print_export(t_list *env)
{
	char	**envp;
	char	*value;
	t_list	*temp;
	int		i;

	envp = 0;
	temp = env;
	while (temp)
	{
		ft_arradd(&envp, ((t_var *)temp->content)->key);
		temp = temp->next;
	}
	ft_arrsort(&envp);
	i = 0;
	while (envp[i])
	{
		value = get_var(env, envp[i]);
		if (value)
			printf("declare -x %s=\"%s\"\n", envp[i], value);
		else
			printf("declare -x %s\n", envp[i]);
		i++;
	}
	ft_arrclear(envp);
}
