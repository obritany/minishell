#include "minishell.h"

static void	ft_set_dir(t_data *data, char **pwd)
{
	char	buf[4096];

	*pwd = ft_strjoin("PWD=", getcwd(buf, 4096));
	if (get_var(data->env, "PWD"))
		set_var(&data->env, *pwd, 0);
	free(*pwd);
}

static void	ft_set_oldpwd(t_data *data, char **oldpwd)
{
	char	buf[4096];
	char	*temp;

	temp = ft_strdup(getcwd(buf, 4096));
	*oldpwd = ft_strjoin("OLDPWD=", temp);
	free(temp);
	set_var(&data->env, *oldpwd, 0);
	free(*oldpwd);
	data->flg.opwd = 0;
}

void	ft_cd(t_data *data, int ret, char *pwd, char *oldpwd)
{
	char	*newdir;

	newdir = NULL;
	if (get_var(data->env, "OLDPWD") || data->flg.opwd)
		ft_set_oldpwd(data, &oldpwd);
	if (data->argc == 1 && get_var(data->env, "HOME"))
		newdir = ft_strdup(get_var(data->env, "HOME"));
	else if (data->argc == 1 && !get_var(data->env, "HOME"))
	{
		printf("bash: cd: HOME not set\n");
		return ;
	}
	else if (data->argc >= 2)
		newdir = ft_strdup(data->argv[1]);
	ret = chdir(newdir);
	if (ret == -1)
	{
		data->ret = 1;
		printf("bash: cd: %s: %s\n", newdir, strerror(errno));
		free(newdir);
		return ;
	}
	free(newdir);
	ft_set_dir(data, &pwd);
}
