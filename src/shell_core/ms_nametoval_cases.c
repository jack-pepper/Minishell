/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval_cases.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/25 12:27:07 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_special_cases(t_shell *sh, char *joined, char *split, char *end)
{
	char	symbol;

	if (*end == CC_VAR_BOUND)
	{
		*end = CC_SUBARG_DELIM;
		joined = split_rejoin(sh, joined, split, CC_VAR_BOUND);
		if (DEBUG == 1)
			printf(">$$$ [special_case] (is CC_VAR_BOUND: TO REMOVE) joined: ~%s~\n", joined);
	}
	else if (*end == CC_VAR_BOUND_SQUOTE)
	{
		*end = CC_SUBARG_DELIM;
		joined = split_rejoin(sh, joined, split, CC_VAR_BOUND_SQUOTE);
		if (DEBUG == 1)
			printf(">$$$ [special_case] (is CC_VAR_BOUND_SQUOTE: TO RESTORE) joined: ~%s~\n", joined);
	}
	else if (*end == CC_SPACE_IN_QUOTE || ft_ispunct(*end))
	{
		symbol = *end;
		*end = CC_SUBARG_DELIM;
		joined = split_rejoin(sh, joined, split, symbol);
		if (DEBUG == 1)
			printf(">$$$ Symbol: CC_SPACE_IN_QUOTE or punct (TO KEEP) joined: ~%s~\n", joined);
	}
	return (joined);
}

char	*handle_exit_status_case(t_shell *sh, char *rejoined_arg, char *subarg)
{
	char	*arg;
	char	*exit_status;
	char	*temp;

	if (DEBUG == 1)
		printf("EXIT STATUS CASE\n");
	exit_status = ft_itoa(sh->last_exit_status);
	if (subarg[1] != '\0')
	{
		temp = ft_strdup(&subarg[1]);
		//if ((temp[0] == '\'' || temp[0] == CC_VAR_BOUND_SQUOTE)
		//	&& !(temp[1]))
		//	temp[0] = '\'';
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
	if (DEBUG == 1)
		printf("arg: ~%s~\n", arg);
	rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
	if (DEBUG == 1)
		printf("rejoined_arg: ~%s~\n", rejoined_arg);
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
