/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:09 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/02 11:11:41 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_env(t_list **this_env)
{
	while (*this_env)
	{
		printf("%s\n", (char *)(*this_env)->next);
		this_env++;
	}
	return ;
}
