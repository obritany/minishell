#include "minishell.h"

void	test_env(t_list **env)
{
	printf("\n");

	set_var(env, "__________A", ft_lstsize(*env) - 1);
	set_var(env, "__________B=", ft_lstsize(*env) - 1);
	set_var(env, "__________C=C", ft_lstsize(*env) - 1);

	print_env(*env);
	printf("\n");
	print_export(*env);

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
		// Child thread
		close(fd[0]);	// close readable fd because only write here
		printf("Writing start \n");
		write(fd[1], &sum, sizeof(int));
		printf("Writing done \n");
		close(fd[1]);
	}
	else
	{
		// Parent thread
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


void	test_dup()
{
	int id = fork();

	if (id != 0)
	{
		// Parent thread
		wait(0);
		printf("You CAN see PARENT it in STDOUT\n");
		return ;
	}

	// Child thread
	int file = open("redirect", O_CREAT | O_RDWR, S_IRWXU);

	printf("You CAN see CHILD it in STDOUT\n");

	int stddup = dup(1);			// makes a copy of STDOUT fd
	close(1);		// we don't use original fd, only a copy

	int filedup = dup2(file, 1);	// filedup fd is a copy of file fd, and now replace STDOUT
	close(file);	// we don't use original fd, only a copy

	printf("You CAN'T see CHILD it in STDOUT\n");
	close(filedup);	// close fd after print done

	int stddup2 = dup2(stddup, 1);	// return STDOUT in child thread, in parent thread STDOUT didn't change at all
	close(stddup);	// we don't use original fd, only a copy

	printf("You CAN see CHILD it in STDOUT\n");
}


int x = 0;	// use global varible for status check
void	handle_sigusr1(int sig)
{
	if (x != 0)
		return ;
	printf("You can make it...\n");
	fflush(stdout);		// immediate write
}
void	test_signal()
{
	int pid = fork();

	if (pid == 0)
	{
		// Child thread
		sleep(3);
		kill(getppid(), SIGUSR1);	// send user signal to parent id
	}
	else
	{
		// Parent thread
		signal(SIGUSR1, &handle_sigusr1);	// attach handle function to signal
		
		printf("What is the result of 2 * 5?\n");
		scanf("%d", &x);
		kill(pid, SIGKILL);	// kill child thread if got answer

		if (x == 10)
			printf("Right!\n");
		else
			printf("Wrong!\n");

		wait(0);
	}
}



void	ft_exe_pipe(char *file, char *argv[], int fdin[], int fdout[])
{
	int pid = fork();
	if (pid == 0)
	{
		// Child thread
		if (fdout != 0)
		{
			dup2(fdout[WR], STDOUT);
			close(fdout[RD]);
			close(fdout[WR]);
		}
		if (fdin != 0)
		{
			dup2(fdin[RD], STDIN);
			close(fdin[RD]);
			close(fdin[WR]);
		}
		exit(execve(file, argv, 0));
	}
	// Parent thread
	if (fdout != 0)
		close(fdout[WR]);
	if (fdin != 0)
		close(fdin[RD]);
	wait(0);
}
void	test_exe_pipe()
{
	int fds[2][2];

	pipe(fds[0]);

	char *argv1[] = {"ls", "-l", 0};
	ft_exe_pipe("/bin/ls", argv1, 0, fds[0]);

	pipe(fds[1]);

	char *argv2[] = {"grep", "env", 0};
	ft_exe_pipe("/usr/bin/grep", argv2, fds[0], fds[1]);

	pipe(fds[0]);

	char *argv3[] = {"grep", ".c", 0};
	ft_exe_pipe("/usr/bin/grep", argv3, fds[1], fds[0]);

	char *argv4[] = {"wc", 0};
	ft_exe_pipe("/usr/bin/wc", argv4, fds[0], 0);
}