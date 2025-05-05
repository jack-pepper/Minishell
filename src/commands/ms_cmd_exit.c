/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/05 12:43:04 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_exit(t_shell *sh, unsigned int status)
{
	size_t	nb_args;

	nb_args = ft_strslen(sh->input_args);
	printf("exit\n");
	if (ft_strcmp(sh->input_args[0], "exit") == 0)
	{
		if (nb_args > 2)
			return (ft_ret(1, EXIT_TOO_MANY_ARGS, STDERR));
		else if (nb_args == 2)
		{
			if (ft_strnopbrk(sh->input_args[1], "0123456789-+") != NULL)
			{
				ft_putstr_fd(EXIT_NUM_ARG_REQ, STDERR);
				status = 2;
			}
			else
				status = (unsigned int)(ft_atoi(sh->input_args[1]));
		}
		free_memory(sh);
		exit(status);
	}
	return (0);
}
