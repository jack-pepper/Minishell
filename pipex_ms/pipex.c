/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:55:38 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/20 12:00:35 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


// void	ft_init_pipex(t_pipex *pipex, char *infile, char *outfile)
// {
// 	pipex->in_fd = open(infile, O_RDONLY);
// 	if (pipex->in_fd == -1)
// 		perror(infile);
// 	pipex->out_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (pipex->out_fd == -1)
// 		perror(outfile);
// 	pipex->cmd_args = NULL;
// 	pipex->cmd_paths = NULL;
// 	pipex->cmd_count = 0;
// 	pipex->here_doc = false;
// }
void	ft_init_pipex(t_pipex *pipex, char *infile, char *outfile)
{
	if (!pipex->here_doc)
	{
		pipex->in_fd = open(infile, O_RDONLY);
		if (pipex->in_fd == -1)
			perror(infile);
	}

	pipex->out_fd = open(outfile,
		O_WRONLY | O_CREAT | (pipex->here_doc ? O_APPEND : O_TRUNC),
		0644);

	if (pipex->out_fd == -1)
		perror(outfile);

	pipex->cmd_args = NULL;
	pipex->cmd_paths = NULL;
	pipex->cmd_count = 0;
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
// static char *join_args(char **args)
// {
//     if (!args || !args[0])
//         return strdup("");

//     int i;
//     size_t total_len = 0;
//     char *result;
    
//     i = 0;
//     // First calculate total length
//     while(args[i])
//     {
//         if (args[i])  // Add NULL check for each argument
//             total_len += strlen(args[i]) + 1; // +1 for space or '\0'
//         i++;
//     }

//     if (total_len == 0)
//         return strdup("");

//     result = malloc(total_len);
//     if (!result)
//         return NULL;

//     result[0] = '\0';
//     i = 0;
//     while (args[i]) 
//     {
//         if (args[i])  // Add NULL check for each argument
//         {
//             strcat(result, args[i]);
//             if (args[i + 1])
//                 strcat(result, " ");
//         }
//         i++;
//     }
//     return result;
// }
int handle_heredoc(const char *limiter)
{
    char *line = NULL;
    int fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return (1);
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, limiter) == 0)
            break;
        write(fd, line, strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    free(line);
    close(fd);
    return (0);
}

int run_pipex_from_minshell(t_pipeline *pipeline, char **envp)
{
    if (!pipeline || !pipeline->cmds->infile)
    {
        ft_printf("Invalid pipeline\n");
        return (1);
    }

    if (ft_strcmp(pipeline->cmds->infile, "here_doc") != 0)
    {
        ft_printf("run_pipex_from_minshell is for here_doc only\n");
        return (1);
    }

    t_pipex pipex = {0};
    int i = 0;

    // Initialize pipex structure
    pipex.envp = envp;
    pipex.here_doc = true;
    pipex.cmd_args = NULL;
    pipex.cmd_paths = NULL;
    pipex.cmd_count = pipeline->cmd_count;
    pipex.in_fd = -1;
    pipex.out_fd = -1;

    if (handle_heredoc(pipeline->cmds->limiter) != 0)
    {
        ft_printf("Error: here_doc failed\n");
        return (1);
    }

    pipex.in_fd = open(".heredoc_tmp", O_RDONLY);
    if (pipex.in_fd == -1)
    {
        perror("open .heredoc_tmp");   
        return (1);
    }

    // Open the output file
    if (pipeline->cmds[pipeline->cmd_count - 1].outfile)
    {
        t_commands *last_cmd = &pipeline->cmds[pipeline->cmd_count - 1];
        int flags = O_WRONLY | O_CREAT;

        if (last_cmd->append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;

        pipex.out_fd = open(last_cmd->outfile, flags, 0644);
        if (pipex.out_fd == -1)
        {
            close(pipex.in_fd);
            perror(pipeline->cmds[pipeline->cmd_count - 1].outfile);
            return (1);
        }
    }
    else
    {
        pipex.out_fd = STDOUT_FILENO;
    }

    // Allocate and initialize cmd_args
    pipex.cmd_args = malloc(sizeof(char **) * pipex.cmd_count);
    if (!pipex.cmd_args)
    {
        close(pipex.in_fd);
        if (pipex.out_fd != STDOUT_FILENO)
            close(pipex.out_fd);
        perror("malloc");
        return (1);
    }

    // Copy command arguments
    i = 0;
    while (i < pipeline->cmd_count)
    {
        if (!pipeline->cmds[i].argv)
        {
            int j = 0;
            while (j < i)
            {
                ft_free_2d_array(pipex.cmd_args[j]);
                j++;
            }
            free(pipex.cmd_args);
            close(pipex.in_fd);
            if (pipex.out_fd != STDOUT_FILENO)
                close(pipex.out_fd);
            return 1;
        }
        // Copy argv array
        int arg_count = 0;
        while (pipeline->cmds[i].argv[arg_count])
            arg_count++;
        
        pipex.cmd_args[i] = malloc(sizeof(char *) * (arg_count + 1));
        if (!pipex.cmd_args[i])
        {
            int j = 0;
            while (j < i)
            {
                ft_free_2d_array(pipex.cmd_args[j]);
                j++;
            }
            free(pipex.cmd_args);
            close(pipex.in_fd);
            if (pipex.out_fd != STDOUT_FILENO)
                close(pipex.out_fd);
            perror("malloc");
            return (1);
        }
        
        int j = 0;
        while (j < arg_count)
        {
            pipex.cmd_args[i][j] = ft_strdup(pipeline->cmds[i].argv[j]);
            if (!pipex.cmd_args[i][j])
            {
                int k = 0;
                while (k < j)
                {
                    free(pipex.cmd_args[i][k]);
                    k++;
                }
                free(pipex.cmd_args[i]);
                int l = 0;
                while (l < i)
                {
                    ft_free_2d_array(pipex.cmd_args[l]);
                    l++;
                }
                free(pipex.cmd_args);
                close(pipex.in_fd);
                if (pipex.out_fd != STDOUT_FILENO)
                    close(pipex.out_fd);
                perror("malloc");
                return (1);
            }
            j++;
        }
        pipex.cmd_args[i][j] = NULL;
        i++;
    }

    // Parse paths and execute
    ft_parse_paths(&pipex);
    execute_multiple_cmds(&pipex);

    // Clean up
    free_pipex(&pipex);

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
