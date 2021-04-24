#include "minishell.h"

void	test_env(t_list **env)
{
	print_env(*env);
	printf("\n");

	set_var(env, "a=a a a", ft_lstsize(*env) - 1);
	print_env(*env);
	printf("\n");

	set_var(env, "b=bbb", ft_lstsize(*env) - 1);
	print_env(*env);
	printf("\n");

	set_var(env, "c=", ft_lstsize(*env) - 1);
	print_env(*env);
	printf("\n");

	set_var(env, "b=b", ft_lstsize(*env) - 1);
	print_env(*env);
	printf("\n");
	
	unset_var(env, "b");
	print_env(*env);
	printf("\n");

	printf("%s\n", get_var(*env, "a"));
	printf("%s\n", get_var(*env, "b"));
	printf("%s\n", get_var(*env, "c"));
	printf("\n");
}