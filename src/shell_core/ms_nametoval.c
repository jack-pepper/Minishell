/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/23 15:49:36 by mmalie           ###   ########.fr       */
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
		if (split_args[i][0] == '?')
			rejoined = handle_exit_status_case(
					sh, rejoined, split_args[i]);
		else
		{
			j = 0;
			while (split_args[i][j] && ft_isalnum_x_chr(&split_args[i][j], "_"))
				j++;
			end_name = &(split_args[i][j]);
			if (*end_name != '\0')
				rejoined = handle_special_cases(sh, rejoined,
						split_args[i], end_name);
			else
				rejoined = split_rejoin(sh, rejoined, split_args[i], '\0');
		}
		i++;
	}
	return (rejoined);
}

char	*rejoin_subarg(char **subargs, char splitter, char *delim, int trailing)
{
	char	*arg;
	char	*temp;

//	printf("splitter = %c - delim: %s - trailing: %d\n", splitter, delim, trailing);
//	ft_show_strs(subargs, "[before rejoin]" );
	if (splitter == '\0' || splitter == CC_VAR_BOUND)
	{
//		printf("CASE 1\n");
		arg = join_all_subargs(subargs, 'n');
	}
	else if (splitter == CC_TRAILING_DOLLAR)
	{
		if (trailing == 0)
		{
//			printf("CC_TRAILING_DOLLAR + trailing = 0\n");
			temp = ft_strjoin(subargs[0], delim);
			arg = join_all_subargs(subargs, 'n');
			//arg = ft_strjoin(temp, delim);
			free(temp);
		}
		else if (trailing == 1)
		{
//			printf("CC_TRAILING_DOLLAR + trailing = 1\n");
			arg = join_all_subargs(subargs, delim[0]);
		}
	}
	else if (splitter == CC_SUBARG_DELIM && delim[0] == ' ')
	{
//		printf("SPLITCASE\n");
		arg = join_all_subargs(subargs, delim[0]);
	}
	else if (splitter == CC_SUBARG_DELIM && ft_ispunct(delim[0]) && delim[0] != '$')
	{
		temp = join_all_subargs(subargs, 'n');
		printf("temp: %s\n", temp);
		arg = ft_strjoin(temp, delim);
		printf("arg: %s\n", arg);
		free(temp);
	}
	else
	{
//		printf("CASE 3 - normal quote or delim logic\n");
		arg = join_all_subargs(subargs, 'n');
	}
	return (arg);

}

char	*split_rejoin(t_shell *sh, char *rejoined_arg, char *arg, char splitter)
{
	char	**subargs;
	t_list	*set_var;
	int		trailing_splitter;
	char	*delim;

	trailing_splitter = (arg[ft_strlen(arg) - 1] == splitter);
	delim = to_delim(splitter);
	if (!delim)
		return (NULL);
	if (ft_ispunct(splitter) && delim[0] != '$')
		splitter = CC_SUBARG_DELIM;
	subargs = ft_split(arg, splitter);
	//ft_show_strs(subargs, "[DEB] subargs ");
	set_var = ft_getenv(subargs[0], &sh->this_env);
	subargs[0] = ft_setenv(set_var, subargs[0]);
	arg = rejoin_subarg(subargs, splitter, delim, trailing_splitter);
	free(delim);
	free_args(subargs);
	if (!arg)
		return (NULL);
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
	else
		delim[0] = splitter;
	delim[1] = '\0';
	return (delim);
}
