/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:04:03 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/04 13:33:11 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_unset(char **input_args, t_list **this_env)
{
	t_list	*tgt_node;
	t_list	*prev_node;

	if (!input_args[1])
	{
		printf("input: not enough arguments\n");
		return ;
	}
	tgt_node = ft_getenv(input_args[1], this_env);
	if (tgt_node != NULL)
	{
		// If a node to suppress is found, we loop through env again to find the previous one.
		// Not very elegant, but works. t_list could be replaced to allow to go back more easily
		// with a prev pointer (just as next). But LIBLIST has to be completed accordingly.
		prev_node = *this_env;
		while (ft_strncmp(((char **)prev_node->next->content)[0], input_args[1], ft_strlen(input_args[1])) != 0)
			prev_node = prev_node->next;
		prev_node->next = tgt_node->next;
		free(tgt_node->content);
		free(tgt_node);
	}
	return;
}
