/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:18:28 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/03 15:30:50 by yel-bouk         ###   ########.fr       */
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
int validate_and_exec_command(char **argv, char **envp, t_shell *sh)
{
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return 0; // Nothing to run (e.g. $EMPTY)

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
			if (opendir(argv[0])) // check if it's a dir
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

	return 0; // Command appears valid → you can execve it
}

bool validate_all_redirections(char **tokens, t_shell *sh) {
	for (int i = 0; tokens[i]; i++) {
		if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0 ||
			ft_strcmp(tokens[i], (char[]){CTRL_CHAR_HEREDOC, '\0'}) == 0)
		{
			if (!tokens[i + 1] || access(tokens[i + 1], R_OK) != 0) {
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return false;
			}
			i++; // Skip file name
		}
		else if (ft_strcmp(tokens[i], (char[]){CTRL_CHAR_REDIR_OUT, '\0'}) == 0 ||
		         ft_strcmp(tokens[i], (char[]){CTRL_CHAR_APPEND, '\0'}) == 0)
		{
			if (!tokens[i + 1]) {
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
void handle_redir_only(t_shell *sh, char **env)
{
	if (strcmp(sh->input_args[0], (char[]){CTRL_CHAR_REDIR_IN, '\0'}) == 0)
	{
		int new_file = open(sh->input_args[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (new_file == -1)
			perror("open");
		else
			close(new_file);
		return;
	}

	if (!validate_all_redirections(sh->input_args, sh))
		return;

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

	// Not a builtin → run normally
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
			handle_basic(&sh, env);
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

