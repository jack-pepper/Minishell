/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:09 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 19:26:43 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_env(t_list *this_env)
{
	t_list	*node;

	node = NULL;
	while (this_env != NULL)
	{
		node = this_env;
		printf("%s=%s\n", ((char **)node->content)[0], ((char **)node->content)[1]);
		this_env = this_env->next;
	}
	return ;
}

/*void	cmd_env(t_list *this_env)
{
	t_list	*node;

	node = NULL;
	while (this_env != NULL)
	{
		node = this_env;
		printf("%s\n", (char *)(node->content));
		this_env = this_env->next;
	}
	return ;
}*/
