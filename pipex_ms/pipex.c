/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:55:38 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/04/13 14:43:24 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


void	ft_init_pipex(t_pipex *pipex, char *infile, char *outfile)
{
	pipex->in_fd = open(infile, O_RDONLY);
	if (pipex->in_fd == -1)
		perror(infile);
	pipex->out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->out_fd == -1)
		perror(outfile);
	pipex->cmd_args = NULL;
	pipex->cmd_paths = NULL;
	pipex->cmd_count = 0;
	pipex->here_doc = false;
}

void	ft_execute_pipex(t_pipex *pipex)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(pipefd) == -1)
		ft_exit_error(pipex, "pipe failed");
	pid1 = fork();
	if (pid1 == -1)
		ft_exit_error(pipex, "fork failed");
	if (pid1 == 0)
		execute_first_child(pipex, pipefd);
	pid2 = fork();
	if (pid2 == -1)
		ft_exit_error(pipex, "fork failed");
	if (pid2 == 0)
		execute_second_child(pipex, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	pipex->exit_status = WEXITSTATUS(status);
}

void	execute_child(t_pipex *pipex, int in_fd, int out_fd, char **cmd)
{
	char	*cmd_path;

	if (dup2(in_fd, STDIN_FILENO) == -1 || dup2(out_fd, STDOUT_FILENO) == -1)
		ft_exit_error(pipex, "dup2 error");
	cmd_path = find_cmd_path(cmd[0], pipex->cmd_paths);
	if (!cmd_path)
	{
		ft_printf("%s: command not found\n", cmd[0]);
		free_pipex(pipex);
		exit(127);
	}
	execve(cmd_path, cmd, pipex->envp);
	perror("execve");
	free(cmd_path);
	exit(1);
}

void	handle_mandatory(t_pipex *pipex, char **argv, int argc)
{
	ft_init_pipex(pipex, argv[1], argv[argc - 1]);
	ft_parse_cmds(pipex, argv);
	ft_parse_paths(pipex);
	ft_execute_pipex(pipex);
	free_pipex(pipex);
}
static char *join_args(char **args)
{
	int i;
	size_t total_len = 0;
	char *result;
	
	i = 0;
	// First calculate total length
	while(args[i])
	{
		total_len += strlen(args[i]) + 1; // +1 for space or '\0'
		i++;
	}

	if (total_len == 0)
		return (strdup(""));

	result = malloc(total_len);
	if (!result)
		return (NULL);

	result[0] = '\0';
	i = 0;
	while (args[i]) 
	{
		strcat(result, args[i]);
		if (args[i + 1])
			strcat(result, " ");
		i++;
	}
	return result;
}

int run_pipex_from_minshell(t_pipeline *pipeline, char **envp)
{
	t_pipex pipex;
	char **argv;
	int argc;
	int i;
	int k = 0;

	if (!pipeline->infile || !pipeline->outfile)
	{
//		ft_printf("Error: missing infile or outfile\n");
		return (1);
	}

	pipex = (t_pipex){0};
	pipex.envp = envp;

	// Added by [m] for $? support
	pipeline->pipex = malloc(sizeof(pipex));
	if (!pipeline->pipex)
		return (-1);
	pipeline->pipex = &pipex;
	// End added by [m]

	argc = 3 + pipeline->cmd_count; // pipex + infile + N cmds + outfile

	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
	{
		perror("malloc failed");
		return (1);
	}

	argv[k++] = strdup("pipex");                // argv[0]
	argv[k++] = strdup(pipeline->infile);       // argv[1]

	i = 0;
	// Join all commands into a single string
	while (i < pipeline->cmd_count)
	{
		argv[k++] = join_args(pipeline->cmds[i].argv); // argv[2] to argv[n]
		i++;
	}

	argv[k++] = strdup(pipeline->outfile);      // argv[n+1]
	argv[k] = NULL;

	// Dispatch to the correct pipex handler
	if (argc >= 6 && ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		printf("here_doc\n");
		handle_here_doc(&pipex, argc, argv);
	}
	else if (argc == 5)
	{
		printf("mandatory\n");
		handle_mandatory(&pipex, argv, argc);
	}
	else if (argc > 5)
	{
		printf("bonus\n");
		handle_bonus(&pipex, argc, argv);
	}
	else
	{
		ft_printf("Invalid input to pipex\n");
		// free argv
		i = 0;
		while (i < argc)
			free(argv[i++]);
		free(argv);
		return (1);
	}

	// Cleanup
	i = 0;
	while (i < argc)
		free(argv[i++]);
	free(argv);

	printf("[DEBUG - runpipexfromminshell()] PIPEX EXIT STATUS: %d\n", pipex.exit_status);

	return pipex.exit_status;
}
// int	main(int argc, char **argv, char **envp)
// {
// 	t_pipex	pipex;

// 	pipex = (t_pipex){0};
// 	pipex.envp = envp;
// 	if (argc >= 6 && ft_strcmp(argv[1], "here_doc") == 0)
// 		handle_here_doc(&pipex, argc, argv);
// 	else if (argc == 5)
// 		handle_mandatory(&pipex, argv, argc);
// 	else if (argc > 5)
// 		handle_bonus(&pipex, argc, argv);
// 	else
// 	{
// 		ft_printf("Usage:\n");
// 		ft_printf("./pipex infile cmd1 cmd2 outfile\n");
// 		ft_printf("./pipex here_doc LIMITER cmd1 ... outfile\n");
// 		return (1);
// 	}
// 	return (pipex.exit_status);
// }
