/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/29 11:54:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_exit(t_shell *sh, unsigned int status)
{
	printf("exit\n");
	if (sh->input_args[1] && ft_isalpha(sh->input_args[1][0])) // Hmm...
	{
		ft_putstr_fd(EXIT_NUM_ARG_REQ, STDERR);
		free_memory(sh);	
		exit(2);
	}
	if (sh->input_args[2])
		return (ft_ret(1, EXIT_TOO_MANY_ARGS, STDERR));
	free_memory(sh);
	exit(status);
	return (0);
}
