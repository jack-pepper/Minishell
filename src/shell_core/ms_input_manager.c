/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:06 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/20 18:07:17 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Get the initial raw input from user
char	*get_input(char *line)
{
	if (line)
	{
		free(line);
		line = (char *) NULL;
	}
	line = readline(PROMPT_STYLE);
	if (line && *line)
	{
		add_history(line);
		if (rl_on_new_line() != -1) // How to handle this error?
			return (line);
	}
	else if (!line)
		return ((char *) NULL);
	return (line);
}

// Normalize the input and store the arguments for further use
char	**normalize_input(char *line, t_shell *sh)
{
	sh->normalized_line = ft_normalize(line);
	if (!sh->normalized_line)
		return (NULL);
	sh->input_args = ft_split(sh->normalized_line, ' ');
	free(sh->normalized_line);
	sh->normalized_line = NULL;
	if (!sh->input_args)
		return (NULL);
	ft_replace_all_chars(sh->input_args, '|', CC_PIPE);
	return (sh->input_args);
}

int	handle_dollar_cmd(t_shell *sh)
{
	char	*exit_status;
	t_list	*set_var;

	if (sh->input_args[0] && (sh->input_args[0][0] == '$' || sh->input_args[0][0] == CC_VAR_TO_INTERPRET))
	{
		if (!sh->input_args[0][1])
			return (ms_err("", "$", CMD_NOT_FOUND, 127));
		if (sh->input_args[0][1] == '?') // should rejoin if there's more after
		{
			exit_status = ft_itoa(sh->last_exit_status);
			ms_err("", exit_status, CMD_NOT_FOUND, 127);
			free(exit_status);
			return (127);
		}
		set_var = ft_getenv(&(sh->input_args[0][1]), &sh->this_env);
		if (!set_var)
			return (0);
		if (((char **)set_var->content)[1][0] == '/')
			return (ms_err("", ((char **)set_var->content)[1],
				CMD_IS_DIR, 126));
		else
			return (ms_err("", ((char **)set_var->content)[1],
				CMD_NOT_FOUND, 127));
	}
	return (1);
}

// Should call the needed command and handle errors 
int	process_input(t_shell *sh)
{
	t_command	*cmd;
	int		res;

	if (!sh->input_args || sh->input_args[0] == NULL)
		return (-1);
	res = handle_dollar_cmd(sh);
	if (res != 1 && res != 0)
		return (res);
	ft_interpret_env(sh);
	ft_replace_all_chars(sh->input_args, CC_TRAILING_DOLLAR, '$');
	if (DEBUG == 1)
		ft_show_strs(sh->input_args, "[PROCESS_INPUT (after env)]");
	cmd = is_registered_cmd(sh);
	if (cmd != NULL)
		sh->last_exit_status = cmd->func(sh);
	else
	{
		if (sh->input_args[0][0] != '\0')
			sh->last_exit_status = stash_var_or_invalidate(sh);
	}
	return (sh->last_exit_status);
}

int	stash_var_or_invalidate(t_shell *sh)
{
	int		res;

	res = 0;
	res = are_args_stashable(sh->input_args);
	if (res != 0)
		return (res);
	res = stash_var(sh);
	return (res);
}
