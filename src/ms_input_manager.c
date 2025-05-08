/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:23:56 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/08 16:35:23 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	//printf("Normalized %s\n", sh->normalized_line);
	if (!sh->normalized_line)
		return (NULL);
	sh->input_args = ft_split(sh->normalized_line, ' ');
//	ft_show_strs(sh->input_args, "[DEBUG: input_args before normalization]");
	free(sh->normalized_line);
	sh->normalized_line = NULL;
	if (!sh->input_args)
		return (NULL);
	ft_replace_all_chars(sh->input_args, '|', CTRL_CHAR_PIPE);
//	ft_show_strs(sh->input_args, "[DEBUG: input_args after normalization]");
	return (sh->input_args);
}

int	handle_dollar_cmd(t_shell *sh)
{
	// NB: Should actually display the var value in the error...
	char	*exit_status;

	if (sh->input_args[0] && sh->input_args[0][0] == CTRL_CHAR_VAR_TO_INTERPRET)
	{
		if (ft_strcmp(sh->input_args[0], (char[]){CTRL_CHAR_VAR_TO_INTERPRET, '?', '\0'}) == 0)
		{
			exit_status = ft_itoa(sh->last_exit_status);
			ft_putstr_fd(exit_status, 2); // small fix, improve it later
			free(exit_status);
			return (ft_ret(127, CMD_NOT_FOUND, STDERR));
		}
		else
		{
			t_list *set_var = ft_getenv(&(sh->input_args[0][1]), &sh->this_env);
			if (!set_var)
				return (0);
			else
			{
				if (((char **)set_var->content)[1][0] == '/')
					return (ft_ret(126, CMD_IS_DIR, STDERR));
				else
					return (ft_ret(0, CMD_NOT_FOUND, STDERR));
			}
		}
	}
	return (1);
}

// Should call the needed command and handle errors 
int	process_input(t_shell *sh)
{
	int res;

	if (!sh->input_args || sh->input_args[0] == NULL)
		return (-1);	
	res = handle_dollar_cmd(sh);
	if (res != 1)
		return (res);

//	ft_show_strs(sh->input_args, "[DEBUG] input_args BEFORE env interpret");
	ft_interpret_env(sh);
//	ft_show_strs(sh->input_args, "[DEBUG] input_args AFTER env interpret");

    if (ft_strcmp(sh->input_args[0], "exit") == 0)
		sh->last_exit_status = cmd_exit(sh, 0);
	else if (ft_strcmp(sh->input_args[0], "pwd") == 0)
                sh->last_exit_status = cmd_pwd();
	else if (ft_strcmp(sh->input_args[0], "cd") == 0)
	{
		sh->last_exit_status = cmd_cd(sh);
		if (sh->last_exit_status != 0)
			return (sh->last_exit_status);
	}
	else if (ft_strcmp(sh->input_args[0], "env") == 0)
		sh->last_exit_status = cmd_env(sh);
	else if (ft_strcmp(sh->input_args[0], "echo") == 0)
		sh->last_exit_status = cmd_echo(sh);
	else if (ft_strcmp(sh->input_args[0], "export") == 0)
		sh->last_exit_status = cmd_export(sh);
	else if (ft_strcmp(sh->input_args[0], "unset") == 0)
		sh->last_exit_status = cmd_unset(sh);
	else
		sh->last_exit_status = stash_var_or_invalidate(sh);
	return (sh->last_exit_status);
}


char	*is_invalid_for_stash(t_shell *sh)
{
	char	*equal_char;
	char	*var_name;
	size_t	len;
	int		i;
	
	i = 0;
	while (sh->input_args[i] != NULL)
	{
		equal_char = ft_strchr(sh->input_args[i], '=');
		len = (size_t)(equal_char - sh->input_args[i]);
		var_name = ft_substr(sh->input_args[i], 0, len);
		if (!is_valid_env_name(var_name))
		{
			return (var_name);
		}
		free(var_name);
		i++;
	}
	return (NULL);
}

int	stash_var_or_invalidate(t_shell *sh)
{
	char	**invalid_cmd;
	char	*invalid_var_name;
	int		i;

	invalid_cmd = ft_strschr(sh->input_args, '=', 0);
	printf("invalid_cmd: %s\n", *invalid_cmd);
	if (invalid_cmd == NULL)
	{
		invalid_var_name = is_invalid_for_stash(sh);
		if (invalid_var_name != NULL)
		{
			printf("minishell: %s: command not found\n", invalid_var_name);
			free(invalid_var_name);
			return (127);
		}
		i = 0;
		while (sh->input_args[i] != NULL)
		{
			i = 0;
			while (sh->input_args[i])
			{
				if (stash_var(sh) == 2)
				{
					printf("minishell: %s: command not found\n", sh->input_args[i]);
					return (127);
				}
				i++;
			}
		}
	}
	else
	{
		printf("minishell: %s: command not found\n", *invalid_cmd);
		return (127);
	}
	return (0);
}
