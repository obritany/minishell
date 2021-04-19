#include "minishell.h"

void	print_env(t_list *env, int mode)		// mode_0 - env, mode_1 - export
{
	while (env)
	{
		if (mode == 1 && !ft_strcmp(((t_var*)env->content)->key, "_"))
		{
			env = env->next;
			continue ;
		}
		if (mode == 1)
			ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(((t_var*)(env->content))->key, 1);
		ft_putstr_fd("=", 1);
		if (mode == 1)
			ft_putstr_fd("\"", 1);
		ft_putstr_fd(((t_var*)(env->content))->value, 1);
		if (mode == 1)
			ft_putstr_fd("\"", 1);
		ft_putstr_fd("\n", 1);
		env = env->next;
	}
}

char	*get_var(t_list *env, char *str)
{
	while (env)
	{
		if (!ft_strcmp(((t_var*)env->content)->key, str))
			return (((t_var*)env->content)->value);
		env = env->next;
	}
	return (0);
}

int 	set_var(t_list **begin, char *str, int n)
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

int		unset_var(t_list **begin, char *str)
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
		set_var(&begin, envp[i], ft_lstsize(begin));
		i++;
	}
	return (begin);
}