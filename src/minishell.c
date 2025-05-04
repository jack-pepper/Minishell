/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:16:39 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/04 17:17:46 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


/* TODO: env variables should be interpreted in paths too! (ex: with cmd cd) 
 *
 */
int validate_and_exec_command(char **argv, char **envp, t_shell *sh)
{
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return 0; // Nothing to run (example: $EMPTY)

	if (ft_strchr(argv[0], '/'))
	{
		if (access(argv[0], F_OK) != 0)
		{
			fprintf(stderr, "%s: No such file or directory\n", argv[0]);
			sh->last_exit_status = 127;
			return 1;
		}
		else if (access(argv[0], X_OK) != 0)
		{
			if (opendir(argv[0])) // check if it's a directory
				fprintf(stderr, "%s: Is a directory\n", argv[0]);
			else
				perror(argv[0]); // permission denied or other
			sh->last_exit_status = 126;
			return 1;
		}
	}
	else
	{
		char *cmd_path = get_cmd_path(argv[0], envp);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: command not found\n", argv[0]);
			sh->last_exit_status = 127;
			return 1;
		}
		free(cmd_path);
	}

	return 0; // if command is valid then you can execve it
}

bool validate_all_redirections(char **tokens, t_shell *sh) {
	for (int i = 0; tokens[i]; i++)
	{
		if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0 ||
			ft_strcmp(tokens[i], (char[]){CTRL_CHAR_HEREDOC, '\0'}) == 0)
		{
			if (!tokens[i + 1] || access(tokens[i + 1], R_OK) != 0)
			{
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return false;
			}
			i++; // Skip file name
		}
		else if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0 ||
		         ft_strcmp(tokens[i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0)
		{
			if (!tokens[i + 1])
			{
				fprintf(stderr, "Error: missing redirection target\n");
				sh->last_exit_status = 1;
				return false;
			}
			int flags = O_WRONLY | O_CREAT | O_APPEND;
			int fd = open(tokens[i + 1], flags, 0644);
			if (fd < 0) {
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return false;
			}
			close(fd);
			i++; // Skip file name
		}
	}
	return true;
}

void handle_redir_only(t_shell *sh, char **env)
{

	if (!validate_all_redirections(sh->input_args, sh))
		return;
		
	if (strcmp(sh->input_args[0], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0)
	{
		int new_file = open(sh->input_args[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (new_file == -1)
			perror("open");
		else
			close(new_file);
		return;
	}

	t_pipeline *pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
	{
		sh->last_exit_status = 1;
		fprintf(stderr, "Invalid redirection command\n");
		free_pipeline(pipeline);
		return;
	}

	exec_with_redirection(pipeline, env, sh);
	free_pipeline(pipeline);
}
void handle_basic(t_shell *sh, char **env)
{
	if (is_builtin(sh->input_args[0]))
	{
		process_input(sh);
		return;
	}

	// Not a builtin, run normally
	if (!validate_all_redirections(sh->input_args, sh))
		return;

	t_pipeline *pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
	{
		sh->last_exit_status = 127;
		fprintf(stderr, "%s: command not found\n", sh->input_args[0]);
		free_pipeline(pipeline);
		return;
	}

	exec_with_redirection(pipeline, env, sh);
	free_pipeline(pipeline);
}

void handle_pipeline(t_shell *sh, char **env)
{
	if (!validate_all_redirections(sh->input_args, sh))
		return;

	t_pipeline *pipeline = build_pipeline_from_tokens(sh->input_args);
	if (!pipeline)
	{
		sh->last_exit_status = 1;
		return;
	}

	if (strcmp(sh->input_args[0], (char[]){CTRL_CHAR_HEREDOC, '\0'}) == 0)
		run_pipex_from_minshell(pipeline, env);
	else
		run_pipeline_with_redir(pipeline, env, sh);

	free_pipeline(pipeline);
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
		line = get_input(line);
		if (line == NULL)
			cmd_exit(&sh, 1);

		if (line[0] == '\0')
			continue;

		sh.input_args = normalize_input(line, &sh);
		if (!sh.input_args)
			continue;

		t_cmd_type type = classify_command(sh.input_args);

		// Handle each command type
		if (type == REDIR_ONLY)
			handle_redir_only(&sh, env);
		else if (type == BASIC)
		{
<<<<<<< HEAD
			
			sh.input_args = normalize_input(line, &sh);
			if (!sh.input_args)
				return (-1);
			//  int x = 0;
			//  while (sh.input_args[x]) {
			//  	printf("[DEBUG]input_argx[%d] = \"%s\"\n", x, sh.input_args[x]);
			
			//  	int j = 0;
			//  	 while (sh.input_args[x][j]) {
			//  	printf("    char[%d] = '%c' (ASCII: %d)\n",
			//  		  j,
			// 	   sh.input_args[x][j],
			//  		   (unsigned char)sh.input_args[x][j]);
			//  	j++;
			//  }
			//  	x++;
			//  }
			 
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
					//printf("[DEBUG] process_input\n");
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
							exec_with_redirection(pipeline, env, &sh);
							
						}
					}	
					else
					{
						sh.last_exit_status = 1;
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
					if(strcmp(sh.input_args[0], (char[]){CTRL_CHAR_HEREDOC, '\0'}) == 0)
					{
						// printf("pipex from minishell\n");	
						run_pipex_from_minshell(pipeline, env);
					}
					else
					{
						// printf("pipes_with redir\n");
						run_pipeline_with_redir(pipeline, env, &sh);
					}
				}
				else
				{
					sh.last_exit_status = 1;	
					fprintf(stderr, "Invalid pipeline\n");
				}
				free_pipeline(pipeline);
			}
			else if (type == MIXED_INVALID)
			{
				sh.last_exit_status = 1;	
				fprintf(stderr, "Error: Unsupported combination of pipes and redirections\n");
			}
			ft_free_array(sh.input_args, -1); // Make sure you free your token array
=======
			// printf("basic\n");
			handle_basic(&sh, env);
>>>>>>> 8b77d82f526f6c2392b59efcd50d99c6010ad176
		}
		else if (type == PIPELINE)
			handle_pipeline(&sh, env);
		else if (type == MIXED_INVALID)
		{
			sh.last_exit_status = 1;
			fprintf(stderr, "Error: Unsupported combination of pipes and redirections\n");
		}

		ft_free_array(sh.input_args, -1);
	}
	free(line);
	rl_clear_history();
	return 0;
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

