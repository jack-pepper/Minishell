/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:04:03 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/03 15:50:21 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Remove requested vars from this_env.
int	cmd_unset(t_shell *sh)
{
	t_list	*tgt_node;
	t_list	*prev_node;
	int		i;

	if (!sh->input_args[1])
		return (0);
	i = 1;
	while (sh->input_args[i])
	{
		tgt_node = ft_getenv(sh->input_args[i], &sh->this_env);
		if (tgt_node != NULL)
		{
			prev_node = sh->this_env;
			while (ft_strcmp(
				((char **)prev_node->next->content)[0],
				sh->input_args[i]) != 0)
				prev_node = prev_node->next;
			prev_node->next = tgt_node->next;
			free_args(tgt_node->content);
			free(tgt_node);
		}
		i++;
	}
	
	return (0);
}
