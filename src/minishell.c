/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/20 12:09:35 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


int	init_shell(t_shell *sh)
{
	sh++; // for compil test
	sh--; // for compil test
	init_signals();
	return (0);
}

int	main()
{
	t_shell	sh;
	static char	*line = (char *)NULL;

	init_shell(&sh);
	if (line)
	{
		free(line);
		line = (char *)NULL;
	}
	while (1)
	{
		line = readline("> ");
		if (line && *line) // save in history only if not empty
		{
			add_history(line); // NB: We should probably store the content in some external file on quit
			printf("%s\n", line);
			free(line);
		}
	}
	rl_clear_history(); // free memory mallocated in readline 
	return (0);
}
