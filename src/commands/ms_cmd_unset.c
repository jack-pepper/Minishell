/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:04:03 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 22:40:07 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_unset(char **input_args, t_list **this_env)
{
	t_list	*tgt_node;

	if (!input_args[1])
	{
		printf("input: not enough arguments\n");
		return ;
	}
	tgt_node = ft_getenv(input_args[1], this_env);
	if (tgt_node != NULL)
	{
		//((char **)tgt_node->content)[1][0] = 'X'; // DEBUG
		// Should detach and free the node
		
	}
	return;
}
