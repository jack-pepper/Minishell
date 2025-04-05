/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/05 10:29:36 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_export(char **input_args, t_list **this_env)
{
	t_list	*last_node;
	t_list	*set_var;
	char	**split_str;
	int	i;

	if (!input_args)
		return ;
	if (!(input_args[1])) // if no args, man says result is unspecified
		cmd_env((*this_env)); // ... we may just display the env variables
	else
	{
		i = 1;
		while (input_args[i])
		{
			split_str = ft_split(input_args[i], '=');
			if (!split_str)
				return ;
			set_var = ft_getenv(input_args[i], this_env);
			if (set_var != NULL)
			{
				if (update_env_value(set_var, split_str) != 0)
					return ;
			}
			else
			{
				last_node = ft_lstlast(*this_env);
				last_node->next = ft_lstnew((char **)split_str);
				if (!last_node->next)
					return ;
			}
			i++;
		}
	}
	return ;
}
