/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yahya_ms_to_pipex.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:30 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/20 15:53:37 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <signal.h>

bool has_heredoc(t_pipeline *p) {
	for (int i = 0; i < p->cmd_count; i++) {
		if (p->cmds[i].limiter)
			return true;
	}
	return false;
}

bool is_builtin(const char *cmd)
{
	if (!cmd)
		return false;

	if (ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "exit") == 0 ||
		ft_strchr(cmd, '=') != NULL || // to handle env stash ("VAR_NAME=[VAR_VALUE]")
		(cmd[0] == CC_VAR_TO_INTERPRET) ||
		((cmd[0] == CC_VAR_TO_INTERPRET) && (cmd[1] == '?')) ||
		(cmd[0] == '$') ||
		((cmd[0] == '$') && (cmd[1] == '?'))
		)
		return true;
	return false;
}



int count_command_tokens(char **tokens, int start) {
	int count = 0;
	while (tokens[start] &&
	       ft_strcmp(tokens[start], (char[]){CC_PIPE, '\0'}) != 0)
	{
		if ((ft_strcmp(tokens[start], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
		ft_strcmp(tokens[start], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
		ft_strcmp(tokens[start], (char[]){CC_APPEND, '\0'}) == 0) &&
	   tokens[start + 1])
		{
			start += 2;
			continue;
		}
		count++;
		start++;
	}
	return count;
}


char **extract_command_args(char **tokens, int *i, int count) {
	char **argv = malloc(sizeof(char *) * (count + 1));
	int j = 0;

	if (!argv)
		return NULL;

	while (tokens[*i] &&
		ft_strcmp(tokens[*i], (char[]){CC_PIPE, '\0'}) != 0 &&
		ft_strcmp(tokens[*i], (char[]){CC_REDIR_IN, '\0'}) != 0 &&
		ft_strcmp(tokens[*i], (char[]){CC_REDIR_OUT, '\0'}) != 0 &&
		ft_strcmp(tokens[*i], (char[]){CC_APPEND, '\0'}) != 0)
	{
					// If it's a redirection token, skip its target
		if ((ft_strcmp(tokens[*i], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
			ft_strcmp(tokens[*i], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
			ft_strcmp(tokens[*i], (char[]){CC_APPEND, '\0'}) == 0) &&
			tokens[*i + 1])
		{
		   *i += 2;
		   continue;
	    }		   
		argv[j++] = ft_strdup(tokens[*i]);
		(*i)++;
	}
	argv[j] = NULL;
	return argv;
}


void parse_next_command(char **tokens, int *i, t_pipeline *p, int *cmd_i) {
	// Skip leading pipes (important for double pipes or pipe at start)
	while (tokens[*i] && ft_strcmp(tokens[*i], (char[]){CC_PIPE, '\0'}) == 0)
		(*i)++;
	int count = count_command_tokens(tokens, *i);
	if (count == 0)
	{
		printf("I am here 400\n");
		return;
	}

	p->cmds[*cmd_i].argv = extract_command_args(tokens, i, count);
	if (!p->cmds[*cmd_i].argv)
	{
		printf("stuck in extract commands\n");
		return;
	}

	(*cmd_i)++;

	// Skip trailing pipe, if any (in case it's still there)
	if (tokens[*i] && ft_strcmp(tokens[*i], (char[]){CC_PIPE, '\0'}) == 0)
		(*i)++;
}

int count_cmds(char **tokens) {
    int i = 0;
    int count = 0;
    int in_cmd = 0;

    while (tokens[i]) {
        if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0) {
            in_cmd = 0;  // Reset after encountering a pipe
        } else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
                   ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
                   ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0) {
            if (tokens[i + 1]) {
                i++;  // Skip the file argument
            }
            // Do not count redirections as new commands
            in_cmd = 0;
        } else {
            if (!in_cmd) {
                count++;  // Count new command only when not inside a command
                in_cmd = 1;
            }
        }
        i++;
    }

    return count;
}


static t_pipeline *init_pipeline(char **tokens) {
	t_pipeline *p = ft_calloc(1, sizeof(t_pipeline));
	if (!p)
		return NULL;

	p->cmd_count = count_cmds(tokens);
	p->cmds = ft_calloc(p->cmd_count, sizeof(t_commands));
	if (!p->cmds) {
		free(p);
		return NULL;
	}
	return p;
}

static bool handle_redirection_tokens(char **tokens, int *i, t_pipeline *p, int cmd_index) {
	if (strcmp(tokens[*i], (char[]){CC_HEREDOC, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->cmds[cmd_index].infile = ft_strdup("here_doc");
			p->cmds[cmd_index].limiter = ft_strdup(tokens[++(*i)]);
			p->cmds[cmd_index].append = false;
			(*i)++;
			return true;
		}
		return false;
	}
	
	if (strcmp(tokens[*i], (char[]){CC_REDIR_IN, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->cmds[cmd_index].infile = ft_strdup(tokens[++(*i)]);
			(*i)++;
			return true;
		}
		return false;
	}
	if (strcmp(tokens[*i], (char[]){CC_REDIR_OUT, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->cmds[cmd_index].outfile = ft_strdup(tokens[++(*i)]);
			p->cmds[cmd_index].append = false;
			(*i)++;
			return true;
		}
		return false;
	}
	if (strcmp(tokens[*i], (char[]){CC_APPEND, '\0'}) == 0) {
		if (tokens[*i + 1]) {
			p->cmds[cmd_index].outfile = ft_strdup(tokens[++(*i)]);
			p->cmds[cmd_index].append = true;
			(*i)++;
			return true;
		}
		return false;
	}
	return false;
}

t_pipeline *build_pipeline_from_tokens(char **tokens) {
    int i = 0;
    t_pipeline *p = init_pipeline(tokens);
    if (!p) 
        return NULL;

    int current_cmd = 0;
    while (tokens[i]) {
        // Handle redirections for current command
        if (handle_redirection_tokens(tokens, &i, p, current_cmd)) {
            continue;
        }
        
        // If we hit a pipe, move to next command
        if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0) {
            current_cmd++;
            i++;
            continue;
        }
        
        // Process command tokens
        if (!p->cmds[current_cmd].argv) {
            int count = count_command_tokens(tokens + i, 0);
            if (count > 0) {
                p->cmds[current_cmd].argv = malloc(sizeof(char *) * (count + 1));
                if (!p->cmds[current_cmd].argv)
                    return NULL;
                p->cmds[current_cmd].argv[0] = NULL;
            }
        }
        
        // Add argument to current command
        if (p->cmds[current_cmd].argv) {
            int arg_count = 0;
            while (p->cmds[current_cmd].argv[arg_count])
                arg_count++;
            p->cmds[current_cmd].argv[arg_count] = ft_strdup(tokens[i]);
            p->cmds[current_cmd].argv[arg_count + 1] = NULL;
        }
        i++;
    }
    return p;
}


t_pipeline *parse_redirection_only(char **tokens)
{
	t_pipeline *p = ft_calloc(1, sizeof(t_pipeline));
	t_commands *cmd = ft_calloc(1, sizeof(t_commands));
	p->cmds = cmd;
	char **argv = ft_calloc(128, sizeof(char *));

	if (!p || !cmd || !argv)
		return NULL;

	int i = 0;
	int arg_i = 0;

	while (tokens[i])
	{
		if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0)
		{
			if (tokens[i + 1])
				p->cmds->infile = ft_strdup(tokens[++i]);
			else
			{
				fprintf(stderr, "Error: missing infile\n");
				free(cmd); free(argv); free(p);
				return NULL;
			}
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0)
		{
			if (tokens[i + 1])
				p->cmds->outfile = ft_strdup(tokens[++i]), p->cmds->append = true;
			else
			{
				fprintf(stderr, "Error: missing outfile\n");
				free(cmd); free(argv); free(p);
				return NULL;
			}
		}
		else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0)
		{
			if (tokens[i + 1])
				p->cmds->outfile = ft_strdup(tokens[++i]), p->cmds->append = false;
			else
			{
				fprintf(stderr, "Error: missing outfile\n");
				free(cmd); free(argv); free(p);
				return NULL;
			}
		}
		else
		{
			argv[arg_i++] = ft_strdup(tokens[i]);
		}
		i++;
	}

	if (arg_i == 0)
	{
		fprintf(stderr, "Error: no command found\n");
		free(cmd); free(argv); free(p);
		return NULL;
	}

	argv[arg_i] = NULL;
	cmd->argv = argv;
	p->cmds = cmd;
	p->cmd_count = 1;
	// printf("parse redirection only\n");
	return p;
}

void free_pipeline(t_pipeline *p)
{
	if (!p) return;
    int i = 0;
	while (i < p->cmd_count) 
    {
		if (p->cmds[i].argv)
        {
            int j = 0;
			while (p->cmds[i].argv[j])
            {
				free(p->cmds[i].argv[j]);
                j++;
            }
			free(p->cmds[i].argv);
		}
        i++;
	}
	// free(p->cmds);
	// free(p->cmds->infile);
	// free(p->cmds->outfile);
}
char *shell_find_cmd_path(char *cmd, char **paths)
{
	int i = 0;
	char *temp;
	char *full;

	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return NULL;
		full = ft_strjoin(temp, cmd);
		free(temp);
		if (!full)
			return NULL;
		if (access(full, X_OK) == 0)
			return full;
		free(full);
		i++;
	}
	return NULL;
}
char *get_cmd_path(char *cmd, char **envp)
{
	// Case 1: command has a '/' (like ./echo or /bin/ls)
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd); // valid path
		else
		{
			// printf("YOOOOX \n");
			return NULL; // let execve fail
		}
	}

	// Case 2: search in PATH
	char *path_str = get_path_from_env(envp);
	if (!path_str)
	{
		// printf("yoooo\n");
		return NULL;
	}

	char **paths = ft_split(path_str, ':');
	if (!paths)
		return NULL;

	char *full_path = shell_find_cmd_path(cmd, paths);
	ft_free_array(paths, -1);
	return full_path;
}

static int open_redirection_fds_mixed(t_commands *cmd, int *in_fd, int *out_fd, t_shell *sh) {
	*in_fd = -1;
	*out_fd = -1;
	// printf("I am here\n");
	if (cmd->infile) {
		*in_fd = open(cmd->infile, O_RDONLY);
		if (*in_fd < 0) {
			// printf("Yo\n");
			sh->last_exit_status = 1;
			perror(cmd->infile);
			return -1;
		}
	}

	if (cmd->outfile) {
		int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
		*out_fd = open(cmd->outfile, flags, 0644);
		if (*out_fd < 0) {
			sh->last_exit_status = 1;
			perror(cmd->outfile);
			if (*in_fd != -1)
				close(*in_fd);
			return -1;
		}
	}
	return 0;
}

static int open_redirection_fds(t_pipeline *cmd, int *in_fd, int *out_fd, t_shell *sh) {
	*in_fd = -1;
	*out_fd = -1;
	// printf("I am here\n");
	if (cmd->cmds->infile) {
		*in_fd = open(cmd->cmds->infile, O_RDONLY);
		if (*in_fd < 0) {
			// printf("Yo\n");
			sh->last_exit_status = 1;
			perror(cmd->cmds->infile);
			return -1;
		}
	}

	if (cmd->cmds->outfile) {
		int flags = O_WRONLY | O_CREAT | (cmd->cmds->append ? O_APPEND : O_TRUNC);
		*out_fd = open(cmd->cmds->outfile, flags, 0644);
		if (*out_fd < 0) {
			sh->last_exit_status = 1;
			perror(cmd->cmds->outfile);
			if (*in_fd != -1)
				close(*in_fd);
			return -1;
		}
	}
	return 0;
}

static void setup_redirections(int in_fd, int out_fd) {
	if (in_fd != -1) {
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1) {
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

void exec_with_redirection(t_pipeline *cmd, char **env, t_shell *sh) {
	int in_fd;
	int out_fd;
	if (open_redirection_fds(cmd, &in_fd, &out_fd, sh) < 0)
	{
		printf("Invalid file\n");	
		return;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		setup_redirections(in_fd, out_fd);
		if (cmd->cmd_count < 1)
			exit(0);
		char **argv = cmd->cmds[0].argv;
		if (validate_and_exec_command(argv, env, sh))
		{
			exit(sh->last_exit_status);
		}
		if (strcmp(argv[0], "echo") == 0)
		{
			sh->input_args = argv;  // temporarily point to the correct argv
			exit(cmd_echo(sh));
		}
		else
			execve(get_cmd_path(argv[0], env), argv, env);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}

	int status;
	status = 0;
	waitpid(pid, &status, 0);

	if (WIFEXITED(status))
		sh->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sh->last_exit_status = 128 + WTERMSIG(status);

	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);
}

t_cmd_type classify_command(char **tokens)
{
    int i = 0;
    int has_pipe = 0;
    int has_redir = 0;
    int here_doc = 0;
    int has_cmd = 0;
    int first_is_redir = 0;  // Track if first token is a redirection

    // Check if first token is a redirection
    if (tokens[0] && (ft_strcmp(tokens[0], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
                      ft_strcmp(tokens[0], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
                      ft_strcmp(tokens[0], (char[]){CC_APPEND, '\0'}) == 0))
        first_is_redir = 1;

    while (tokens[i])
    {
        if (ft_strcmp(tokens[i], (char[]){CC_PIPE, '\0'}) == 0)
            has_pipe = 1;
        else if (ft_strcmp(tokens[i], (char[]){CC_HEREDOC, '\0'}) == 0)
            here_doc = 1;
        else if (ft_strcmp(tokens[i], (char[]){CC_REDIR_IN, '\0'}) == 0 ||
                 ft_strcmp(tokens[i], (char[]){CC_REDIR_OUT, '\0'}) == 0 ||
                 ft_strcmp(tokens[i], (char[]){CC_APPEND, '\0'}) == 0)
        {
            has_redir = 1;
            if (!tokens[i + 1])
                return MIXED_INVALID;
            i++; // skip the redirection target
        }
        else if ((unsigned char)tokens[i][0] >= 32) // not a control char
            has_cmd = 1;
        i++;
    }

    if(here_doc)
        return HERE_DOC;
    if (has_pipe && has_redir)
        return PIPELINE_WITH_RED;
    if (has_pipe)
        return PIPELINE;
    if (has_redir)
    {
        if (first_is_redir)
            return BASIC;  // Only return BASIC if first token is a redirection
        return REDIR_ONLY;  // All other redirection cases
    }
    if(has_cmd)
        return BASIC;
    return BASIC;
}


// char **clean_env(char **env) {
// 	int count = 0;
// 	for (int i = 0; env[i]; i++) {
// 		if (strncmp(env[i], "SHLVL=", 6) == 0 || env[i][0] == '_')
// 			continue;
// 		count++;
// 	}

// 	char **filtered = malloc(sizeof(char *) * (count + 1));
// 	if (!filtered)
// 		return NULL;

// 	int j = 0;
// 	for (int i = 0; env[i]; i++) {
// 		if (strncmp(env[i], "SHLVL=", 6) == 0 || env[i][0] == '_')
// 			continue;
// 		filtered[j++] = strdup(env[i]);
// 	}
// 	filtered[j] = NULL;
// 	return filtered;
// }

// void print_env(t_list *env)
// {
//     while (env)
//     {
//         char *entry = (char *)env->content;
//         if (entry && ft_strchr(entry, '=') && ft_strncmp(entry, "_=", 2) != 0)
//             printf("%s\n", entry);
//         env = env->next;
//     }
// }
int cmd_echo_x(char **argv) {
    int i = 1;  // Skip the command name
    while (argv[i]) {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    return 0;
}

int exec_builtin_in_child(char **argv, t_shell *sh)
{
    if (ft_strcmp(argv[0], "echo") == 0)
        return cmd_echo_x(argv);  // Pass the command's argv directly
    else if (ft_strcmp(argv[0], "pwd") == 0)
        return cmd_pwd();
    else if (ft_strcmp(argv[0], "env") == 0)
        return cmd_env(sh);
    else if (argv[0][0] == CC_VAR_TO_INTERPRET && argv[0][1] == '?' && !argv[0][2])
    {
        perror(" ");
        sh->last_exit_status = 1;
        return 0;
    }
    return 1; // Not a safe builtin for child process
}

static bool validate_pipeline_commands(t_pipeline *p, t_shell *sh) {
    for (int i = 0; i < p->cmd_count; i++) {
        char **argv = p->cmds[i].argv;
        if (!argv || !argv[0])
            continue;

        // Check for builtin commands that require valid arguments
        if (strcmp(argv[0], "cd") == 0) {
            if (!argv[1]) {
                fprintf(stderr, "cd: missing argument\n");
                sh->last_exit_status = 1;
                return false;
            }
            struct stat st;
            if (stat(argv[1], &st) != 0) {
                fprintf(stderr, "cd: %s: No such file or directory\n", argv[1]);
                sh->last_exit_status = 1;
                return false;
            }
            if (!S_ISDIR(st.st_mode)) {
                fprintf(stderr, "cd: %s: Not a directory\n", argv[1]);
                sh->last_exit_status = 1;
                return false;
            }
        }
        // Add other builtin validations here if needed
    }
    return true;
}

void run_pipeline_with_redir(t_pipeline *p, char **env, t_shell *sh) {
    int i = 0;
    int prev_fd = -1;
    int pipe_fd[2];
    pid_t last_pid = -1;

    // Set up signal handling for parent
    // signal(SIGINT, SIG_IGN);   // Parent ignores CTRL+C
    // signal(SIGQUIT, SIG_IGN);  // Parent ignores CTRL+

    while (i < p->cmd_count) {
        if (i < p->cmd_count - 1)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror(" ");
                exit(EXIT_FAILURE);
            }
        }
        pid_t pid = fork();
        if (pid == 0) {
            // Restore default signal handling in child
            // signal(SIGINT, SIG_DFL);
            // signal(SIGQUIT, SIG_DFL);

            int in_fd = -1, out_fd = -1;

            if (open_redirection_fds_mixed(&p->cmds[i], &in_fd, &out_fd, sh) < 0)
                exit(1);

            // Setup input (prioritize redirection over pipe)
            if (in_fd != -1) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            } else if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
            }

            // Setup output (prioritize redirection over pipe)
            if (out_fd != -1) {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            } else if (i < p->cmd_count - 1) {
                dup2(pipe_fd[1], STDOUT_FILENO);
            }

            // Close all pipe fds in child
            if (prev_fd != -1)
                close(prev_fd);
            if (i < p->cmd_count - 1) {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            // Now run builtin *after* redirection is set
            if (is_builtin(p->cmds[i].argv[0]))
            {
                exit(exec_builtin_in_child(p->cmds[i].argv, sh));
            }

            // External command fallback
            char *cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
            if (!cmd_path)
                exit(127);
            // char **cleaned_env = clean_env(env);
            // if (!cleaned_env)
            //     exit(1);
            execve(cmd_path, p->cmds[i].argv, env);
            perror("execve failed");
            exit(EXIT_FAILURE);
        }
        else {
            if (i == p->cmd_count - 1)
                last_pid = pid;
        }

        // Parent process cleanup
        if (prev_fd != -1)
            close(prev_fd);
        if (i < p->cmd_count - 1)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        i++;
    }

    // Wait for all children
    i = 0;
    int status;
    while (i < p->cmd_count)
    {
        pid_t wpid = wait(&status);
        if (wpid == -1)
            break;
    
        if (wpid == last_pid) {
            if (WIFEXITED(status))
                sh->last_exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                if (sig == SIGINT) {
                    sh->last_exit_status = 130;
                    printf("\n");
                }
                else if (sig == SIGQUIT) {
                    sh->last_exit_status = 131;
                    printf("Quit (core dumped)\n");
                }
                else
                    sh->last_exit_status = 128 + sig;
            }
        }
        i++;
    }

    // Restore signal handlers
    init_signals(sh);
}
void run_pipeline_basic_pipeline(t_pipeline *p, char **env, t_shell *sh) {
    // Validate commands before executing
    if (!validate_pipeline_commands(p, sh))
        return;

    // Check if all commands exist before starting the pipeline
    for (int i = 0; i < p->cmd_count; i++) {
        if (!is_builtin(p->cmds[i].argv[0])) {
            char *cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
            if (!cmd_path) {
                fprintf(stderr, "%s: command not found\n", p->cmds[i].argv[0]);
                sh->last_exit_status = 127;
                return;
            }
            free(cmd_path);
        }
    }

    int i = 0;
    int prev_fd = -1;
    int pipe_fd[2];
    
    while (i < p->cmd_count) {
        if (i < p->cmd_count - 1) {
            if (pipe(pipe_fd) < 0) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == 0) {  // Child
            // Handle cat without arguments
            if (strcmp(p->cmds[i].argv[0], "cat") == 0 && !p->cmds[i].argv[1]) {
                signal(SIGPIPE, SIG_IGN);
                // Redirect both stdout and stderr to /dev/null if not the last command
                if (i < p->cmd_count - 1) {
                    int dev_null = open("/dev/null", O_WRONLY);
                    if (dev_null != -1) {
                        dup2(dev_null, STDOUT_FILENO);
                        dup2(dev_null, STDERR_FILENO);
                        close(dev_null);
                    }
                }
            }

            // Input
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            // Output (only if not the last command)
            if (i < p->cmd_count - 1) {
                close(pipe_fd[0]); // close unused read
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            // Exec
            if (is_builtin(p->cmds[i].argv[0])) {
                exit(exec_builtin_in_child(p->cmds[i].argv, sh));
            }

            char *cmd_path = get_cmd_path(p->cmds[i].argv[0], env);
            if (!cmd_path)
                exit(127);

            execve(cmd_path, p->cmds[i].argv, env);
            perror("execve failed");
            exit(EXIT_FAILURE);
        }

        // Parent
        if (prev_fd != -1)
            close(prev_fd);
        if (i < p->cmd_count - 1) {
            close(pipe_fd[1]); // close unused write
            prev_fd = pipe_fd[0]; // save read end for next command
        }
        i++;
    }

    // Wait for all
    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status))
            sh->last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            sh->last_exit_status = 128 + WTERMSIG(status);
    }
}
