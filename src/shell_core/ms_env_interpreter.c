/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/15 14:20:55 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_interpret_env(t_shell *sh)
{
	char	**split_args;
	char	*rejoined_arg;
	int		i;

	i = 0;
	while (sh->input_args[i])
	{
		split_args = NULL;
		if (ft_strchr(sh->input_args[i], CTRL_CHAR_VAR_TO_INTERPRET) != NULL)
		{
			split_args = ft_split(sh->input_args[i], CTRL_CHAR_VAR_TO_INTERPRET);
			ft_show_strs(split_args, "[ENV]");
			if (!split_args)
				return (-1);
			rejoined_arg = rejoin_arg(sh, rejoined_arg, split_args, i);
			free_args(split_args);
			if (!rejoined_arg)
				return (-1);
			ft_copy_free(&sh->input_args[i], rejoined_arg);
			free(rejoined_arg);
			if (!sh->input_args[i])
				return (-1);
		}
		i++;
	}
	return (0);
}

char	*rejoin_arg(t_shell *sh, char *rejoined_arg, char **split_args, int i)
{
	if (sh->input_args[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
	{
		rejoined_arg = ft_strdup("");
		rejoined_arg = ft_nametoval(sh, rejoined_arg, split_args);
	}
	else
	{
		rejoined_arg = ft_strdup(split_args[0]);
		rejoined_arg = ft_nametoval(sh, rejoined_arg, &split_args[1]);
	}
	return (rejoined_arg);
}

char	*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
	char	*first_space;
	t_list	*set_var;
	int		i;

	i = 0;
	while (split_args[i])
	{
		first_space = ft_strpbrk(split_args[i], (char[]){CTRL_CHAR_SPACE_IN_QUOTE, '\0'});
		if (first_space != NULL)
		{
			first_space[0] = CTRL_CHAR_SUBARG_DELIM;
			rejoined_arg = handle_space_in_quote_case(sh, rejoined_arg, split_args, i);
		}
		else
		{
			if (split_args[i][0] == '?')
				split_args[i] = handle_exit_status_case(sh, split_args[i]);
			else
			{
				// echo "$HO"ME
				if (ft_strrchr(split_args[i], CTRL_CHAR_VAR) != NULL)
				{
					//split_args[i][ft_strlen(split_args[i])] = '\0';
					char **split_subargs = ft_split(split_args[i], CTRL_CHAR_VAR);
					ft_show_strs(split_subargs, "[split_subargs]");
					printf("split_subargs[0]: %s\n", split_subargs[0]);
					set_var = ft_getenv(split_subargs[0], &sh->this_env);
					free(split_subargs[0]);
					if (set_var != NULL)
					{
						printf("FOUND VAR\n");
						split_subargs[0] = ft_strdup(((char **)set_var->content)[1]);
						printf("split_subargs[0]: %s\n", split_subargs[0]);
					}
					else
						split_subargs[0] = ft_strdup("");
				}
				else
				{
					printf("[nametoval - split_args[%d]] %s\n", i, split_args[i]);
					set_var = ft_getenv(split_args[i], &sh->this_env);
					free(split_args[i]);
					if (set_var != NULL)
						split_args[i] = ft_strdup(((char **)set_var->content)[1]);
					else
						split_args[i] = ft_strdup("");
				}
			}
			rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		}
		i++;
	}
	return (rejoined_arg);
}

char	*handle_space_in_quote_case(t_shell *sh, char *rejoined_arg, char **split_args, int i)
{
	char	**subargs;
	t_list	*set_var;

	subargs = ft_split(split_args[i], CTRL_CHAR_SUBARG_DELIM);
	if (subargs[0][0] == '?')
		subargs[0] = handle_exit_status_case(sh, subargs[0]);
	else
	{
		set_var = ft_getenv(subargs[0], &sh->this_env);
		free(subargs[0]);
		if (set_var != NULL)
			subargs[0] = ft_strdup(((char **)set_var->content)[1]);
		else
			subargs[0] = ft_strdup("");
	}
	free(split_args[i]);
	if (subargs[1] != NULL)
		split_args[i] = ft_strjoin_delim(subargs[0], subargs[1], " ");
	else
		split_args[i] = ft_strjoin_delim(subargs[0], "", " ");
	free_args(subargs);
	rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
	return (rejoined_arg);
}

char	*handle_exit_status_case(t_shell *sh, char *subarg)
{
	char	*exit_status;
	char	*temp;

	exit_status = ft_itoa(sh->last_exit_status);
	if (subarg[1] != '\0')
	{
		temp = ft_strdup(&subarg[1]);
		free(subarg);
		subarg = ft_strjoin(exit_status, temp);
		free(temp);
	}
	else
	{
		free(subarg);
		subarg = ft_strdup(exit_status);
	}
	free(exit_status);
	return (subarg);
}
