/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/20 00:13:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	init_shell(t_shell *sh)
{	
	init_signals();
//	init_history();
	return (0);
}

int	main()
{
	t_shell	sh;
	char	*line;

	line = (char *)NULL;
	init_shell(&sh);
//	while (1)
//	{
		line = readline("> ");
		if (line)
		{
			add_history(line);
			printf("%s\n", line);
		}

//	}
	rl_clear_history(); // free memory mallocated in readline 
	free(line); // 
	return (0);
}
