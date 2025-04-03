/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:04:03 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 19:31:15 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_unset(char **input_args, t_list **this_env)
{
//	t_list	*cur_node;

	this_env++; this_env--;

	printf("[cmd_unset]\n");
	if (!input_args[1])
	{
		printf("input: not enough arguments\n");
		return ;
	}
		
/*
	cur_node = this_env;
	while (cur_node != NULL)
	{
		if (ft_strncmp(input_args[1], cur_node->next, ft_strlen(input_args[1]) == 0))
		{
			cur_node
		}
		this_env
	}
*/
}
