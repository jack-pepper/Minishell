/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_basic_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/04 12:38:18 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_echo_x(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}

static void	setup_child_fds(int prev_fd, int i, int total, int pipe_fd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (i < total - 1)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

static void	close_parent_pipe_ends(int i, int *prev_fd,
			int pipe_fd[2], int total)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (i < total - 1)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
}

static void	wait_for_all_children_basic(t_shell *sh)
{
	int	status;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			sh->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			sh->last_exit_status = 128 + WTERMSIG(status);
	}
}

static void	exec_pipeline_child_pipe(t_pipeline *p, char **env, t_shell *sh,
	int i, int prev_fd, int pipe_fd[2])
{
	char	*cmd_path;

	// handle_cat_no_args(&p->cmds[i], i, p->cmd_count);
	setup_child_fds(prev_fd, i, p->cmd_count, pipe_fd);
	if (is_builtin(p->cmds[i].argv[0]))
		exit(exec_builtin_in_child(p->cmds[i].argv, sh));
	cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
	if (!cmd_path)
		exit(127);
	execve(cmd_path, p->cmds[i].argv, env);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

void	run_pipeline_basic_pipeline(t_pipeline *p, char **env, t_shell *sh)
{
	int		i;
	int		prev_fd;
	int		pipe_fd[2];
	pid_t	pid;

	prev_fd = -1;
	if (!validate_pipeline_commands(p, sh))
		return ;
	if (!validate_all_non_builtins(p, env, sh))
		return ;
	i = 0;
	while (i < p->cmd_count)
	{
		open_pipe_if_needed(i, p->cmd_count, pipe_fd);
		pid = fork();
		if (pid == 0)
			exec_pipeline_child_pipe(p, env, sh, i, prev_fd, pipe_fd);
		close_parent_pipe_ends(i, &prev_fd, pipe_fd, p->cmd_count);
		i++;
	}
	wait_for_all_children_basic(sh);
}
