/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/20 14:34:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
	int		i;
	int		j;
	char		end_name;

	i = 0;
	while (split_args[i])
	{
		if (DEBUG == 1) printf("[DEBUG start nametoval] %s\n", split_args[i]);
		if (split_args[i][0] == '?')
			rejoined_arg = handle_exit_status_case(sh, rejoined_arg, split_args[i]);
		else
		{
			j = 0;
			while (split_args[i][j] && ft_isalnum_x_chr(&split_args[i][j], "_"))
				j++;
			end_name = split_args[i][j];
			if (DEBUG == 1)	printf("[DEBUG nametoval] end_name: %c\n", end_name);
			if (end_name != '\0') // can it be another char or control char?
			{
				if (ft_ispunct(end_name) && !ft_is_in_set(end_name, "$")) // which one???
				{	
					//if (DEBUG == 1) printf("[CASE PUNCT]\n");
					split_args[i][j] = CC_SUBARG_DELIM;
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], end_name);
				}
				else if (end_name == CC_TRAILING_DOLLAR || end_name == '$') // $ABC123_$ : rejoin subargs with "$"
				{
					//if (DEBUG == 1) printf("[CASE TRAILING_DOLLAR\n]");
					split_args[i][j] = CC_TRAILING_DOLLAR;
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], CC_TRAILING_DOLLAR);
				}
				else if (end_name == CC_SPACE_IN_QUOTE) // "$ABC YYY" : rejoin subargs with " "
				{
					//if (DEBUG == 1) printf("[CASE SPACE IN QUOTE\n]");
					split_args[i][j] = CC_SUBARG_DELIM;
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], CC_SUBARG_DELIM);
				}
				else if (end_name == CC_VAR_BOUND) // $USER"ABC"ABC or ABC"$USER"ABC
				{	
					//if (DEBUG == 1) printf("[CASE VAR BOUND\n]");
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], CC_VAR_BOUND);
				}
			}
			else // var ready to be tested
			{
				// if (DEBUG == 1) printf("[CASE VAR READY\n]");
				rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], '\0');
			}
		}
		i++;
	}
	return (rejoined_arg);
}

char	*split_rejoin(t_shell *sh, char *rejoined_arg, char *arg, char splitter)
{
	char	**subargs;
	t_list	*set_var;
	int	trailing_splitter;
	char	*temp;
	char	*delim;

	trailing_splitter = 0;
	if (arg[ft_strlen(arg) - 1] == splitter)
		trailing_splitter = 1;	
	delim = to_delim(splitter);
	if (DEBUG == 1)	
		printf("delim: %s\n", delim);
	if (!delim)
		return (NULL);
	if (ft_ispunct(splitter))
		splitter = CC_SUBARG_DELIM;
	if (DEBUG == 1)
		printf("[arg last char] : %c\n", arg[ft_strlen(arg) - 1]);
	subargs = ft_split(arg, splitter);
	if (DEBUG == 1)
		ft_show_strs(subargs, "subargs ");
	set_var = ft_getenv(subargs[0], &sh->this_env);
	subargs[0] = ft_setenv(set_var, subargs[0]);
	if (splitter == '\0' || splitter == CC_VAR_BOUND)
		arg = join_all_subargs(subargs, 'n');
	else if ((trailing_splitter == 0 && ft_ispunct(delim[0]))
		|| (trailing_splitter == 1 && !ft_ispunct(delim[0])))
	{
		temp = join_all_subargs(subargs, *delim);
		arg = ft_strjoin(temp, delim);
		free(temp);
	}
	else
		arg = join_all_subargs(subargs, *delim);
	free(delim);
	free_args(subargs);
	rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
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
	if (splitter == CC_TRAILING_DOLLAR)
		delim[0] = '$';
	else if (splitter == CC_SPACE_IN_QUOTE
		|| splitter == CC_SUBARG_DELIM)
		delim[0] = ' ';
	else // punct
		delim[0] = splitter;
	delim[1] = '\0';
	return (delim);
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
