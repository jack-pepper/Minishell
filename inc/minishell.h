/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:35:22 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/20 18:03:43 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define PATH_TO_HEADER "../inc/minishell.h"

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

// A similar struct is suggested by documentation at https://web.mit.edu/gnu/doc/html/rlman_2.html
/*
typedef struct s_command
{
	char *name;			// User printable name of the function.
	Function *func;			// Function to call to do the job: can we use a pointer to a function? (or a "t_function" struct if needed)
	char *doc;			// Documentation for this function.
} 		t_command;
*/

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
                { (char *)NULL, (Function *)NULL, (char *)NULL } /// ???
        };
*/

// Main structure
typedef	struct s_shell
{

	HISTORY_STATE	hist;
}		t_shell;

/* Prototypes */

	// minishell.c
int	main();
int     init_shell(t_shell *sh);
char    *get_input(char *line);
void    process_input(char *line);

	// ms_signals.c
void	init_signals(void);
void	init_sigset(void);
void	init_sigaction(void);
void	signal_handler(int signum);

/* Prototypes: commands */

	// ms_cmd_pwd.c - Print name of current/working directory
void	cmd_pwd(void);

	// ms_cmd_cd.c - Change the working directory
void	cmd_cd(void);

	// ms_cmd_echo.c - Display a line of text
void	cmd_echo(void);

	// ms_cmd_exit.c - Cause the shell to exit
void	cmd_exit(void);

	// ms_cmd_export.c - Set the export attribute for variables
void	cmd_export(void);

	// ms_cmd_unset.c - Unset values and attributes of variables and functions
void	cmd_unset(void);

	// ms_cmd_env.c - Display the env variables
void	cmd_env(void);

/* Protoypes: error handling and cleaning */

	// ms_free.c

#endif
