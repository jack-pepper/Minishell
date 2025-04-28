/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/28 23:01:19 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_exit(t_shell *sh, unsigned int status)
{
	printf("exit\n");
	if (sh->input_args[1] && ft_isalpha(sh->input_args[1][0]))
	{
		printf("numeric argument required\n");
		free_memory(sh);	
		exit(2);
	}
	free_memory(sh);
	exit(status);
	return (0);
}
