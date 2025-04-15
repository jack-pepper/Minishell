/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/15 15:07:26 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_interpret_env(t_shell *sh)
{
	char	**split_args;
	char	*rejoined_arg;
	int		i;

	i = 0;
	while (sh->input_args[i])
	{
		split_args = NULL;
		rejoined_arg = ft_strdup("");
		split_args = ft_split_args(split_args, sh->input_args[i]);
		if (!split_args)
			return (-1);
		rejoined_arg = ft_nametoval(rejoined_arg, split_args,
				&sh->this_env);
		free_args(split_args);
		if (!rejoined_arg)
			return (-1);
		ft_copy_free(&sh->input_args[i], rejoined_arg);
		free(rejoined_arg);
		if (!sh->input_args[i])
			return (-1);
		i++;
	}
	return (0);
}

char	**ft_split_args(char **split_args, char *input_arg)
{
	int	i;

	i = 0;
	while (input_arg[i] != '\0') // Might need to check it for edge cases
	{
		if (i > 0 && input_arg[i] != ' ' && input_arg[i + 1] == ' ')
		{
			input_arg[i + 1] = CTRL_CHAR_SUBARG_DELIM;
			i++;
		}
		if (input_arg[i] == ' ' && input_arg[i + 1] != ' ')
		{
			input_arg[i] = CTRL_CHAR_SUBARG_DELIM;
			i++;
		}
		i++;
	}
	split_args = ft_split(input_arg, CTRL_CHAR_SUBARG_DELIM);
	if (!split_args)
		return (NULL);
	return (split_args);
}

char	*ft_nametoval(char *rejoined_arg, char **split_args, t_list **this_env)
{
	t_list	*set_var;
	int		i;

	i = 0;
	while (split_args[i])
	{
		if (split_args[i][0] == CTRL_CHAR_VAR_TO_INTERPRET)
		{
			set_var = ft_getenv(&split_args[i][1], this_env);
			if (set_var != NULL)
			{
				free(split_args[i]);
				split_args[i] = ft_strdup(((char **)set_var->content)[1]);
				if (!split_args[i])
					return (NULL);
			}
			else
				split_args[i][0] = '$';
		}
		rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		i++;
	}
	return (rejoined_arg);
}

char	*ft_rejoin_subarg(char **split_args, char *rejoined_arg, int i)
{
	char	*temp;
	char	*temp_with_space;

	temp = ft_strjoin(rejoined_arg, split_args[i]);
	free(rejoined_arg);
	if (!temp)
		return (NULL);
	if (split_args[i + 1])
	{
		temp_with_space = ft_strjoin(temp, " ");
		free(temp);
		if (!temp_with_space)
			return (NULL);
		rejoined_arg = ft_strdup(temp_with_space);
		free(temp_with_space);
		if (!rejoined_arg)
			return (NULL);
		return (rejoined_arg);
	}
	rejoined_arg = ft_strdup(temp);
	free(temp);
	return (rejoined_arg);
}

char	**ft_copy_free(char **input_arg, char *rejoined_arg)
{
	free(*input_arg);
	(*input_arg) = ft_strdup(rejoined_arg);
	if (!(*input_arg))
		return (NULL);
	return (input_arg);
}
