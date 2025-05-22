/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:07:06 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/22 12:26:02 by mmalie           ###   ########.fr       */
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
		if (rl_on_new_line() != -1)
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

// Should call the needed command and handle errors 
int	process_input(t_shell *sh)
{
	t_command	*cmd;

	if (!sh->input_args || sh->input_args[0] == NULL)
		return (-1);
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
			sh->last_exit_status = handle_non_cmd(sh);
	}
	return (sh->last_exit_status);
}

int	handle_non_cmd(t_shell *sh)
{
	int		res;

	ft_replace_chars_in_str(sh->input_args[0], CC_SPACE_IN_QUOTE, ' ');
	res = 0;
	res = are_args_stashable(sh->input_args);
	if (res != 0)
	{
		if (ft_strchr(sh->input_args[0], '/') != NULL)
		{
			res = handle_file_or_dir(sh);
			return (res);
		}
		else
			return (ms_err("", sh->input_args[0], CMD_NOT_FOUND, 127));
	}
	res = stash_var(sh);
	return (res);
}

int	handle_file_or_dir(t_shell *sh)
{
	struct stat	st;

	if (ft_strchr(sh->input_args[0], '/'))
	{
		if (access(sh->input_args[0], F_OK) != 0)
			return (ms_err("", sh->input_args[0], NO_FILE_OR_DIR, 127));
		else
		{
			if (stat(sh->input_args[0], &st) != 0)
				return (ms_err("", sh->input_args[0], NO_FILE_OR_DIR, 127));
			if (S_ISDIR(st.st_mode))
				return (ms_err("", sh->input_args[0], CMD_IS_DIR, 126));
			if (access(sh->input_args[0], X_OK) != 0)
				return (ms_err("", sh->input_args[0], PERM_DENY, 126));
		}
	}
	return (0);
}
