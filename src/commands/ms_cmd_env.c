/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:09 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/18 22:56:32 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_env(t_shell *sh)
{
	t_list	*cur_node;

	cur_node = sh->this_env;
	while (cur_node != NULL)
	{
		printf("%s=%s\n",
			((char **)cur_node->content)[0],
			((char **)cur_node->content)[1]);
		cur_node = cur_node->next;
	}
	return (0);
}
