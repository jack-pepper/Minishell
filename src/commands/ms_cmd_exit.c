/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/27 23:17:15 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_exit(t_shell *sh, unsigned int status)
{
	free_memory(sh);	
	printf("exit\n");
	if (sh->input_args[1] && ft_isalpha(sh->input_args[1][0]))
	{
		perror("numeric argument required\n");
		exit(2);
	}
	exit(status);
	return (0);
}
