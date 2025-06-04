/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 10:58:30 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/04 14:19:32 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redir_only(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;
	// int			new_file;

	if (!validate_all_redirections(sh->input_args, sh))
		return ;
	// if (is_token_control_char(sh->input_args[0], CC_REDIR_IN))
	// {
	// 	printf(" I am here\n");
	// 	new_file = open(sh->input_args[1], O_WRONLY
	// 			| O_CREAT | O_TRUNC, 0644);
	// 	if (new_file == -1)
	// 		perror("open");
	// 	else
	// 		close(new_file);
	// 	return ;
	// }
	pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
	{
		sh->last_exit_status = 1;
		ft_putstr_fd(" Invalid redirection command\n", 2);
	
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
			if (!tokens[i + 1])
			{
				ft_putstr_fd(" Error: missing input file\n", 2);
				sh->last_exit_status = 1;
				return (false);
			}
			restore_quoted_spaces(tokens[i + 1]);
			if (access(tokens[i + 1], R_OK) != 0)
			{
				perror(tokens[i + 1]);
				sh->last_exit_status = 1;
				return (false);
			}
			i++;
		}
		else if (is_token_control_char(tokens[i], CC_REDIR_OUT)
			|| is_token_control_char(tokens[i], CC_APPEND))
		{
			if (!tokens[i + 1])
			{
				ft_putstr_fd(" Error: missing output file\n", 2);
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
		else if (is_token_control_char(tokens[i], CC_PIPE))
		{
			if (!tokens[i + 1])
			{
				ft_putstr_fd(" Error: missing command after pipe\n", 2);
				sh->last_exit_status = 1;
				return (false);
			}
		}
		i++;
	}
	return (true);
}
