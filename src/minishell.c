/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:18:28 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/28 09:58:51 by yel-bouk         ###   ########.fr       */
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
bool validate_pipeline_files(t_pipeline *p)
{
	if (p->infile)
	{
		if (access(p->infile, F_OK) != 0)
		{
			perror(p->infile);
			return false;
		}
	}
	return true;
}
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
			// int x = 0;
			// while (sh.input_args[x]) {
			// 	printf("[DEBUG]input_argx[%d] = \"%s\"\n", x, sh.input_args[x]);
			
			// 	// int j = 0;
			// 	// while (sh.input_args[x][j]) {
			// 	// 	printf("    char[%d] = '%c' (ASCII: %d)\n",
			// 	// 		   j,
			// 	// 		   sh.input_args[x][j],
			// 	// 		   (unsigned char)sh.input_args[x][j]);
			// 	// 	j++;
			// 	// }
			// 	x++;
			// }
				
			t_cmd_type type = classify_command(sh.input_args);
			if ((type == REDIR_ONLY || type == BASIC) && type != PIPELINE)
			{
				if (strcmp(sh.input_args[0], (char[]){CTRL_CHAR_REDIR_IN,'\0'}) == 0)
				{
					// printf("[DEBUG]REDIR_ONLY\n");
					int new_file = open(sh.input_args[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if (new_file == -1)
						perror("open");
				}
				else if (is_builtin(sh.input_args[0]) && type == BASIC) 
				{
					// printf("[DEBUG] process_input\n");	
					process_input(&sh);
				}
				else
				{	
					t_pipeline *pipeline = parse_redirection_only(sh.input_args);
					if (pipeline && pipeline->cmds && pipeline->cmds[0].argv)
					{
						// printf("[DEBUG]exec_with_red\n");
						if(strcmp(sh.input_args[0], "cd") == 0)
							printf(" ");
						else
						{
							// printf("exec_with_redirection\n");
							exec_with_redirection(pipeline, env);
						}
					}	
					else
					{
						fprintf(stderr, "Invalid redirection command\n");
					}
					free_pipeline(pipeline);
				}
			}
			else if (type == PIPELINE)
			{
				// printf("[DEBUG]PIPELINE\n");
				t_pipeline *pipeline = build_pipeline_from_tokens(sh.input_args);
				// printf("Pipeline Created\n");
				if (pipeline)
				{
					// printf("[DEBUG]run pipes_with_no_redir\n");	
					// run_pipes_with_no_redir(pipeline, env);
					run_pipeline_with_redir(pipeline, env);
				}
				else
					fprintf(stderr, "Invalid pipeline\n");
				free_pipeline(pipeline);
			}

			
			else if (type == MIXED_INVALID)
				fprintf(stderr, "Error: Unsupported combination of pipes and redirections\n");
			ft_free_array(sh.input_args, -1); // Make sure you free your token array
		}
	}

	free(line);
	rl_clear_history();
	return (0);
}

// // Initialize a local environment by storing the env variables to a t_list
// int	init_env(t_shell *sh, char **env)
// {
// 	size_t	nb_vars;

// 	nb_vars = ft_strslen(env);
// 	sh->this_env = NULL;
// 	sh->env_stash = NULL;
// 	if (ft_strstolist(&sh->this_env, env, nb_vars, '=') != 0)
// 		return (-1);
// 	return (0);
// }

// // Initialize what is needed for the shell (signals, env, pipex, commands)
// int	init_shell(t_shell *sh, char **env)
// {
// 	sh->normalized_line = NULL;
// 	sh->input_args = NULL;
// 	sh->last_exit_status = 0;
// 	init_signals();
// 	if (init_env(sh, env) != 0)
// 		return (-1);
// 	// init_pipex (make a pointer to this_env)
// 	if (init_cmds(sh) != 0)
// 		return (-1);	
// 	return (0);
// }

