/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env_interpreter.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 21:05:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/15 15:38:37 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_env_spaces(t_shell *sh)
{
	char	*temp;

	temp = join_all_subargs(sh->input_args, ' ');
	if (!temp)
		return (1);
	protect_spaces_in_assignements(temp);
	free_args(sh->input_args);
	sh->input_args = ft_split(temp, ' ');
	free(temp);
	if (!sh->input_args)
		return (1);
	restore_protected_spaces(sh->input_args);
	return (0);
}

int	ft_interpret_env(t_shell *sh)
{
	char	**split_args;
	char	*rejoined_arg;
	int		i;

	i = 0;
	while (sh->input_args[i])
	{
		split_args = NULL;
		if (ft_strchr(sh->input_args[i], CC_VAR_TO_INTERPRET) != NULL)
		{
			split_args = ft_split(sh->input_args[i], CC_VAR_TO_INTERPRET);
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
	return (handle_env_spaces(sh));
}

char	*rejoin_arg(t_shell *sh, char *rejoined_arg, char **split_args, int i)
{
	if (sh->input_args[i][0] == CC_VAR_TO_INTERPRET)
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

char	**ft_copy_free(char **input_arg, char *rejoined_arg)
{
	free(*input_arg);
	(*input_arg) = ft_strdup(rejoined_arg);
	if (!(*input_arg))
		return (NULL);
	return (input_arg);
}

void	remove_empty_args(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
			args[j++] = args[i];
		else
			free(args[i]);
		i++;
	}
	args[j] = NULL;
}
