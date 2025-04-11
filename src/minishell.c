/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:59:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/11 16:31:06 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* TODO: env variables should be interpreted in paths too! (ex: with cmd cd) 
 * TODO: ctrl+c should display a new prompt
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
			if (normalize_input(line, &sh) != 0)
				return (-1);
			if (!sh.normalized_line)
				return (-1);
			process_input(&sh);
		}
		if (line[0] == '<')
		{
			if (normalize_input(line, &sh) != 0)
				return (-1);
			t_pipeline *pipeline = build_pipeline_from_tokens(sh.tokens);
			if (pipeline)
				run_pipex_from_minshell(pipeline, env);
			// Memory cleanup
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
