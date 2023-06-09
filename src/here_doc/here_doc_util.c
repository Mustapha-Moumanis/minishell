/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 16:25:19 by mmoumani          #+#    #+#             */
/*   Updated: 2023/06/04 14:30:24 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*parse_line(t_data *data, t_elem *elem, char *str)
{
	char	*cmd;
	char	*tmp;

	while (elem)
	{
		if (elem->type == WORD || elem->type == '\\')
			cmd = parse_word(&elem);
		else if (elem->type == ENV || elem->type == EXIT_STATUS)
			cmd = parse_env(data, &elem);
		else
			cmd = ft_strdup(elem->content);
		tmp = str;
		str = ft_strjoin(tmp, cmd);
		free(tmp);
		if (cmd && elem->type != ENV && elem->type != EXIT_STATUS)
			free(cmd);
		cmd = NULL;
		elem = elem->next;
	}
	tmp = str;
	str = ft_strjoin(tmp, "\n");
	free(tmp);
	return (str);
}

char	*update_line(t_data *last_data, char *str)
{
	t_data	data;
	char	*line;
	t_elem	*tmp;

	data = (t_data){0};
	data.n_env = last_data->n_env;
	data.input = str;
	lexer(&data);
	tmp = data.elem;
	while (tmp)
	{
		if (tmp->type == WORD && tmp->content[0] == '$')
			tmp->type = ENV;
		if (!ft_strcmp(tmp->content, "$?"))
			tmp->type = EXIT_STATUS;
		tmp = tmp->next;
	}
	line = parse_line(&data, data.elem, ft_strdup(""));
	ft_clear_elems(&data.elem, &free);
	return (line);
}

char	*get_quote_delimiter(t_elem **lex, enum e_type type)
{
	char	*s;
	char	*tmp;

	*lex = (*lex)->next;
	s = NULL;
	while (*lex)
	{
		if (!s)
			s = ft_strdup("");
		if ((*lex)->next && (*lex)->type == type && (*lex)->next->type == type)
			(*lex) = (*lex)->next;
		else if ((*lex)->type != type)
		{
			tmp = s;
			s = ft_strjoin(tmp, (*lex)->content);
			free(tmp);
		}
		else
			break ;
		if (*lex)
			*lex = (*lex)->next;
	}
	return (s);
}

int	check_advence(t_elem **lex)
{
	enum e_type	type;

	type = (*lex)->next->type;
	return ((*lex)->next && (type == DQUOTE || type == QUOTE));
}

char	*get_delimiter(t_elem **lex, char *tmp, char *tmp2, int *is)
{
	char	*str;

	skeap_space(lex);
	str = ft_strdup("");
	while (*lex && !ft_whitespace((*lex)->type) && (*lex)->type != '|')
	{
		tmp = str;
		if ((*lex)->type == DQUOTE || (*lex)->type == QUOTE)
		{
			tmp2 = get_quote_delimiter(lex, (*lex)->type);
			str = ft_strjoin(tmp, tmp2);
			free(tmp2);
			*is = 1;
		}
		else if ((*lex)->type == '$' && (*lex)->next && check_advence(lex))
			str = ft_strjoin(tmp, "");
		else
			str = ft_strjoin(tmp, (*lex)->content);
		free(tmp);
		if ((*lex)->next && is_red((*lex)->next->type))
			break ;
		(*lex) = (*lex)->next;
	}
	return (str);
}
