/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/25 18:49:43 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(void)
{
	t_shell		sh;
	static char	*line = (char *)NULL;

	if (init_shell(&sh) != 0)
		return (-1);
	while (1)
	{
		line = get_input(line);
		if (line == NULL) // CTRL-D sends EOF, which is handled here
			exit(1);
		process_input(line);
	}
	free(line);
	rl_clear_history(); // free memory mallocated in readline 
	return (0);
}

int	init_shell(t_shell *sh)
{
	init_signals();
	if (init_cmds(sh) != 0)
		return (-1);
	return (0);
}


