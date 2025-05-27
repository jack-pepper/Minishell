/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_handle_pipe_with_red.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/27 16:27:18 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static int	open_redirection_fds_mixed(t_commands *cmd,
	int *in_fd, int *out_fd, t_shell *sh)
{
int	flags;

*in_fd = -1;
*out_fd = -1;
if (cmd->infile)
{
*in_fd = open(cmd->infile, O_RDONLY);
if (*in_fd < 0)
{
sh->last_exit_status = 1;
perror(cmd->infile);
return (-1);
}
}
if (cmd->outfile)
{
flags = O_WRONLY | O_CREAT;
if (cmd->append)
flags |= O_APPEND;
else
flags |= O_TRUNC;
*out_fd = open(cmd->outfile, flags, 0644);
if (*out_fd < 0)
{
sh->last_exit_status = 1;
perror(cmd->outfile);
if (*in_fd != -1)
	close(*in_fd);
return (-1);
}
}
return (0);
}

void	run_pipeline_with_redir(t_pipeline *p, char **env, t_shell *sh)
{
	int		i;
	int		prev_fd;
	int		pipe_fd[2];
	pid_t	last_pid;
	char	*cmd_path;
	int		status;
	int		sig;
	pid_t	wpid;
	int		in_fd;
	int		out_fd;
	pid_t	pid;

	prev_fd = -1;
	i = 0;
	last_pid = -1;
	while (i < p->cmd_count)
	{
		if (i < p->cmd_count - 1)
		{
			if (pipe(pipe_fd) < 0)
			{
				perror(" ");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == 0)
		{
			in_fd = -1;
			out_fd = -1;
			if (open_redirection_fds_mixed(&p->cmds[i],
					&in_fd, &out_fd, sh) < 0)
			{
				if (i == p->cmd_count - 1)
					exit(1);
				exit(0);
			}
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
		else
			if (i == p->cmd_count - 1)
				last_pid = pid;
		if (prev_fd != -1)
			close(prev_fd);
		if (i < p->cmd_count - 1)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
		i++;
	}
	i = 0;
	while (i < p->cmd_count)
	{
		wpid = wait(&status);
		if (wpid == -1)
			break ;
		if (wpid == last_pid)
		{
			if (WIFEXITED(status))
				sh->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig == SIGINT)
				{
					sh->last_exit_status = 130;
					printf("\n");
				}
				else if (sig == SIGQUIT)
				{
					sh->last_exit_status = 131;
					printf("Quit (core dumped)\n");
				}
				else
					sh->last_exit_status = 128 + sig;
			}
		}
		i++;
	}
	init_signals(sh);
}
