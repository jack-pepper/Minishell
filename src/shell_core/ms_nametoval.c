/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/23 23:57:11 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_nametoval(t_shell *sh, char *rejoined, char **split_args)
{
	int		i;
	int		j;
	char	*end_name;

	i = 0;
	while (split_args[i])
	{
		if (DEBUG == 1)
			printf("--> [NAMETOVAL] split_args[%d] ~%s~\n", i, split_args[i]);
		if (split_args[i][0] == '?')
			rejoined = handle_exit_status_case(
					sh, rejoined, split_args[i]);
		else
		{
			j = 0;
			while (split_args[i][j] && ft_isalnum_x_chr(&split_args[i][j], "_"))
				j++;
			end_name = &(split_args[i][j]);
			if (DEBUG == 1)
				printf("[NAMETOVAL] var ends on: ~%c~\n", split_args[i][j]);
			if (*end_name != '\0')
			{
				if (DEBUG == 1)
					printf("--> not NULL: handle_special_cases");
				rejoined = handle_special_cases(sh, rejoined,
						split_args[i], end_name);
			}
			else
			{
				rejoined = split_rejoin(sh, rejoined, split_args[i], '\0');
				if (DEBUG == 1)
					printf("--> NULL: rejoined: ~%s~\n", rejoined);
			}
		}
		i++;
	}
	return (rejoined);
}

char	*rejoin_subarg(char **subargs, char splitter, char *delim, int trailing)
{
	char	*arg;
	char	*temp;

	trailing++;trailing--; // not needed
	if (DEBUG == 1)
			printf("splitter: ~%c~\n", splitter);	
	if (splitter == '\0' || splitter == CC_VAR_BOUND)
		arg = join_all_subargs(subargs, 'n');
	else if (splitter == CC_SUBARG_DELIM && delim[0] == ' ')
	{	
		temp = ft_strdup(subargs[0]);
		free(subargs[0]);	
		subargs[0] = ft_strjoin(temp, delim);
		arg = join_all_subargs(subargs, 'n');
		free(temp);
	}
	else if (splitter == CC_SUBARG_DELIM
		&& ft_ispunct(delim[0]))
	{
		temp = ft_strdup(subargs[0]);
		free(subargs[0]);	
		subargs[0] = ft_strjoin(temp, delim);
		arg = join_all_subargs(subargs, 'n');
		free(temp);
	}
	else
		arg = join_all_subargs(subargs, 'n');
	return (arg);
}

char	*split_rejoin(t_shell *sh, char *rejoined_arg, char *arg, char splitter)
{
	char	**subargs;
	t_list	*set_var;
	int		trailing_splitter;
	char	*delim;

	if (DEBUG == 1)
		printf("[SPLIT_REJOIN] splitter: ~%c~ \n", splitter);
	trailing_splitter = (arg[ft_strlen(arg) - 1] == splitter);
	delim = to_delim(splitter);	
	if (DEBUG == 1)
		printf("trailing_splitter: ~%d~ - delim: ~%s~ \n", trailing_splitter, delim);
	if (!delim)
		return (NULL);
	if (ft_ispunct(splitter))
	{
		if (DEBUG == 1)
			printf("ispunct = splitter changed to CC_SUBARG_DELIM\n");
		splitter = CC_SUBARG_DELIM;
	}
	subargs = ft_split(arg, splitter);	
	if (DEBUG == 1)
		ft_show_strs(subargs, "subargs ");
	set_var = ft_getenv(subargs[0], &sh->this_env);
	subargs[0] = ft_setenv(set_var, subargs[0]);
	arg = rejoin_subarg(subargs, splitter, delim, trailing_splitter);
	if (DEBUG == 1)
		printf("arg: %s\n", arg);
	free(delim);
	free_args(subargs);
	if (!arg)
		return (NULL);
	rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
	if (DEBUG == 1)
		printf("rejoined_arg: %s\n", rejoined_arg);	
	free(arg);
	return (rejoined_arg);
}

// Set the delim str used to replace the splitter char (restore punct if apply)
char	*to_delim(char splitter)
{
	char	*delim;

	delim = malloc(2);
	if (!delim)
		return (NULL);
	delim[0] = '\0';
	if (splitter == CC_SPACE_IN_QUOTE
		|| splitter == CC_SUBARG_DELIM)
	{
		if (DEBUG == 1)
			printf("CC_SPACE_IN_QUOTE OR SUBARG_DELIM: splitter changed to space\n");		
		delim[0] = ' ';
	}
	else
	{
		if (DEBUG == 1)
			printf("delim = splitter\n");		
		delim[0] = splitter;
	}
	delim[1] = '\0';
	return (delim);
}
