/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 23:24:52 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/06 16:12:30 by mmalie           ###   ########.fr       */
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

int	case_no_pipeline_needed(t_shell *sh, char **env_arr)
{
	if (!sh->input_args[0])
		return (1);
	if (is_builtin(sh->input_args[0]))
	{
		sh->last_exit_status = process_input(sh);
		return (1);
	}
	if (!validate_all_redirections(sh->input_args, sh))
		return (1);
	if (ft_strcmp(sh->input_args[0], ".") == 0
		|| ft_strcmp(sh->input_args[0], "..") == 0)
	{
		sh->last_exit_status = ms_err(
				"", sh->input_args[0], CMD_NOT_FOUND, 127);
		return (1);
	}
	if (ft_isalnum_x_str(sh->input_args[0], "!#$%&()*+,'\"-:;?@[\\]^{}~.") == 0)
	{
		if (validate_in_path(sh->input_args, env_arr, sh))
			return (1);
	}
	return (0);
}

void	handle_basic(t_shell *sh, char **env)
{
	t_pipeline	*pipeline;
	char		**env_arr;

	env_arr = env_list_to_array(sh->this_env);
	if (case_no_pipeline_needed(sh, env_arr) != 0)
	{
		free_args(env_arr);
		return ;
	}
	free_args(env_arr);
	pipeline = parse_redirection_only(sh->input_args);
	if (!pipeline || !pipeline->cmds || !pipeline->cmds[0].argv)
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
