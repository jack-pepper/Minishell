/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/30 17:39:15 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_shell		sh;
	static char	*line = (char *)NULL;

	if (argc != 1 || argv[1])
		return (-1);
	if (init_shell(&sh) != 0)
		return (-1);
	while (1)
	{
		line = get_input(line);
		if (line == NULL) // CTRL-D sends EOF, which is handled here
			exit(1);
		// if (line[0] != '\0') // or if it contains only whitespace, add func
		// 	process_input(normalize_input(line), env);
		else if (line[0] == '<')
		{
			printf("Yahya [DEBUG] line: %s\n", line);
			char **tokens = normalize_input(line);
			int i = 0;
			while(tokens[i])
			{
				printf("Yahya [DEBUG] token[%d]: %s\n", i, tokens[i]);
				i++;
			}
			t_pipeline *pipeline = build_pipeline_from_tokens(tokens);
			printf("Yahya [DEBUG] pipeline->cmd_count: %d\n", pipeline->cmd_count);
			if (pipeline)
				run_pipex_from_minshell(pipeline, env);
			// Memory cleanup
		}
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
