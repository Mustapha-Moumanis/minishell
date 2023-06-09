/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoumani <mmoumani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 21:41:45 by mmoumani          #+#    #+#             */
/*   Updated: 2023/05/21 17:01:13 by mmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_cmd	*ft_new_cmd(int in_file, int out_file, char **full_cmd)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->in_file = in_file;
	node->out_file = out_file;
	node->full_cmd = full_cmd;
	node->next = NULL;
	return (node);
}

t_cmd	*ft_last_cmd(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_cmd_back(t_cmd **lst, t_cmd *new)
{
	if (!(*lst))
		*lst = new;
	else
		ft_last_cmd(*lst)->next = new;
}

void	ft_lst_delone(t_cmd *lst, void (*del)(char **))
{
	if (lst && del)
	{
		del(lst->full_cmd);
		free(lst);
	}
}

void	ft_cmd_clear(t_cmd **lst, void (*del)(char **))
{
	t_cmd	*tmp;

	if (lst && del)
	{
		while (*lst)
		{
			tmp = (*lst)->next;
			ft_lst_delone(*lst, del);
			*lst = tmp;
		}
	}
}
