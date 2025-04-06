/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/06 21:17:26 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* TODO: env variables should be interpreted in paths too! (ex: with cmd cd) 
 * TODO: unset cmd must handle several env var in a single command
 * TODO: need to process lines like: $USER $VAR ABC $GHI CDE
 */

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
		if (line == NULL) // Handles EOF (sent by CTRL-D)
			exit(1);
		if (line[0] != '\0')  // NEEDED
			process_input(normalize_input(line), &sh.this_env); // NEEDED // Need to split it to free
	
	/* ADDED BY YAHYA
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
	*/

	}
	free(line);
	rl_clear_history(); // we should probably save the history in a file
	return (0);
}

// Initialize what is needed for the shell (signals, env, pipex, commands)
int	init_shell(t_shell *sh, char **env)
{
	init_signals();
	if (init_env(sh, env) != 0)
		return (-1);
	// init_pipex (make a pointer to this_env)
	if (init_cmds(sh) != 0)
		return (-1);	
	return (0);
}

// Initialize a local environment by storing the env variables to a t_list
int	init_env(t_shell *sh, char **env)
{
	size_t	nb_vars;

	nb_vars = ft_strslen(env);
	sh->this_env = malloc(sizeof(t_list *) * (nb_vars + 1));
	if (!sh->this_env)
		return (-1);
	sh->this_env = NULL;
	if (ft_copy_strs_to_list(&sh->this_env, env, nb_vars, '=') != 0)
		return (-1);
	return (0);
}
