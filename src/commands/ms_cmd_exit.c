/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/08 17:30:55 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_exit(char **input_args, t_shell *sh, unsigned int status)
{
	free_memory(sh, input_args);
	printf("exit\n");
	exit(status);
}
