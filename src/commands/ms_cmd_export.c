/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 19:56:42 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*void	cmd_export(char **input_args, t_list **this_env)
{
	t_list	*new_var;
	char	*equal_appended;
	int	i;
	int	nb_of_equal;

	new_var = NULL;
	if (!input_args)
		return ;
	if (!(input_args[1])) // if no args, man says result is unspecified
	{
		cmd_env((*this_env)); // ... we may just display the env variables
		return ;
	}
	else
	{
		i = 1;
		while (input_args[i]) // parsing probably uncomplete
		{
			// SHOULD CHECK whether the var already exists, and update it accordingly
			nb_of_equal = ft_count_char(input_args[i], '=');
			if ((input_args[i][0] != '=') // first char not an equal
				&& (nb_of_equal <= 1)) // can't be more than one equal
			{
				if (nb_of_equal == 0)
				{
					equal_appended = ft_strjoin(input_args[i], "=");
					if (!equal_appended)
						return ;	
					new_var = ft_lstnew(((char *)equal_appended));
				}
				else
					new_var = ft_lstnew((char *)input_args[i]);
				if (!new_var)
					return ;
				ft_lstadd_back(this_env, new_var);
			}
			i++;
		}
	}
}
*/

void	cmd_export(char **input_args, t_list **this_env)
{
	t_list	*new_var;
	char	*equal_appended;
	int	i;
	int	nb_of_equal;

	new_var = NULL;
	if (!input_args)
		return ;
	if (!(input_args[1])) // if no args, man says result is unspecified
	{
		cmd_env((*this_env)); // ... we may just display the env variables
		return ;
	}
	else
	{
		i = 1;
		while (input_args[i]) // parsing probably uncomplete
		{
			// SHOULD CHECK whether the var already exists, and update it accordingly
			nb_of_equal = ft_count_char(input_args[i], '=');
			if ((input_args[i][0] != '=') // first char not an equal
				&& (nb_of_equal <= 1)) // can't be more than one equal
			{
				if (nb_of_equal == 0)
				{
					equal_appended = ft_strjoin(input_args[i], "=");
					if (!equal_appended)
						return ;	
					new_var = ft_lstnew(((char *)equal_appended));
				}
				else
					new_var = ft_lstnew((char *)input_args[i]);
				if (!new_var)
					return ;
				ft_lstadd_back(this_env, new_var);
			}
			i++;
		}
	}
}
