/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:09 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/05 18:21:03 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_env(t_list **this_env)
{
	t_list	*cur_node;

	cur_node = *this_env;
	while (cur_node != NULL)
	{
		printf("%s=%s\n",
			((char **)cur_node->content)[0],
			((char **)cur_node->content)[1]);
		cur_node = cur_node->next;
	}
	return ;
}
