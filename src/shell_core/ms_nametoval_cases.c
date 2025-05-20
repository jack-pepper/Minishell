/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval_cases.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/20 23:18:41 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_special_cases(t_shell *sh, char *rejoined_arg, char *split_arg, char *end_name)
{
	char	temp;

	if (ft_ispunct(*end_name) && !ft_is_in_set(*end_name, "$"))
	{
		temp = *end_name;
		*end_name = CC_SUBARG_DELIM;
		rejoined_arg = split_rejoin(sh, rejoined_arg, split_arg, temp);
	}
	else if (*end_name == CC_TRAILING_DOLLAR)
	{
		rejoined_arg = split_rejoin(sh, rejoined_arg, split_arg,
				CC_TRAILING_DOLLAR);
	}
	else if (*end_name == CC_SPACE_IN_QUOTE)
	{
		*end_name = CC_SUBARG_DELIM;
		rejoined_arg = split_rejoin(sh, rejoined_arg, split_arg,
				CC_SUBARG_DELIM);
	}
	else if (*end_name == CC_VAR_BOUND)
	{
		rejoined_arg = split_rejoin(sh, rejoined_arg, split_arg,
				CC_VAR_BOUND);
	}
	return (rejoined_arg);
}

char	*handle_exit_status_case(t_shell *sh, char *rejoined_arg, char *subarg)
{
	char	*arg;
	char	*exit_status;
	char	*temp;

	exit_status = ft_itoa(sh->last_exit_status);
	if (subarg[1] != '\0')
	{
		temp = ft_strdup(&subarg[1]);
		if (!temp)
		{
			free(exit_status);
			return (NULL);
		}
		arg = ft_strjoin(exit_status, temp);
		free(temp);
	}
	else
		arg = ft_strdup(exit_status);
	free(exit_status);
	if (!arg)
		return (NULL);
	rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
	free(arg);
	return (rejoined_arg);
}

char	*handle_var_case(t_shell *sh, char *rejoined_arg, char *arg)
{
	t_list	*set_var;

	set_var = ft_getenv(arg, &sh->this_env);
	if (set_var != NULL)
		arg = ft_strdup(((char **)set_var->content)[1]);
	else
		arg = ft_strdup("");
	rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
	return (rejoined_arg);
}
