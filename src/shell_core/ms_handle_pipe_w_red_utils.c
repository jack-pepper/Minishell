/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_pipe_w_red_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 07:13:38 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/05 09:44:24 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pipeline_fork_loop(t_pipeline *p, char **env, t_shell *sh)
{
	int		i;
	int		prev_fd;
	int		pipe_fd[2];
	pid_t	last_pid;
	pid_t	pid;

	last_pid = -1;
	prev_fd = -1;
	i = 0;
	while (i < p->cmd_count)
	{
		if (i < p->cmd_count - 1 && pipe(pipe_fd) < 0)
		{
			perror(" ");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == 0)
			execute_child_pipeline_cmd(p, env, sh, i, prev_fd, pipe_fd);
		else if (i == p->cmd_count - 1)
			last_pid = pid;
		handle_parent_pipe_closing(&prev_fd, pipe_fd, i, p->cmd_count);
		i++;
	}
	return (last_pid);
}

void	exit_if_redirection_failed(int redir_status, int is_last_cmd)
{
	if (redir_status < 0)
	{
		if (is_last_cmd)
			exit(1);
		else
			exit(0);
	}
}

void	execute_child_pipeline_cmd(t_pipeline *p, char **env,
			t_shell *sh, int i, int prev_fd, int pipe_fd[2])
{
	int		in_fd;
	int		out_fd;
	char	*cmd_path;
	int		redir_status;

	out_fd = -1;
	in_fd = -1;
	redir_status = open_redirection_fds_mixed(&p->cmds[i], &in_fd, &out_fd, sh);
	exit_if_redirection_failed(redir_status, i == p->cmd_count - 1);
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	else if (prev_fd != -1)
		dup2(prev_fd, STDIN_FILENO);
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	else if (i < p->cmd_count - 1)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (prev_fd != -1)
		close(prev_fd);
	if (i < p->cmd_count - 1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (!p->cmds[i].argv || !p->cmds[i].argv[0])
		exit(1);
	if (is_builtin(p->cmds[i].argv[0]))
		exit(exec_builtin_in_child(p->cmds[i].argv, sh));
	cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
	if (!cmd_path)
		exit(127);
	execve(cmd_path, p->cmds[i].argv, env);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void	handle_parent_pipe_closing(int *prev_fd,
	int pipe_fd[2], int i, int cmd_count)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (i < cmd_count - 1)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

void	run_pipeline_with_redir(t_pipeline *p, char **env, t_shell *sh)
{
	int		last_pid;

	last_pid = pipeline_fork_loop(p, env, sh);
	wait_for_pipeline(p, last_pid, sh);
}
