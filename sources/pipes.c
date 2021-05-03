#include "minishell.h"

// char *files[] = {"/usr/bin/yes", "/usr/bin/head", "/usr/bin/wc"};
// char *argv[3][100] = {{"yes", 0}, {"head", 0}, {"wc", 0}};

char *files[] = {"/bin/ls", "/usr/bin/grep", "/usr/bin/wc"};
char *argv[100][100] = {{"ls", 0}, {"grep", "env", 0}, {"wc", 0}};

// char *files[] = {"/bin/ls", "/bin/ls", "/bin/ls"};
// char *argv[100][100] = {{"ls", 0}, {"ls", 0}, {"ls", 0}};

// char *files[] = {"/bin/sleep", "/usr/bin/head", "/usr/bin/wc"};
// char *argv[3][100] = {{"sleep", "2", 0}, {"head", 0}, {"wc", 0}};

// 0 0 - pipes = 0
// 0 1 - pipes = 1
// 1 1 - pipes = 2,3,4,5
// 1 0 - pipes = -1

int	execute_pipe(char *file, char *argv[], t_pipeline *pl)
{
	pl->fdin[RD] = pl->fdout[RD];
	pl->fdin[WR] = pl->fdout[WR];
	if (pl->pipenum > 0)
		pipe(pl->fdout);

	int pid = fork();
	if (pid == 0)
	{
		// Child thread
		if (pl->pipenum  > 0)					// fdout != 0
		{
			dup2(pl->fdout[WR], STDOUT);
			close(pl->fdout[RD]);
			close(pl->fdout[WR]);
		}
		if (pl->pipenum  > 1 || pl->pipenum  < 0)		// fdin != 0
		{
			dup2(pl->fdin[RD], STDIN);
			close(pl->fdin[RD]);
			close(pl->fdin[WR]);
		}
		int ret = execve(file, argv, 0);
		exit(ret);
	}
	// Parent thread
	if (pl->pipenum  > 0)					// fdout != 0
	{
		close(pl->fdout[WR]);
		// close(pl->fdout[RD]);
	}
	if (pl->pipenum  > 1 || pl->pipenum  < 0)		// fdin != 0
	{
		close(pl->fdin[RD]);
		// close(pl->fdin[WR]);
	}
	return (pid);
}


// static void  ft_execute()
// {
//     extern char **environ;

// 	int rslt;
//     if (!fork())
//     {
//         rslt = execve(files[0], argv[0], environ);
//         if (errno)
// 		{
//             printf("%s:\n", strerror(errno));
// 		}
//         exit(rslt);
//     }
//     wait(&rslt);
//     if (WIFEXITED(rslt))
//         rslt = WEXITSTATUS(rslt);
// }

// int main()
// {
// 	ft_execute();
// 	return (0);
// }



int		main()
{
	t_pipeline pl;

	pl.pipenum = 0;
	int i = 0;
	while (i < 3)
	{
		pl.pipenum++;
		if (i == 2)
			pl.pipenum = -1;		// last command
		pl.pids[i] = execute_pipe(files[i], argv[i], &pl);
		i++;
	}

	i = 0;
	while(i < 3)
	{
		int ret;
		int rslt = waitpid(pl.pids[i], &ret, 0);
		printf("i: %d, rslt: %d, ret: %d, errno: %d\n", i, 0, 0, errno);
		i++;
	}
	return (0);
}