/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/01 13:49:22 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell		sh;
	static char	*line = (char *)NULL;

	if (argc != 1 || argv[1])
		return (-1);
	if (init_shell(&sh, env) != 0)
		return (-1);
	while (1)
	{
		line = get_input(line);
		if (line == NULL) // CTRL-D sends EOF, which is handled here
			exit(1);
		if (line[0] != '\0') // or if it contains only whitespace, add func
			process_input(normalize_input(line), sh.this_env);
	}
	free(line);
	rl_clear_history(); // free memory mallocated in readline 
	return (0);
}

int	init_shell(t_shell *sh, char **env)
{
	init_signals();
	if (init_cmds(sh) != 0)
		return (-1);
	
	// init env
	sh->this_env = malloc(sizeof(char *) * ft_strslen(env) + 1);
	if (!sh->this_env)
		return (-1);
	ft_memmove(sh->this_env, env, sizeof(char *) * ft_strslen(env));
	sh->this_env[ft_strslen(env)] = NULL;

	return (0);
}
