/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:04:03 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/15 14:03:32 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void	cmd_unset(t_shell *sh)
{
	t_list	*tgt_node;
	t_list	*prev_node;
	int	i;

	if (!sh->input_args[1])
	{
		printf("input: not enough arguments\n");
		return ;
	}
	i = 1;
	while (sh->input_args[i])
	{
		tgt_node = ft_getenv(sh->input_args[i], &sh->this_env);
		if (tgt_node != NULL)
		{
		// If a node to suppress is found, we loop through env again to find the previous one.
		// Not very elegant, but works. t_list could be replaced to allow to go back more easily
		// with a prev pointer (just as next). But LIBLIST has to be completed accordingly.
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
		return;
}
