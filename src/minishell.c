/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:18:28 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/27 15:29:21 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* TODO: env variables should be interpreted in paths too! (ex: with cmd cd) 
 *
 */

// int	main(int argc, char **argv, char **env)
// {
// 	t_shell		sh;
// 	static char	*line = (char *)NULL;

// 	if (argc != 1 || argv[1])
// 		return (-1);
// 	if (init_shell(&sh, env) != 0)
// 		return (-1);
// 	while (1)
// 	{
// 		if (sh.input_args != NULL)
// 			free_args(sh.input_args);
// 		line = get_input(line);
// 		if (line == NULL) // Handles EOF (sent by CTRL-D)
// 			cmd_exit(&sh, 1);
// 		if (line[0] != '\0')
// 		{
// 			sh.input_args = normalize_input(line, &sh);
// 			if (!sh.input_args)
// 				return (-1);

// 			// echo abc > infile.txt

// 			sh.pipeline = build_pipeline_from_tokens(sh.input_args);
// 			if (!sh.pipeline || sh.pipeline->cmd_count == 0 || !sh.pipeline->cmds[0].argv)
// 			{
// 				fprintf(stderr, "Invalid pipeline or command parsing failed\n");
// 				continue;
// 			}
// 			if (sh.pipeline)
// 			{
// 				run_pipex_from_minshell(sh.pipeline, env);
// 				// DEBUG: See what command was parsed
// 		//		printf("Running: %s\n", sh.pipeline->cmds[0].argv[0]);
// 				// ONLY call exec if pipeline is valid
// 		//		exec_with_redirection(sh.pipeline, env);
// 				// free pipeline
// 			}
// 			process_input(&sh);
// 		}
// 	}
// 	free(line);
// 	rl_clear_history(); // we should probably save the history in a file
// 	return (0);
// }

int main(int argc, char **argv, char **env)
{
	t_shell sh;
	static char *line = NULL;

	if (argc != 1 || argv[1])
		return (-1);
	if (init_shell(&sh, env) != 0)
		return (-1);

	while (1)
	{
		// if (sh.input_args != NULL)
		// 	free_args(sh.input_args);
		line = get_input(line);
		if (line == NULL)
			cmd_exit(&sh, 1);
		if (line[0] != '\0')
		{
			sh.input_args = normalize_input(line, &sh);
			if (!sh.input_args)
				return (-1);
			t_cmd_type type = classify_command(sh.input_args);
			if (type == REDIR_ONLY || type == BASIC)
			{
				//if (is_builtin(sh.input_args[0]))
					process_input(&sh);
				//else
				//{	
				//	t_pipeline *pipeline = parse_redirection_only(sh.input_args);
				//	if (pipeline && pipeline->cmds && pipeline->cmds[0].argv)
				//		exec_with_redirection(pipeline, env);
				//	else
				//	{
				//		fprintf(stderr, "Invalid redirection command\n");
				//	}
				//	free_pipeline(pipeline);
				//}
			}
			else if (type == PIPELINE)
			{
				t_pipeline *pipeline = build_pipeline_from_tokens(sh.input_args);
				if (pipeline)
					run_pipeline_from_minshell(pipeline, env);
				else
					fprintf(stderr, "Invalid pipeline\n");
				free_pipeline(pipeline);
			}

			
			else if (type == MIXED_INVALID)
				fprintf(stderr, "❌ Error: Unsupported combination of pipes and redirections\n");
			ft_free_array(sh.input_args, -1); // Make sure you free your token array
		}
	}

	free(line);
	rl_clear_history();
	return (sh.last_exit_status); // ???
}
