#include "minishell.h"

void	ft_flags(t_data *data, char c, int type)
{
	if (type == 0)
	{
		data->flg.esc = 0;
		data->flg.quotes = 0;
		data->flg.fdread = 0;
		data->flg.fdwrite = 0;
	}
	if (type == 1 || type == 2)
	{
		if (c == '\\' && (!data->flg.esc || data->flg.esc == 1) \
		&& data->flg.quotes != 39)
			data->flg.esc = 2;
		else if (data->flg.esc == 2)
			data->flg.esc = 1;
		else
			data->flg.esc = 0;
	}
	if (type == 1)
	{
		if (!data->flg.esc && (c == '"' || c == '\'') && !data->flg.quotes)
			data->flg.quotes = c;
		else if (!data->flg.esc && data->flg.quotes && c == \
			data->flg.quotes)
			data->flg.quotes = 0;
	}
}

static void	ft_print_err(t_data *data, int i, int type)
{
	if (type == 1)
	{
		if (data->line[i] != '>')
			printf("bash: syntax error near unexpected token `%c%c'\n", \
			data->line[i], data->line[i]);
		else
			printf("bash: syntax error near unexpected token `%c'\n", \
			data->line[i]);
	}
	if (type == 2)
		write(2, "bash: syntax error near unexpected token `newline'\n", 51);
	if (type == 3)
		write(2, "bash: >\n", 8);
	data->ret = 258;
	ft_init(&data);
}

static void	ft_validate_dups(t_data *data, int i)
{
	while (data->line[++i])
	{
		ft_flags(data, data->line[i], 2);
		if ((!data->flg.esc && !data->flg.squote && (data->line[i] == ';' || data->line[i] == '|') \
		&& data->line[i + 1] == data->line[i]) || (data->line[i] == '>' && data->\
		line[i + 1] == data->line[i] && data->line[i + 2] == data->line[i]))
		{
			ft_print_err(data, i, 1);
			return ;
		}
		if (!data->flg.esc && (data->line[i] == '>' || \
		data->line[i] == '<') && !data->line[i + 1])
		{
			ft_print_err(data, i, 2);
			return ;
		}
		if (data->line[data->len - 2] == '\\')
		{
			ft_print_err(data, i, 3);
			return ;
		}
	}
	ft_get_cmd(data, -1, 0);
}

static void	ft_validate_quotes(t_data *data, int i)
{
	ft_flags(data, '\0', 0);
	while (data->line[++i])
		ft_flags(data, data->line[i], 1);
	if (data->flg.quotes)
	{
		write(2, "bash: >\n", 8);
		data->ret = 258;
		ft_init(&data);
		return ;
	}
	ft_validate_dups(data, -1);
}

void	ft_validate_line(t_data *data, int i)
{
	data->line[data->len - 1] = 0;
	ft_set_term(2);
	if (data->line[0] == 0 && data->len == 1)
	{
		free(data->line);
		ft_init(&data);
		return ;
	}
	add_hist(data->hist, data->line);
	while (data->line[++i])
	{
		while (data->line[i] == ' ')
			i++;
		if (data->line[i] == ';' || data->line[i] == '|')
		{
			validator_err(data, i);
			ft_init(&data);
			data->ret = 258;
			return ;
		}
		break ;
	}
	ft_validate_quotes(data, -1);
}
