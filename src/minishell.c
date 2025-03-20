/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/20 18:08:14 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(void)
{
	t_shell		sh;
	static char	*line = (char *)NULL;

	init_shell(&sh);
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
	sh++; // for compil test
	sh--; // for compil test
	init_signals();
	return (0);
}

char	*get_input(char *line)
{
	if (line)
	{
		free(line);
		line = (char *)NULL;
	}
	line = readline("> ");
	if (line && *line) // save in history only if not empty
	{
		add_history(line); // NB: We should probably store the content in some external file on quit
		printf("%s\n", line);
		if (rl_on_new_line() != 0) // How to actually handle this error?
			return (line);
	}
	else if (!line) // CTRL-D sends EOF, which is handled here
		return ((char *)NULL);
	return (line);
}

void	process_input(char *line)
{
	// parsing
	// execution
}
