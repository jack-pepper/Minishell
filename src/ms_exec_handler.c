/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 10:58:30 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/05 12:45:39 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_redir_only(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;

	if (!validate_all_redirections(sh->input_args, sh))
		return ;
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
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv
		|| ft_strcmp(sh->input_args[0], "..") == 0
		|| ft_strcmp(sh->input_args[0], ".") == 0) // we might be able to solve some other errors here
	{
		if (is_token_control_char(sh->input_args[0], CC_REDIR_OUT)
			|| is_token_control_char(sh->input_args[0], CC_APPEND)
			|| is_token_control_char(sh->input_args[0], CC_REDIR_IN)
			|| is_token_control_char(sh->input_args[0], CC_HEREDOC))
		{
			free_pipeline(pipeline);
			return ;
		}
		sh->last_exit_status = ms_err("", sh->input_args[0], CMD_NOT_FOUND, 127);
		free_pipeline(pipeline);
		return ;
	}
	exec_with_redirection(pipeline, env, sh);
	free_pipeline(pipeline);
}

bool	handle_heredoc_pipeline(t_pipeline *pipeline, char **env, t_shell *sh)
{
	int	status;

	if (pipeline->cmd_count < 1)
	{
		ft_printf("Error: heredoc requires at least one command\n");
		sh->last_exit_status = 1;
		return (false);
	}
	status = run_pipex_from_minshell(pipeline, env);
	if (status != 0)
	{
		sh->last_exit_status = status;
		return (false);
	}
	return (true);
}

void	handle_pipeline(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;

	pipeline = build_pipeline_from_tokens(sh->input_args);
	if (!pipeline)
	{
		sh->last_exit_status = 1;
		return ;
	}
	if (has_heredoc(pipeline))
	{
		if (!handle_heredoc_pipeline(pipeline, env, sh))
		{
			free_pipeline(pipeline);
			return ;
		}
	}
	else
		run_pipeline_basic_pipeline(pipeline, env, sh);
	free_pipeline(pipeline);
}
