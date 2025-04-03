/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/03 22:24:55 by mmalie           ###   ########.fr       */
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
//	t_list	**node;
//	t_list	*new_var;
//	char	**set_var;
//	char	*equal_appended;
//	int	i;
//	int	j;
//	int	nb_of_equal;

//	new_var = NULL;
	if (!input_args)
		return ;
	if (!(input_args[1])) // if no args, man says result is unspecified
	{
		cmd_env((*this_env)); // ... we may just display the env variables
		return ;
	}
} // rm	
/*
	else
	{
		i = 1;
		j = 0;
		node = this_env;
		while (input_args[i]) // parsing probably uncomplete
		{


			set_name = ft_getenv(input_args[i], this_env);
			if (set_name != NULL)
			{
				set_name[1]// update the var
			}
			else
			{
				// add a new var
			}


			if (ft_strncmp(input_args[i], node->content[0], ft_strlen(input_args[i])) == 0)
			{
				
			}

			// if input_args[i] = any content[0]
			
			// if no equal: copy to this_env->content[0]
			
			// if one equal: split (delim: equal) and copy split[0] to this_env->content[0] and 1 to 1



			// if not in the list


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
} */
