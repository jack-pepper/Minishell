/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:35:22 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/30 17:38:09 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PATH_TO_HEADER "../inc/minishell.h"
# define NB_CMDS 10 // Temporary, find out dynamically. DEBUG

# include "../pipex_ms/pipex.h"


/* Standard libraries */

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <stdlib.h>
# include <curses.h>
# include <term.h>

/* Custom libraries */
# include "../libft/libft.h"

/* Structures */

typedef struct s_command
{
	char *name;
	void *func;
	char *doc;
} 		t_command;

// Could be initialized like this:
/*
t_command commands[] = {
                { "pwd", cmd_pwd, "Print name of current/working directory" },
                { "cd", cmd_cd, "Change the working directory" }, 
  		{ "echo", cmd_echo, "Display a line of text" }, 
                { "exit", cmd_exit, "Cause the shell to exit" },
                { "export", cmd_export, "Set the export attribute for variables" },
                { "unset", cmd_unset, "Unset values and attributes of variables and functions" },
                { "env", cmd_env, "Display the env variables" },
		{ "help", cmd_help, "Display this text" },
                { (char *)NULL, (Function *)NULL, (char *)NULL } /// should allow to loop through the cmd list
        };
*/

// Main structure
typedef	struct s_shell
{

	HISTORY_STATE	hist;
	t_command	**cmds;
}		t_shell;

/* Prototypes */

	// minishell.c
int	main(int argc, char **argv, char **env);
int     init_shell(t_shell *sh);

	// ms_signals.c
void	init_signals(void);
void	init_sigset(void);
void	init_sigaction(void);
void	signal_handler(int signum);

	// ms_input_manager.c
char    *get_input(char *line);
char	**normalize_input(char *line);
void    process_input(char **input_args, char **env);

	// ms_parser.c
char	*ft_normalize(char *line);
char	*ft_strcollapse(char *line);
int	ft_count_char(char *str, char c);
char	*copy_collapse(char *dst, char *src, size_t src_len);

	// ms_quotes_handler.c
void	handle_quote(char *line, char quote_type, int *i, int *to_collapse);
void	ft_replace_char(char *cur_c, char new_c);

	// ms_commands_manager.c
int     	init_cmds(t_shell *sh);
t_command       *register_cmd(char *name, void *func, char *doc);

/* Prototypes: commands */

	// ms_cmd_pwd.c - Print name of current/working directory
void	cmd_pwd(void);
	// ms_cmd_cd.c - Change the working directory
int	cmd_cd(char *path);
	// ms_cmd_echo.c - Display a line of text
void	cmd_echo(char **input_args, char **env);
	// ms_cmd_exit.c - Cause the shell to exit
void	cmd_exit(unsigned int status);
	// ms_cmd_export.c - Set the export attribute for variables
void	cmd_export(char **input_args, char **env);
	// ms_cmd_unset.c - Unset values and attributes of variables and functions
void	cmd_unset(void);
	// ms_cmd_env.c - Display the env variables
void	cmd_env(char **env);

/* Protoypes: error handling and cleaning */

	// ms_free.c

/*Yahyas's function*/
t_pipeline *build_pipeline_from_tokens(char **tokens);
void free_pipeline(t_pipeline *p);
#endif