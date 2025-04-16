/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:35:22 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/16 19:01:57 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define NB_CMDS 10 // Temporary, find out dynamically. DEBUG

# define PROMPT_STYLE "ms> "
# define CTRL_CHAR_SPACE_IN_QUOTE 29
# define CTRL_CHAR_VAR_TO_INTERPRET 30
# define CTRL_CHAR_SUBARG_DELIM 31

/* Libraries */

	// Pipex
# include "../pipex_ms/pipex.h"

	// Standard libraries
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <term.h>
# include <unistd.h>

	// Libft
# include "../libft/libft.h"

/* Structures */

typedef struct s_command
{
	char	*name;
	void	*func;
	char	*doc;
}			t_command;

// Main structure
typedef struct s_shell
{
	HISTORY_STATE	hist;
	t_command		**cmds;
	t_list			*this_env;
	char			*normalized_line;
	char			**input_args;
	t_pipeline		*pipeline;
	int				last_exit_status;
	char			**tokens;
}					t_shell;

/* Prototypes */

	// minishell.c
int			main(int argc, char **argv, char **env);
int			init_shell(t_shell *sh, char **env);
int			init_env(t_shell *sh, char **env);

	// ms_signals.c
void		init_signals(void);
void		init_sigset(void);
void		init_sigaction(void);
void		signal_handler(int signum);

	// ms_input_manager.c
char		*get_input(char *line);
char		**normalize_input(char *line, t_shell *sh);
int			ft_interpret_env(t_shell *sh);
void		process_input(t_shell *sh);

	// ms_normalizer.c
char		*ft_normalize(char *line);
char		*ft_strcollapse(char *line);
int			ft_count_char(char *str, char c);
char		*copy_collapse(char *dst, char *src, size_t src_len);
void		pass_quotes(char *dst, char *src, size_t *i, size_t *j);

	// ms_quotes_handler.c
void		handle_quote(char *line, char quote_type, int *i, int *to_collapse);

	// ms_redirections_manager.c
//

	// ms_env_interpreter.c
int			ft_interpret_env(t_shell *sh);
char		**ft_split_args(char **split_args, char *input_arg);
char		*ft_nametoval(char *rejoined_arg, char **split_args, t_list **this_env);
char		*ft_rejoin_subarg(char **split_args, char *rejoined_arg, int i);
char		**ft_copy_free(char **input_arg, char *rejoined_arg);

	// ms_env_utils.c
size_t		ft_strslen(char **strs);
int			ft_strstolist(t_list **list, char **strs, size_t nb_strs, char delim);
t_list		*ft_getenv(char *var_name, t_list **this_env);
int			ft_update_env_value(t_list *set_var, char **split_str);

	// ms_replace_utils.c
void		ft_replace_if_space(char *cur_c, char new_c);
void		ft_replace_char(char *cur_c, char new_c);
void		ft_replace_all_chars(char **input_args, char old_c, char new_c);

	// ms_commands_manager.c
int			init_cmds(t_shell *sh);
t_command	*register_cmd(char *name, void *func, char *doc);

/* Prototypes: commands */

	// ms_cmd_pwd.c - Print name of current/working directory
void		cmd_pwd(void);
	// ms_cmd_cd.c - Change the working directory
int			cmd_cd(t_shell *sh);
	// ms_cmd_echo.c - Display a line of text
void		cmd_echo(t_shell *sh);
void		echo_set_n(char **input_args, bool *opt_n, int *i);
	// ms_cmd_exit.c - Cause the shell to exit
void		cmd_exit(t_shell *sh, unsigned int status);
	// ms_cmd_export.c - Set the export attribute for variables
void		cmd_export(t_shell *sh);
void		export_stash_var(t_shell *sh);
	// ms_cmd_unset.c - Unset values and attributes of variables and functions
void		cmd_unset(t_shell *sh);
	// ms_cmd_env.c - Display the env variables
void		cmd_env(t_shell *sh);

/* Protoypes: error handling and cleaning */

	// ms_free.c
void		free_memory(t_shell *sh);
void		free_env(t_list **this_env);
void		free_commands(t_command **cmds);
void		free_args(char **input_args);

/*Yahyas's function*/
t_pipeline	*build_pipeline_from_tokens(char **tokens);
void		free_pipeline(t_pipeline *p);
void		exec_with_redirection(t_pipeline *cmd, char **env);
t_pipeline	*parse_redirection_only(char **tokens);
void		print_pipeline(t_pipeline *p);

#endif
