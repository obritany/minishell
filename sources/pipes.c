#include "minishell.h"

#define STDIN	0
#define STDOUT	1
#define RD	0
#define WR	1

char *files[] = {"/usr/bin/yes", "/usr/bin/head", "/usr/bin/wc"};
char *argv[3][100] = {{"yes", 0}, {"head", 0}, {"wc", 0}};

// char *files[] = {"/bin/ls", "/usr/bin/grep", "/usr/bin/wc"};
// char *argv[100][100] = {{"ls", 0}, {"grep", "env", 0}, {"wc", 0}};

// 0 0 - pipes = 0
// 0 1 - pipes = 1
// 1 1 - pipes = 2,3,4,5
// 1 0 - pipes = -1

int	ft_execute(char *file, char *argv[], int fdin[], int fdout[], int pipes)
{
	int pid = fork();
	if (pid == 0)
	{
		// Child thread
		if (pipes > 0)					// fdout != 0
		{
			dup2(fdout[WR], STDOUT);
			close(fdout[RD]);
		}
		if (pipes > 1 || pipes == -1)	// fdin != 0
			dup2(fdin[RD], STDIN);
		execve(file, argv, 0);
	}
	// Parent thread
	if (pipes > 0)					// fdout != 0
		close(fdout[WR]);
	if (pipes > 1 || pipes == -1)	// fdin != 0
		close(fdin[RD]);
	return (pid);
}

int		main()
{
	int fds[2][2];
	int pipes = 0;
	int pids[100];

	int i = 0;
	while (i < 3)
	{
		pipes++;
		if (i == 2)
			pipes = -1;		// last command

		fds[0][0] = fds[1][0];
		fds[0][1] = fds[1][1];
		if (pipes > 0)
			pipe(fds[1]);

		pids[i] = ft_execute(files[i], argv[i], fds[0], fds[1], pipes);
		i++;
	}

	waitpid(pids[0], 0, 0);
	waitpid(pids[1], 0, 0);
	waitpid(pids[2], 0, 0);
	return (0);
}




// void	ft_execute(char *file, char *argv[], int fdin[], int fdout[])
// {
// 	int pid = fork();
// 	if (pid == 0)
// 	{
// 		// Child thread
// 		if (fdout != 0)
// 			dup2(fdout[WR], STDOUT);
// 		if (fdin != 0)
// 			dup2(fdin[RD], STDIN);
// 		execve(file, argv, 0);
// 	}
// 	// Parent thread
// 	if (fdout != 0)
// 		close(fdout[WR]);
// 	if (fdin != 0)
// 		close(fdin[RD]);
// 	wait(0);
// }

// int		main()
// {
// int fds[2][2];

// 	pipe(fds[0]);

// 	char *argv1[] = {"ls", "-l", 0};
// 	ft_execute("/bin/ls", argv1, 0, fds[0]);

// 	pipe(fds[1]);

// 	char *argv2[] = {"grep", "env", 0};
// 	ft_execute("/usr/bin/grep", argv2, fds[0], fds[1]);

// 	pipe(fds[0]);

// 	char *argv3[] = {"grep", ".c", 0};
// 	ft_execute("/usr/bin/grep", argv3, fds[1], fds[0]);

// 	char *argv4[] = {"wc", 0};
// 	ft_execute("/usr/bin/wc", argv4, fds[0], 0);
// 	return (0);
// }