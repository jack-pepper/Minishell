/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_input_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:09:13 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/09 11:26:06 by mmalie           ###   ########.fr       */
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
		add_history(line); // Could be stored on a file before quitting
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
	// printf("Normalized %s\n", sh->normalized_line);
	if (!sh->normalized_line)
		return (NULL);
	sh->input_args = ft_split(sh->normalized_line, ' ');
	
	// int i = 0;
	// while(sh->input_args[i])
	// {
	// 	printf("normalize_input_arg[i] = %s\n", sh->input_args[i]);
	// 	i++;
	// }
	// ft_show_strs(sh->input_args, "[DEBUG: input_args befo  normalization]");
	
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
			// printf("I am here 312\n");
			t_list *set_var = ft_getenv(&(sh->input_args[0][1]), &sh->this_env);
			if (!set_var)
			{
				// printf("I am here 416\n");
				return (0);
			}
			else
			{
				if (((char **)set_var->content)[1][0] == '/')
					return (ft_ret(126, CMD_IS_DIR, STDERR));
				else
				{
					// printf("I am here 200\n");
					return (ft_ret(0, CMD_NOT_FOUND, STDERR));
				}
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
	{
		//printf("I am here 2000\n");
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
