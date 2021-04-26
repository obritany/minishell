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

void	test_fork()
{
	extern char	**environ;
	int id = fork();
	int i;
	
	if (!id)
	{
		printf("id - %d, pid - %d, ppid - %d\n", id, getpid(), getppid());
		
		i = 0;
		while(i < 5)
		{
			printf("%d ", i++);
			fflush(stdout);
		}
		execve("a.out", 0, environ);
	}
	else
	{
		printf("id - %d, pid - %d, ppid - %d\n", id, getpid(), getppid());
		int *rslt = malloc(sizeof(int));
		wait(rslt);
		printf("rslt = %d\n", *rslt);

		i = 5;
		while(i < 10)
		{
			printf("%d ", i++);
			fflush(stdout);
		}
	}
	printf("Fork test done!\n");
}