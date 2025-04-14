/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/14 14:59:58 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* TODO: env variables should be interpreted in paths too! (ex: with cmd cd) 
 *
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
			cmd_exit(&sh, 1);
		if (line[0] != '\0')
		{
			sh.input_args = normalize_input(line, &sh);
			if (!sh.input_args)
				return (-1);

			// echo abc > infile.txt

			sh.pipeline = build_pipeline_from_tokens(sh.input_args);
			if (!sh.pipeline || sh.pipeline->cmd_count == 0 || !sh.pipeline->cmds[0].argv)
			{
				fprintf(stderr, "Invalid pipeline or command parsing failed\n");
				continue;
			}
			if (sh.pipeline)
			{
				run_pipex_from_minshell(sh.pipeline, env);
				// DEBUG: See what command was parsed
		//		printf("Running: %s\n", sh.pipeline->cmds[0].argv[0]);
				// ONLY call exec if pipeline is valid
		//		exec_with_redirection(sh.pipeline, env);
				// free pipeline
			}
			process_input(&sh);
		}
	}
	free(line);
	rl_clear_history(); // we should probably save the history in a file
	return (0);
}

// Initialize what is needed for the shell (signals, env, pipex, commands)
int	init_shell(t_shell *sh, char **env)
{
	sh->normalized_line = NULL;
	sh->input_args = NULL;
	sh->last_exit_status = 0;
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
	sh->this_env = NULL;
	if (ft_copy_strs_to_list(&sh->this_env, env, nb_vars, '=') != 0)
		return (-1);
	return (0);
}
