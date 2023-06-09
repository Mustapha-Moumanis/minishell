/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 21:59:18 by mmoumani          #+#    #+#             */
/*   Updated: 2023/06/03 23:05:53 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*change_content(t_data *data)
{
	char	*str;

	str = data->cmd_lst->next->content;
	free(data->cmd_lst->next);
	data->cmd_lst->next = NULL;
	return (str);
}

char	*collecting_cmd(t_data *data, t_elem **lex, char *str, enum e_type t)
{
	char	*cmd;
	char	*tmp;

	skeap_space(lex);
	while (*lex && !ft_whitespace((*lex)->type) && (*lex)->type != '|')
	{
		if (is_red(t) && (*lex)->type == ENV)
		{
			if (data->expnd)
				free(data->expnd);
			data->expnd = ft_strdup((*lex)->content);
		}
		cmd = parse_cmd(data, lex);
		tmp = str;
		str = ft_strjoin(tmp, cmd);
		free(tmp);
		if ((*lex)->type != ENV && (*lex)->type != EXIT_STATUS)
			free(cmd);
		if (str && (*lex)->next && is_red((*lex)->next->type))
			break ;
		(*lex) = (*lex)->next;
	}
	return (str);
}

void	get_red(t_data *data, t_elem **lex, enum e_type type, char	*str)
{
	if (str)
		ft_lstadd_back(&data->cmd_lst, ft_lstnew(str));
	str = ft_strdup("");
	(*lex) = (*lex)->next;
	if (type == HERE_DOC && data->error == 0)
		her_doc(data, lex, 0);
	else
	{
		str = collecting_cmd(data, lex, str, type);
		if (data->file_error != 1)
		{
			if (type == REDIR_IN)
				in_file(data, str);
			else if (type == REDIR_OUT)
				out_file(data, str);
			else if (type == DREDIR_OUT)
				dout_file(data, str);
		}
	}
	free(str);
}

void	simple_cmd(t_data *data, t_elem **lex)
{
	char	*str;

	str = NULL;
	while (*lex && !((*lex)->type == '|' && (*lex)->state == GENERAL))
	{
		if (is_red((*lex)->type))
			get_red(data, lex, (*lex)->type, str);
		else
		{
			if (!str && !ft_whitespace((*lex)->type))
				str = ft_strdup("");
			if (!(ft_whitespace((*lex)->type) && (*lex)->state == GENERAL))
				str = collecting_cmd(data, lex, str, -1);
			if ((str && (!(*lex) || ft_whitespace((*lex)->type)))
				|| (*lex)->type == '|')
			{
				ft_lstadd_back(&data->cmd_lst, ft_lstnew(str));
				str = NULL;
			}
		}
		if ((*lex) && (*lex)->type == '|')
			break ;
		if (*lex)
			(*lex) = (*lex)->next;
	}
}

int	parsing(t_data *data, t_elem *lex)
{
	if (!lex)
		return (0);
	skeap_space(&lex);
	while (lex)
	{
		simple_cmd(data, &lex);
		if (!lex || lex->type == PIPE_LINE)
		{
			append_exution_struct(data);
			data->file_error = 0;
		}
		if (lex)
			lex = lex->next;
	}
	print_error(data->save_error);
	ft_lstclear(&data->save_error, &free);
	return (0);
}
