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
	int i;
	int id = fork();	// below code execute in two thread
	
	if (!id)
	{
		// Child thread
		printf("id - %d, pid - %d, ppid - %d\n", id, getpid(), getppid());
		
		i = 0;
		while(i < 5)
		{
			printf("%d ", i++);
			fflush(stdout);		// immediate write
		}
		execve("a.out", 0, environ);	// auto exit after exe work done
	}
	else
	{
		// Parent thread
		printf("id - %d, pid - %d, ppid - %d\n", id, getpid(), getppid());
		
		int *rslt = malloc(sizeof(int));
		wait(rslt);		// wait child thread and put exe work result in rslt
		printf("rslt = %d\n", *rslt);

		i = 5;
		while(i < 10)
		{
			printf("%d ", i++);
			fflush(stdout);		// immediate write
		}
	}
	printf("Fork test done!\n");
}

void	test_pipe()
{
	int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int arrSize = sizeof(arr) / sizeof(int);
	int start, end;
	
	int fd[2];		// 0 - for read, 1 - for write
	// int fd2[2];		// use two fd's for two way communication
	pipe(fd);

	int id = fork();
	if (!id)
	{
		start = 0;
		end = arrSize / 2; 
	}
	else
	{
		start = arrSize / 2;
		end = arrSize;; 
	}

	int sum = 0;
	int i = start;
	while (i < end)
		sum += arr[i++];

	printf("Sum in id %d = %d\n", id, sum);

	if (!id)
	{
		close(fd[0]);	// close readable fd because only write here
		printf("Writing start \n");
		write(fd[1], &sum, sizeof(int));
		printf("Writing done \n");
		close(fd[1]);
	}
	else
	{
		close(fd[1]);	// close writable fd because only write here
		int sumFromChild;
		printf("Reading start \n");
		read(fd[0], &sumFromChild, sizeof(int));	// read wait data from child
		printf("Reading done\n");
		close(fd[0]);

		int totalSum = sum + sumFromChild;
		printf("Total sum in id %d = %d\n", id, totalSum);
		wait(0);	// always wait child before exit
	}
}