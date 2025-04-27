/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:04:03 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/27 14:54:31 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Remove requested vars from this_env.
int	cmd_unset(t_shell *sh)
{
	t_list	*tgt_node;
	t_list	*prev_node;
	int	i;

	if (!sh->input_args[1])
	{
		printf("input: not enough arguments\n");
		return (-1);
	}
	i = 1;
	while (sh->input_args[i])
	{
		tgt_node = ft_getenv(sh->input_args[i], &sh->this_env);
		if (tgt_node != NULL) // if a node to remove is found...
		{
			// ... we loop through env again to find prev node.
			// (could use bidirectional lists instead))
			// note: do a get_prev_node() from this
			prev_node = sh->this_env;
			while (ft_strcmp(((char **)prev_node->next->content)[0], sh->input_args[i]) != 0)
				prev_node = prev_node->next;
			prev_node->next = tgt_node->next;
			free_args(tgt_node->content);
			free(tgt_node);
		}
		else
			printf("No such env var found\n");
		i++;
	}
	return (0);
}
