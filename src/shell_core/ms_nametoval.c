/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/19 10:58:38 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// each arg starts with a var to interpret.
// To isolate the name:
// - find first space in quote 
// - if first char in arg is '?': exit status ()
// - else: interpret env
//




char	*ft_setenv(t_list *set_var, char *arg)
{
	free(arg);
	if (set_var != NULL)
		arg = ft_strdup(((char **)set_var->content)[1]);
	else
		arg = ft_strdup("");
	return (arg);
}

//char	*ft_interpret_arg()
//{
//	
//	ft_setenv
//}

char	*to_delim(char splitter)
{
	char	*delim;

	delim = malloc(2);
	if (!delim)
		return (NULL);
	delim[0] = '\0';
	if (splitter == CC_TRAILING_DOLLAR)
		delim[0] = '$';
	else if (splitter == CC_SPACE_IN_QUOTE)
		delim[0] = ' ';
	else // punct
		delim[0] = splitter;
	delim[1] = '\0';
	return (delim);
}

char	*split_rejoin(t_shell *sh, char *rejoined_arg, char *arg, char splitter)
{
	char	**subargs;
	t_list	*set_var;
	int	trailing_splitter;
	char	*temp;

	trailing_splitter = 0;
	if (arg[ft_strlen(arg) - 1] == splitter)
		trailing_splitter = 1;
	if (DEBUG == 1)
		printf("[arg last char] : %c\n", arg[ft_strlen(arg) - 1]);
	// split
	subargs = ft_split(arg, splitter);
	if (DEBUG == 1)
		ft_show_strs(subargs, "subargs ");

	set_var = ft_getenv(subargs[0], &sh->this_env);
	if (subargs[0] != NULL)
	{
		free(subargs[0]);
		if (set_var != NULL)
			subargs[0] = ft_strdup(((char **)set_var->content)[1]);
		else
			subargs[0] = ft_strdup("");
	}
	else
		subargs[0] = ft_strdup("");
	// rejoin all subargs
	if (splitter == '\0' || splitter == CC_VAR_BOUND)
		arg = join_all_subargs(subargs, 'n');
	else
	{
		if (trailing_splitter == 1)
		{
			temp = join_all_subargs(subargs, splitter);
			char *delim = to_delim(splitter);
			arg = ft_strjoin(temp, delim);
			free(delim);
			free(temp);
		}
		else
			arg = join_all_subargs(subargs, splitter);
	}
	free_args(subargs);
	rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
	free(arg);
	return (rejoined_arg);
}


char	*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
	int		i;
	int		j;
	char		end_name;

	i = 0;
	while (split_args[i])
	{
		if (DEBUG == 1)
			printf("[DEBUG start nametoval] %s\n", split_args[i]);
		if (split_args[i][0] != '?')
		{
			j = 0;
			while ((split_args[i][j] && (split_args[i][j] == '_' || ft_isalnum(split_args[i][j]))))
				j++;
			end_name = split_args[i][j];
			if (DEBUG == 1)
				printf("[DEBUG nametoval] end_name: %c\n", end_name);
			if (end_name != '\0') // can it be another char or control char?
			{
				if (ft_ispunct(end_name) && !ft_is_in_set(end_name, "$_/.")) // which one???
				{	
					if (DEBUG == 1)
						printf("[CASE PUNCT]\n");
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], end_name);
				}
				else if (end_name == CC_TRAILING_DOLLAR || end_name == '$') // $ABC123_$ : rejoin subargs with "$"
				{
					if (DEBUG == 1)
						printf("[CASE TRAILING_DOLLAR\n]");
					split_args[i][j] = CC_TRAILING_DOLLAR;
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], CC_TRAILING_DOLLAR);
				}
				else if (end_name == CC_SPACE_IN_QUOTE) // "$ABC YYY" : rejoin subargs with " "
				{
					if (DEBUG == 1)
						printf("[CASE SPACE IN QUOTE\n]");
					split_args[i][j] = CC_SUBARG_DELIM;
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], CC_SUBARG_DELIM);
				}
				else if (end_name == CC_VAR_BOUND) // $USER"ABC"ABC or ABC"$USER"ABC
				{	
					if (DEBUG == 1)
						printf("[CASE VAR BOUND\n]");
					rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], CC_VAR_BOUND);
				}
			}
			else // var ready to be tested
			{
				if (DEBUG == 1)
					printf("[CASE VAR READY\n]");
				rejoined_arg = split_rejoin(sh, rejoined_arg, split_args[i], '\0');
			}
		}
		else // $?
		{
			if (DEBUG == 1)
				printf("[CASE EXIT STATUS\n]");
			rejoined_arg = handle_exit_status_case(sh, rejoined_arg, split_args[i]);
			//rejoined_arg = ft_rejoin_subarg(rejoined_arg, split_args[i]);
		}
		i++;
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
	{
		//free(subarg);
		arg = ft_strdup(exit_status);
	}
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
	//free(arg);
	if (set_var != NULL)
		arg = ft_strdup(((char **)set_var->content)[1]);
	else
		arg = ft_strdup("");
        rejoined_arg = ft_rejoin_subarg(arg, rejoined_arg);
	return (rejoined_arg);
}



/*
char	*ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
	char	*first_space;
	char	*trailing_dollar;
	int		i;

	i = 0;
	while (split_args[i])
	{
		trailing_dollar = ft_strpbrk(split_args[i], (char[]){CC_TRAILING_DOLLAR, '\0'});
		if (trailing_dollar != NULL)
		{
			printf("[nametoval] Trailing dollar found!\n");
			rejoined_arg = handle_trailing_dollar_case(sh, rejoined_arg, split_args, i);
		}

		first_space = ft_strpbrk(split_args[i], (char[]){CC_SPACE_IN_QUOTE, '\0'});
		else if (first_space != NULL)
		{
			first_space[0] = CC_SUBARG_DELIM;
			rejoined_arg = handle_space_in_quote_case(sh, rejoined_arg, split_args, i);
		}
		else
		{
			if (split_args[i][0] == '?')
				split_args[i] = handle_exit_status_case(sh, split_args[i]);
			else
				split_args[i] = handle_var_case(sh, split_args, i);
			rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		}
		i++;
	}
	return (rejoined_arg);
}

char	*handle_trailing_dollar_case(t_shell *sh, char *rejoined_arg, char **split_args, int i)
{
	char	**subargs;
	t_list	*set_var;
	char	*var_bound;
	char	**subsubargs;

	subargs = ft_split(split_args[i], CC_TRAILING_DOLLAR);
	if (DEBUG == 1)
		ft_show_strs(subargs, "[case trailing dollar] subargs ");
	if (subargs[0][0] == '?')
		subargs[0] = handle_exit_status_case(sh, subargs[0]);
	else
	{
		var_bound = ft_strchr(subargs[0], CC_STICKY_VAR); 
		if (var_bound != NULL)
		{
			subsubargs = ft_split(subargs[0], CC_STICKY_VAR);
			if (subsubargs == NULL)
				return (NULL);
			if (DEBUG == 1)
				ft_show_strs(subsubargs, "[nametoval] subsubargs ");
			set_var = ft_getenv(subsubargs[0], &sh->this_env);
			free(subsubargs[0]);
			if (set_var != NULL)
				subsubargs[0] = ft_strdup(((char **)set_var->content)[1]);
			else
				subsubargs[0] = ft_strdup("");
			free(subargs[0]);
			subargs[0] = join_all_subargs(subsubargs, 'n');
			free_args(subsubargs);
		}
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
	}
	if (subargs[1] != NULL)
		split_args[i] = ft_strjoin_delim(subargs[0], subargs[1], "$");
	else
	        split_args[i] = ft_strjoin_delim(subargs[0], "", "$");
	free_args(subargs);
	rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
	return (rejoined_arg);
}

char	*handle_space_in_quote_case(t_shell *sh, char *rejoined_arg, char **split_args, int i)
{
	char	**subargs;
	t_list	*set_var;
	char	*var_bound;
	char	**subsubargs;

	subargs = ft_split(split_args[i], CC_SUBARG_DELIM);
	if (DEBUG == 1)
		ft_show_strs(subargs, "[case space in quote] subargs ");
	if (subargs[0][0] == '?')
		subargs[0] = handle_exit_status_case(sh, subargs[0]);
	else
	{
		var_bound = ft_strchr(subargs[0], CC_STICKY_VAR); 
		if (var_bound != NULL)
		{
			subsubargs = ft_split(subargs[0], CC_STICKY_VAR);
			if (subsubargs == NULL)
				return (NULL);
			if (DEBUG == 1)
				ft_show_strs(subsubargs, "[nametoval] subsubargs ");
			set_var = ft_getenv(subsubargs[0], &sh->this_env);
			free(subsubargs[0]);
			if (set_var != NULL)
				subsubargs[0] = ft_strdup(((char **)set_var->content)[1]);
			else
				subsubargs[0] = ft_strdup("");
			free(subargs[0]);
			subargs[0] = join_all_subargs(subsubargs, 'n');
			free_args(subsubargs);
		}
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
	}
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
		//printf("temp: %s\n", temp);
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

char	*handle_var_case(t_shell *sh, char **split_args, int i)
{
	char	**split_subargs;
	t_list	*set_var;

	if (ft_strrchr(split_args[i], CC_STICKY_VAR) != NULL)
	{
		split_subargs = ft_split(split_args[i], CC_STICKY_VAR);
		if (DEBUG == 1)
			ft_show_strs(split_subargs, "[split_subargs]");
		set_var = ft_getenv(split_subargs[0], &sh->this_env);
		free(split_subargs[0]);
		if (set_var != NULL)
		{
			split_subargs[0] = ft_strdup(((char **)set_var->content)[1]);
			if (DEBUG == 1)
				printf("split_subargs[0]: %s\n", split_subargs[0]);
		}
		else
			split_subargs[0] = ft_strdup("");
		free(split_args[i]);
		split_args[i] = join_all_subargs(split_subargs, 'n');
		free_args(split_subargs);
	}
	else
	{
		if (DEBUG == 1)
			printf("[nametoval - split_args[%d]] %s\n", i, split_args[i]);
		set_var = ft_getenv(split_args[i], &sh->this_env);
		free(split_args[i]);
		if (set_var != NULL)
			split_args[i] = ft_strdup(((char **)set_var->content)[1]);
		else
			split_args[i] = ft_strdup("");
	}
	return (split_args[i]);
}
*/
