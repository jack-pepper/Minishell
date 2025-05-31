/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:29:54 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/30 16:57:01 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	open_redirection_fds(t_pipeline *cmd,
				int *in_fd, int *out_fd, t_shell *sh)
{
	int	flags;

	*in_fd = -1;
	*out_fd = -1;
	if (cmd->cmds->infile)
	{
		*in_fd = open_fd(cmd->cmds->infile, O_RDONLY, sh);
		if (*in_fd < 0)
			return (-1);
	}
	if (cmd->cmds->outfile)
	{
		flags = get_output_flags(cmd->cmds->append);
		*out_fd = open_fd(cmd->cmds->outfile, flags, sh);
		if (*out_fd < 0)
		{
			if (*in_fd != -1)
				close(*in_fd);
			return (-1);
		}
	}
	return (0);
}

void	update_exit_status(t_shell *sh, int status)
{
	if (WIFEXITED(status))
		sh->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit_status = 128 + WTERMSIG(status);
}

static void	execute_command(t_pipeline *cmd, char **env, t_shell *sh)
{
	char	**argv;
	char	*cmd_path;

	argv = cmd->cmds[0].argv;
	if (validate_and_exec_command(argv, env, sh))
		exit(sh->last_exit_status);
	if (strcmp(argv[0], "echo") == 0)
	{
		sh->input_args = argv;
		exit(cmd_echo(sh));
	}
	else
	{
		cmd_path = get_cmd_path(argv[0], env);
		execve(cmd_path, argv, env);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
}

void	exec_with_redirection(t_pipeline *cmd, char **env, t_shell *sh)
{
	int			in_fd;
	int			out_fd;
	pid_t		pid;
	int			status;

	if (open_redirection_fds(cmd, &in_fd, &out_fd, sh) < 0)
	{
		printf("Invalid file\n");
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		setup_redirections(in_fd, out_fd);
		if (cmd->cmd_count < 1)
			exit(0);
		execute_command(cmd, env, sh);
	}
	status = 0;
	waitpid(pid, &status, 0);
	update_exit_status(sh, status);
	close_fds(in_fd, out_fd);
}

int	exec_builtin_in_child(char **argv, t_shell *sh)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (cmd_echo_x(argv));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (cmd_pwd());
	else if (ft_strcmp(argv[0], "env") == 0)
		return (cmd_env(sh));
	else if (argv[0][0] == CC_VAR_TO_INTERPRET
				&& argv[0][1] == '?' && !argv[0][2])
	{
		perror(" ");
		sh->last_exit_status = 1;
		return (0);
	}
	return (1);
}
