/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:05:05 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/30 12:35:58 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	validate_and_exec_command(char **argv, char **envp, t_shell *sh)
{
	struct stat	st;
	char		*cmd_path;

	if (!argv || !argv[0] || argv[0][0] == '\0')
		return (0);
	if (ft_strchr(argv[0], '/'))
	{
		if (access(argv[0], F_OK) != 0)
		{
			perror("No such file or directory");
			sh->last_exit_status = 127;
			return (1);
		}
		else
		{
			if (stat(argv[0], &st) != 0)
			{
				perror(argv[0]);
				sh->last_exit_status = 127;
				return (1);
			}
			if (S_ISDIR(st.st_mode))
			{
				fprintf(stderr, "%s: Is a directory\n", argv[0]);
				sh->last_exit_status = 126;
				return (1);
			}
			if (access(argv[0], X_OK) != 0)
			{
				perror(argv[0]);
				sh->last_exit_status = 126;
				return (1);
			}
		}
	}
	else
	{
		cmd_path = get_cmd_path(argv[0], envp);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: command not found\n", argv[0]);
			sh->last_exit_status = (127);
			return (1);
		}
		free(cmd_path);
	}
	return (0);
}

void	restore_quoted_spaces(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == CC_SPACE_IN_QUOTE)
			str[i] = ' ';
		i++;
	}
}

bool	validate_all_redirections(char **tokens, t_shell *sh)
{
	int	i;
	int	fd;
	int	flags;

	i = 0;
	while (tokens[i])
	{
		if (is_token_control_char(tokens[i], CC_REDIR_IN)
			|| is_token_control_char(tokens[i], CC_HEREDOC))
		{
			restore_quoted_spaces(tokens[i + 1]);
			if (!tokens[i + 1] || access(tokens[i + 1], R_OK) != 0)
			{
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return (false);
			}
			i++;
		}
		else if (is_token_control_char (tokens[i], CC_REDIR_OUT)
			|| is_token_control_char(tokens[i], CC_APPEND))
		{
			if (!tokens[i + 1])
			{
				fprintf(stderr, "Error: missing redirection target\n");
				sh->last_exit_status = 1;
				return (false);
			}
			restore_quoted_spaces(tokens[i + 1]);
			flags = O_WRONLY | O_CREAT | O_APPEND;
			fd = open(tokens[i + 1], flags, 0644);
			if (fd < 0)
			{
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return (false);
			}
			close(fd);
			i++;
		}
		i++;
	}
	return (true);
}

void	handle_redir_only(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;
	int			new_file;

	if (!validate_all_redirections(sh->input_args, sh))
		return ;
	if (is_token_control_char(sh->input_args[0], CC_REDIR_IN))
	{
		new_file = open(sh->input_args[1], O_WRONLY
				| O_CREAT | O_TRUNC, 0644);
		if (new_file == -1)
			perror("open");
		else
			close(new_file);
		return ;
	}
	pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
	{
		sh->last_exit_status = 1;
		fprintf(stderr, "Invalid redirection command\n");
		free_pipeline(pipeline);
		return ;
	}
	exec_with_redirection(pipeline, env, sh);
	free_pipeline(pipeline);
}

void	handle_basic(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;

	if (is_builtin(sh->input_args[0]))
	{
		sh->last_exit_status = process_input(sh);
		return ;
	}

	// int	i;
	// int	first_arg;

	// i = 0;
	// if (!sh->input_args[i])
	// 	return ;
	// while (sh->input_args[i][0] == '\0')
	// 	i++;
	// first_arg = i;
	// while (sh->input_args[i])
	// {
	// 	if (sh->input_args[i][0] == '\0')
	// 		i++;
	// 	else if (ft_strpbrk(sh->input_args[i], "|<>") != NULL)
	// 	{	
	// 	//	printf("arg[%d] (%s) contains pipe or redir!\n", i, sh->input_args[i]);
	// 		break ;
	// 	}
	// 	i++;
	// }

	// char * cmd_path = get_cmd_path(sh->input_args[first_arg], env);
	// if (is_registered_cmd(sh) || cmd_path == NULL)
	// {
	// 	free(cmd_path);
	// 	sh->last_exit_status = process_input(sh);
	// 	return ;
	// }
	// free(cmd_path);

	// Not a builtin, run normally
	if (!validate_all_redirections(sh->input_args, sh))
		return ;
	pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
	{
		sh->last_exit_status = 127;
		ft_printf("%s: command not found\n", sh->input_args[0]);
		free_pipeline(pipeline);
		return ;
	}
	exec_with_redirection(pipeline, env, sh);
	free_pipeline(pipeline);
}

void	handle_pipeline(t_shell *sh, char **env)
{
	int			status;
	t_pipeline	*pipeline;

	pipeline = build_pipeline_from_tokens(sh->input_args);
	if (!pipeline)
	{
		sh->last_exit_status = 1;
		return ;
	}
	if (has_heredoc(pipeline))
	{
		if (pipeline->cmd_count < 1)
		{
			ft_printf("Error: heredoc requires at least one command\n");
			sh->last_exit_status = 1;
			free_pipeline(pipeline);
			return ;
		}
		status = run_pipex_from_minshell(pipeline, env);
		if (status != 0)
		{
			sh->last_exit_status = status;
			free_pipeline(pipeline);
			return ;
		}
	}
	else
		run_pipeline_basic_pipeline(pipeline, env, sh);
	free_pipeline(pipeline);
}

void	handle_pipeline_with_red(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;

	pipeline = build_pipeline_from_tokens(sh->input_args);
	if (!pipeline)
	{
		sh->last_exit_status = 1;
		return ;
	}
	if (has_heredoc(pipeline))
		run_pipex_from_minshell(pipeline, env);
	else
		run_pipeline_basic_pipeline(pipeline, env, sh);
	free_pipeline(pipeline);
}

int	main(int argc, char **argv, char **env)
{
	t_shell		sh;
	static char	*line;
	t_cmd_type	type;

	line = NULL;
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
			continue ;
		sh.input_args = normalize_input(line, &sh);
		if (!sh.input_args)
			continue ;
		if (g_signal_status != 0)
		{
			sh.last_exit_status = g_signal_status;
			g_signal_status = 0;
		}
		type = classify_command(sh.input_args);
// =======

		// // What are those lines for?
		// if (line[0] == '\0')
		// 	continue; 
		// sh.input_args = normalize_input(line, &sh);
		// if (!sh.input_args)
		// 	continue;
		// ////////////////////////////

		// // int i = 0;
		// // while(sh.input_args[i])
		// // {
		// // 	printf("input_arg[%d] = %s\n", i,sh.input_args[i]);
		// // 	i++;
		// // }

		// t_cmd_type type = classify_command(sh.input_args);

		// // Handle each command type
// >>>>>>> main
		if (type == REDIR_ONLY)
		{
			handle_redir_only(&sh, env);
		}
		else if (type == BASIC)
		{
			validate_and_exec_command(sh.tokens, sh.input_args, &sh);
			handle_basic(&sh, env);
		}
		else if (type == PIPELINE || type == HERE_DOC)
		{
			handle_pipeline(&sh, env);
		}
		else if (type == PIPELINE_WITH_RED)
		{
			sh.pipeline = ft_calloc(1, sizeof(t_pipeline));
			parse_and_build_pipeline(sh.pipeline, sh.input_args);
			run_pipeline_with_redir(sh.pipeline, env, &sh);
		}
		else
		{
			sh.last_exit_status = 1;
			perror("Error: Unsupported combination of pipes and redirections");
		}
		ft_free_array(sh.input_args, -1);
	}
	free(line);
	rl_clear_history();
	return (sh.last_exit_status);
}
