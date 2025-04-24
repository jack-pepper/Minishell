/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:17:34 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/24 21:10:33 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define NB_CMDS 10 // Temporary, find out dynamically. DEBUG

# define PROMPT_STYLE "ms> "
# define CTRL_CHAR_SPACE_IN_QUOTE 29
# define CTRL_CHAR_VAR_TO_INTERPRET 30
# define CTRL_CHAR_SUBARG_DELIM 31
# define CTRL_CHAR_TO_BE_DELETED 29

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
	t_pipeline		*pipeline;
	t_list			*this_env;
	t_list			*env_stash;
	char			*normalized_line;
	char			**input_args;
	int				last_exit_status;
	char			**tokens;
}					t_shell;

/* Prototypes */

	// minishell.c
int			main(int argc, char **argv, char **env);
int			init_shell(t_shell *sh, char **env);
int			init_env(t_shell *sh, char **env);
void    		stash_var_or_invalidate(t_shell *sh);

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
void		stash_var(t_shell *sh);
char		**ft_strschr(char **strs, char c, int forbidden_pos);

	// ms_replace_utils.c
void		ft_replace_if_space(char *cur_c, char new_c);
void		ft_replace_char(char *cur_c, char new_c);
void		ft_replace_all_chars(char **input_args, char old_c, char new_c);

	// ms_commands_manager.c
int			init_cmds(t_shell *sh);
t_command	*register_cmd(char *name, void *func, char *doc);

/* Prototypes: commands */

	// ms_cmd_pwd.c - Print name of current/working directory
int		cmd_pwd(void);
char	*store_cwd(char *cwd);
	// ms_cmd_cd.c - Change the working directory
int			cmd_cd(t_shell *sh);
int			change_directory(t_shell *sh, char *cwd, char *path);
char		*handle_dotted_path(t_shell *sh, char *cwd, char *path);
void		update_pwds_vars(t_shell *sh, char *prev_cwd, char *new_pwd);
void		update_pwd_var(t_shell *sh, char **split_pwd);
void		update_old_pwd_var(t_shell *sh, char **split_old_pwd);
	// ms_cmd_echo.c - Display a line of text
int		cmd_echo(t_shell *sh);
void		echo_set_n(char **input_args, bool *opt_n, int *i);
	// ms_cmd_exit.c - Cause the shell to exit
int		cmd_exit(t_shell *sh, unsigned int status);
	// ms_cmd_export.c - Set the export attribute for variables
int		cmd_export(t_shell *sh);
void		export_from_term(t_shell *sh, size_t *i);
void		export_from_stash(t_shell *sh, t_list *stashed_var);
void		add_new_env_var(t_shell *sh, char **split_str);
int			ft_update_env_value(t_list *set_var, char **split_str);
	// ms_cmd_unset.c - Unset values and attributes of variables and functions
int		cmd_unset(t_shell *sh);
	// ms_cmd_env.c - Display the env variables
int		cmd_env(t_shell *sh);

/* Protoypes: error handling and cleaning */

	// ms_free.c
void		free_memory(t_shell *sh);
void		free_list(t_list **list);
void		free_commands(t_command **cmds);
void		free_args(char **input_args);

/*Yahyas's function*/
t_pipeline	*build_pipeline_from_tokens(char **tokens);
void		free_pipeline(t_pipeline *p);
void		exec_with_redirection(t_pipeline *cmd, char **env);
t_pipeline	*parse_redirection_only(char **tokens);
void		print_pipeline(t_pipeline *p);
void run_pipeline_from_minshell(t_pipeline *p, char **env);
typedef enum e_cmd_type {
	BASIC,
	REDIR_ONLY,  
	PIPELINE,   
	MIXED_INVALID
}	t_cmd_type;
bool is_builtin(const char *cmd);
t_cmd_type classify_command(char **tokens);
#endif
