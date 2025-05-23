/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:05:05 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/23 20:18:57 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


/* 
 *
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
		else
		{
			struct stat st;

			// if the path exists
			if (stat(argv[0], &st) != 0)
			{
				perror(argv[0]);
				sh->last_exit_status = 127; // no such file or dir
				return 1;
			}
			// is it a valid directory
			if (S_ISDIR(st.st_mode))
			{
				fprintf(stderr, "%s: Is a directory\n", argv[0]);
				sh->last_exit_status = 126;
				return 1;  // Return immediately after detecting directory
			}
		
			// check execute permissions only if it's not a directory
			if (access(argv[0], X_OK) != 0)
			{
				perror(argv[0]); // permission denied
				sh->last_exit_status = 126;
				return 1;
			}
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
void restore_quoted_spaces(char *str)
{
	for (int i = 0; str && str[i]; i++)
	{
		if (str[i] == CC_SPACE_IN_QUOTE)
			str[i] = ' ';
	}
}
bool validate_all_redirections(char **tokens, t_shell *sh)
{
	for (int i = 0; tokens[i]; i++)
	{
		if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
			ft_strcmp(tokens[i], (char[]){CC_HEREDOC, '\0'}) == 0)
		{
			restore_quoted_spaces(tokens[i+1]);
			if (!tokens[i + 1] || access(tokens[i + 1], R_OK) != 0)
			{
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return false;
			}
			i++; // Skip file name
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
		         ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0)
		{
			if (!tokens[i + 1])
			{
				fprintf(stderr, "Error: missing redirection target\n");
				sh->last_exit_status = 1;
				return false;
			}
			restore_quoted_spaces(tokens[i+1]);
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
	// printf("TUREE!\n");
	return true;
}

void handle_redir_only(t_shell *sh, char **env)
{

	if (!validate_all_redirections(sh->input_args, sh))
		return;
		
	if (strcmp(sh->input_args[0], (char[]){CC_REDIR_IN, '\0'}) == 0)
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
	int	i;
	int	first_arg;

	i = 0;
	if (!sh->input_args[i])
		return ;
	while (sh->input_args[i][0] == '\0')
		i++;
	first_arg = i;
	while (sh->input_args[i])
	{
		if (sh->input_args[i][0] == '\0')
			i++;
		else if (ft_strpbrk(sh->input_args[i], "|<>") != NULL)
		{	
		//	printf("arg[%d] (%s) contains pipe or redir!\n", i, sh->input_args[i]);
			break ;
		}
		i++;
	}

	char * cmd_path = get_cmd_path(sh->input_args[first_arg], env);
	if (is_registered_cmd(sh) || cmd_path == NULL)
	{
		free(cmd_path);
		sh->last_exit_status = process_input(sh);
		return ;
	}
	free(cmd_path);

	// Not a builtin, run normally
	if (!validate_all_redirections(sh->input_args, sh))
		return;
	// printf("here again\n");
	t_pipeline *pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
	{
		// printf("I am here2939\n");
		sh->last_exit_status = 127;
		ft_printf("%s: command not found\n", sh->input_args[0]);
		free_pipeline(pipeline);
		return;
	}
	// printf("here??\n");
	exec_with_redirection(pipeline, env, sh);
	free_pipeline(pipeline);
}

void handle_pipeline(t_shell *sh, char **env) {
	t_pipeline *pipeline = build_pipeline_from_tokens(sh->input_args);
	if (!pipeline) {
		sh->last_exit_status = 1;
		return;
	}

	if (has_heredoc(pipeline))
	{
		// For heredoc, we need at least one command
		if (pipeline->cmd_count < 1) {
			ft_printf("Error: heredoc requires at least one command\n");
			sh->last_exit_status = 1;
			free_pipeline(pipeline);
			return;
		}
		int status = run_pipex_from_minshell(pipeline, env);
		if (status != 0) {
			sh->last_exit_status = status;
			free_pipeline(pipeline);
			return;
		}
	} else {
		run_pipeline_basic_pipeline(pipeline, env, sh);
	}

	free_pipeline(pipeline);
}
void handle_pipeline_with_red(t_shell *sh, char **env) {
	// if (!validate_all_redirections(sh->input_args, sh))
	// 	return;

	t_pipeline *pipeline = build_pipeline_from_tokens(sh->input_args);
	if (!pipeline) {
		sh->last_exit_status = 1;
		return;
	}

	if (has_heredoc(pipeline))
		run_pipex_from_minshell(pipeline, env);
	else
	{
		// printf("I am here31\n");	
		run_pipeline_basic_pipeline(pipeline, env, sh);
	}

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

		// What are those lines for?
		if (line[0] == '\0')
			continue; 
		sh.input_args = normalize_input(line, &sh);
		if (!sh.input_args)
			continue;
		////////////////////////////

		// int i = 0;
		// while(sh.input_args[i])
		// {
		// 	printf("input_arg[%d] = %s\n", i,sh.input_args[i]);
		// 	i++;
		// }

		t_cmd_type type = classify_command(sh.input_args);

		// Handle each command type
		if (type == REDIR_ONLY)
		{
			// printf("handle redirection\n");
			handle_redir_only(&sh, env);
		}
		else if (type == BASIC)
		{
			// printf("basic\n");
			validate_and_exec_command(sh.tokens, sh.input_args, &sh);
			handle_basic(&sh, env);
		}
		else if (type == PIPELINE || type == HERE_DOC)
		{
			// printf("handle pipeline\n");
			handle_pipeline(&sh, env);
		}
		else if (type == PIPELINE_WITH_RED)
		{
			// printf("pipeline with redi\n");
			// int num_cmds = count_pipes(sh.input_args) + 1;
			sh.pipeline = ft_calloc(1, sizeof(t_pipeline)); 
			parse_and_build_pipeline(sh.pipeline, sh.input_args);
			run_pipeline_with_redir(sh.pipeline, env, &sh);
		}
		else
		{
			sh.last_exit_status = 1;
			fprintf(stderr, "Error: Unsupported combination of pipes and redirections\n");
		}

		ft_free_array(sh.input_args, -1);
	}
	free(line);
	rl_clear_history();
	//return 0;
	return (sh.last_exit_status);
}
