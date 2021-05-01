#include "minishell.h"

char	*get_var(t_list *env, char *str)
{
	while (env)
	{
		if (!ft_strcmp(((t_var *)env->content)->key, str))
			return (((t_var *)env->content)->value);
		env = env->next;
	}
	return (0);
}

int	add_var(t_list **begin, char **key_val, int n)
{
	t_list	*new;

	new = ft_lstnew(malloc(sizeof(t_var)));
	if (new == 0)
		return (3);
	((t_var *)(new->content))->key = key_val[0];
	((t_var *)(new->content))->value = key_val[1];
	ft_lstadd_at(begin, new, n);
	free(key_val);
	return (0);
}

int	set_var(t_list **begin, char *str, int n)
{
	t_list	*temp;
	char	**key_val;

	key_val = ft_split(str, '=');
	if (key_val == 0)
		return (1);
	if (ft_strchr(str, '=') && !key_val[1])
		key_val[1] = ft_strdup("");
	if (ft_strchr(str, '=') && !key_val[1])
		return (2);
	temp = *begin;
	while (temp)
	{
		if (!ft_strcmp(((t_var *)temp->content)->key, key_val[0]))
		{
			free(((t_var *)(temp->content))->key);
			free(((t_var *)(temp->content))->value);
			((t_var *)(temp->content))->key = key_val[0];
			((t_var *)(temp->content))->value = key_val[1];
			free(key_val);
			return (0);
		}
		temp = temp->next;
	}
	return (add_var(begin, key_val, n));
}

int	unset_var(t_list **begin, char *str)
{
	t_list	*temp;
	t_list	*prev;

	prev = 0;
	temp = *begin;
	while (temp)
	{
		if (!ft_strcmp(((t_var *)temp->content)->key, str))
		{
			if (prev)
				prev->next = temp->next;
			else
				*begin = temp->next;
			free(((t_var *)(temp->content))->key);
			free(((t_var *)(temp->content))->value);
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
		if (set_var(&begin, envp[i], ft_lstsize(begin)))
			return (0);
		i++;
	}
	return (begin);
}
