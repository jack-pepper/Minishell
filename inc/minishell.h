/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:08:37 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/16 23:35:13 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define NB_CMDS 7

# define STDOUT 1
# define STDERR 2

# define PROMPT_STYLE "ms> "

# define CTRL_CHAR_REDIR_IN  24  // "<"
# define CTRL_CHAR_REDIR_OUT 25 // ">"
# define CTRL_CHAR_APPEND    26  // ">>"
# define CTRL_CHAR_HEREDOC   27 // "<<"
# define CTRL_CHAR_PIPE 28 // "|"
# define CTRL_CHAR_SPACE_IN_QUOTE '_' // 29
# define CTRL_CHAR_VAR_TO_INTERPRET '#' // 30
# define CTRL_CHAR_SUBARG_DELIM '-' // 31
# define CTRL_CHAR_TO_BE_DELETED 'x' // 23
# define CTRL_CHAR_EXTRA_DELIM ';' // 31
# define CTRL_CHAR_VAR '*' // 21 // For edge case `echo "$HO"ME`
# define CTRL_CHAR_STICKY_VAR '@' // 22

/* Return messages (ft_ret) */

# define SHELL_NAME "minishell: " // could be used to improve error msg

# define TOO_MANY_ARGS ": too many arguments\n"
# define HOME_NON_SET ": HOME non set\n"
# define NO_FILE_OR_DIR ": No such file or directory\n"
# define INVALID_ID ": not a valid identifier\n"
# define NUM_ARG_REQ ": numeric argument required\n"
# define CMD_NOT_FOUND ": command not found\n"
# define CMD_IS_DIR ": Is a directory\n"

/* Libraries */

	// Pipex
# include "../pipex_ms/pipex.h"

	// Standard libraries
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
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

struct	s_shell;

typedef struct s_command
{
	char	*name;
	int	(*func)(struct s_shell *);
	char	*doc;
}			t_command;

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

// ./SHELL_CORE

	// minishell.c
int			main(int argc, char **argv, char **env);
int			is_invalid_for_stash(char *arg);
int			stash_var_or_invalidate(t_shell *sh);

	// ms_initer.c
int			init_shell(t_shell *sh, char **env);
int			init_env(t_shell *sh, char **env);
void		init_signals(void);
void		signal_handler(int signum);
int		normalize_env(t_list *this_env);

	// ms_input_manager.c
char		*get_input(char *line);
char		**normalize_input(char *line, t_shell *sh);
int			process_input(t_shell *sh);

	// ms_normalizer.c
char		*ft_normalize(char *line);
char		*ft_strcollapse(char *line);
char		*copy_collapse(char *dst, char *src, size_t src_len);

	// ms_flagger.c
void	flag_dollar(char *line, int *i);
void	flag_pipe_and_redir(char *line, int *i);
void	flag_quote(char *line, int *i);
void	ante_merge_quote(char *line, int *i);
void	post_merge_quote(char *line, int *i);


	// ms_quotes_handler.c
char		*flag_edge_var(char *line, int *i);
void		handle_quote(char *line, char quote_type, int *i);
void		pass_quotes(char *dst, char *src, size_t *i, size_t *j);

	// ms_env_interpreter.c
int			ft_interpret_env(t_shell *sh);
char		*rejoin_arg(t_shell *sh, char *rejoined_arg, char **split_args, int i);

	// ms_nametoval.c
char		*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args);
char		*handle_space_in_quote_case(t_shell *sh, char *rejoined_arg, char **split_args, int i);
char		*handle_exit_status_case(t_shell *sh, char *subarg);
char		*handle_var_case(t_shell *sh, char **split_args, int i);

	// ms_env_interpreter_edge.c
int			ft_interpret_env_edge(t_shell *sh);
char		*rejoin_arg_edge(t_shell *sh, char *rejoined_arg, char **split_args, int i);
char		*ft_nametoval_edge(t_shell *sh, char *rejoined_arg, char **split_args);
char		*handle_space_in_quote_case_edge(t_shell *sh, char *rejoined_arg, char **split_args, int i);

	// ms_commands_manager.c
int			init_cmds(t_shell *sh);
t_command	*register_cmd(char *name, void *func, char *doc);
t_command	*is_registered_cmd(t_shell *sh);

	// ms_free.c
void		free_memory(t_shell *sh);
void		free_list(t_list **list);
void		free_commands(t_command **cmds);
void		free_args(char **input_args);

// ./BUILTINS

	// ms_cmd_cd.c - Change the working directory
int			cmd_cd(t_shell *sh);
int			cd_process_path(t_shell *sh, char *cwd, char *path);
int			change_directory(t_shell *sh, char *cwd, char *path);
void		update_pwds_vars(t_shell *sh, char *prev_cwd, char *new_pwd);
void		update_pwd_var(t_shell *sh, char **split_pwd, char *key);

	// ms_cmd_echo.c - Display a line of text
int			cmd_echo(t_shell *sh);
void		echo_set_n(char **input_args, bool *opt_n, int *i);
char		*echo_process_all_args(t_shell *sh, char *joined_input, int *i);
char		*echo_join_input(char *joined_input, char **input_args, int *i);
void		echo_display(char *joined_output, bool opt_n);

	// ms_cmd_env.c - Display the env variables
int			cmd_env(t_shell *sh);

	// ms_cmd_exit.c - Cause the shell to exit
int			cmd_exit(t_shell *sh, unsigned int status);
void		free_and_exit(t_shell *sh, unsigned int status);
int     	ft_isnum(char *str);
char		*ms_trim(char *trimmed_arg, char *arg, int len, int k);
int		exit_arg_overflow(char *str);

	// ms_cmd_export.c - Set the export attribute for variables
int			cmd_export(t_shell *sh);
int			try_export(t_shell *sh, size_t *i);
int			export_from_term(t_shell *sh, size_t *i);
void		export_from_stash(t_shell *sh, t_list *stashed_var);

	// ms_cmd_pwd.c - Print name of current/working directory
int			cmd_pwd(void);
char		*store_cwd(char *cwd);

	// ms_cmd_unset.c - Unset values and attributes of variables and functions
int			cmd_unset(t_shell *sh);
t_list		*get_prev_node(t_list *cur_node, t_list *this_env);

	// ./UTILS

	// ms_cd_path_utils.c
char		*handle_dotted_path(char *cwd, char *path);
char		*get_abs_path(char *joined_path, char *cwd, char *path);
char		**split_abs_path(char **split_path, char *joined_path);
void		flag_dotted_path(char **split_path, char ctrl_char);
char		*rejoin_abs_path(char *rejoined_path, char **split_path);

	// ms_debug_utils.c
void		ft_show_strs(char **strs, char *debug_msg);
int			ft_ret(int return_val, char *msg, int fd);
int		ms_err(char *cmd_name, char *arg, char *err_msg, int return_val);

	// ms_env_utils.c
t_list		*ft_getenv(char *var_name, t_list **this_env);
int			is_valid_env_name(char *var_name);
int			add_new_env_var(t_shell *sh, char **split_str);
int			ft_update_env_value(t_list *set_var, char **split_str);

	// ms_flags_utils.c
void		ft_flag_delim(char *str, char delim, char flag, char *mode);
void		ft_flag_all_but_first(char *str, char delim, char flag);
void            ft_unflag_delim(char *str, char delim, char flag);
char		**split_input_arg(t_shell *sh, size_t *i);

	// ms_replace_utils.c
void		ft_replace_if_space(char *cur_c, char new_c);
void		ft_replace_char(char *cur_c, char new_c);
void		ft_replace_all_chars(char **input_args, char old_c, char new_c);

	// ms_split_join_utils.c
//char		**ft_split_args(char **split_args, char *input_arg);
char		*join_all_subargs(char **args, char delim);
char		*ft_strjoin_delim(char const *s1, char const *s2, char const *delim);
char		*ft_rejoin_subarg(char **split_args, char *rejoined_arg, int i);
char		**ft_copy_free(char **input_arg, char *rejoined_arg);

	// ms_stash_utils.c
int			stash_var(t_shell *sh);
int			handle_stashing(t_shell *sh, char **split_str);
int			is_valid_env_name_sub(char *arg);
int			are_args_stashable(char **args);
int			add_new_stash_var(t_shell *sh, char **split_str);

	// ms_strs_utils.c
size_t		ft_strslen(char **strs);
int			ft_strstolist(t_list **list, char **strs, size_t nb_strs, char delim);
char		**ft_strschr(char **strs, char c, int forbidden_pos);
char		**ft_strsdup(char **strs);

/*Yahyas's function*/
t_pipeline	*build_pipeline_from_tokens(char **tokens);
void		free_pipeline(t_pipeline *p);
void		exec_with_redirection(t_pipeline *cmd, char **env, t_shell *sh);
t_pipeline	*parse_redirection_only(char **tokens);
void		print_pipeline(t_pipeline *p);
void run_pipes_with_no_redir(t_pipeline *p, char **env);
void run_pipeline_with_redir(t_pipeline *p, char **env, t_shell *sh);
char *get_cmd_path(char *cmd, char **envp);
int validate_and_exec_command(char **argv, char **envp, t_shell *sh);
typedef enum e_cmd_type {
	BASIC,
	REDIR_ONLY,  
	PIPELINE,   
	MIXED_INVALID
}	t_cmd_type;
typedef struct s_redir {
	char *file;
	t_cmd_type type; 
	struct s_redir *next;
}	t_redir;

bool		is_builtin(const char *cmd);
t_cmd_type classify_command(char **tokens);
#endif
