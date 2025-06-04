/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:05:05 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/05 00:22:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	validate_and_exec_command(char **argv, char **envp, t_shell *sh)
{
	if (!argv || !argv[0] || argv[0][0] == '\0')
		return (0);
	if (ft_strchr(argv[0], '/'))
		return (validate_direct_path(argv, sh));
	else
		return (validate_in_path(argv, envp, sh));
}

void	restore_quoted_spaces(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == CC_SPACE_IN_QUOTE)
			str[i] = ' ';
		i++;
	}
}

void	dispatch_command(t_cmd_type type, t_shell *sh, char **env)
{
	if (type == REDIR_ONLY)
		handle_redir_only(sh, env);
	else if (type == BASIC)
	{
		//validate_and_exec_command(sh->tokens, sh->input_args, sh); // already handled in handle_basic by handle_file_or_dir
		handle_basic(sh, env);
	}
	else if (type == PIPELINE || type == HERE_DOC)
		handle_pipeline(sh, env);
	else if (type == PIPELINE_WITH_RED)
	{
		sh->pipeline = ft_calloc(1, sizeof(t_pipeline));
		parse_and_build_pipeline(sh->pipeline, sh->input_args);
		run_pipeline_with_redir(sh->pipeline, env, sh);
	}
	else
	{
		//sh->last_exit_status = 2;
		//ft_putstr_fd("synthax error\n", 2);
		sh->last_exit_status = ms_err("", "", SYNTAX_ERR, 2);
	}
}

void	main_loop(t_shell *sh, char **env)
{
	static char	*line;
	t_cmd_type	type;

	line = NULL;
	while (1)
	{
		line = get_input(line);
		if (line == NULL)
			free_and_exit(sh, 0);
			//cmd_exit(sh, 1); // replaced by the prev line to escape some reading errors issue	
		if (line[0] == '\0')
			continue ;
		sh->input_args = normalize_input(line, sh);
		if (!sh->input_args)
			continue ;
		if (g_signal_status != 0)
		{
			sh->last_exit_status = g_signal_status;
			g_signal_status = 0;
		}
		type = classify_command(sh->input_args);
		dispatch_command(type, sh, env);
		ft_free_array(sh->input_args, -1);
	}
	free(line);
}

int	main(int argc, char **argv, char **env)
{
	t_shell		sh;

	if (argc != 1 || argv[1])
		return (-1);
	if (init_shell(&sh, env) != 0)
		return (-1);
	main_loop(&sh, env);
	rl_clear_history();
	return (sh.last_exit_status);
}
