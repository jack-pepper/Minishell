/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/02 11:13:43 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_export(char **input_args, t_list **this_env)
{
	printf("[cmd_export]\n");
	if (!input_args)
		return ;
	if (!(input_args[1])) // if no args, man says result is unspecified
	{
		cmd_env(this_env); // ... we may just display the env variables
		return ;
	}
	
	// TEST
	else   // ADD NODE TO LIST
	{

		//size_t i = ft_strslen(this_env);


	/*	new_env = malloc(sizeof(char *) * (i + 2)); // replace the number by the number of env var to add + 1
		if (!new_env)
			return ;
		ft_memmove(new_env, this_env, sizeof(char *) * i);
	
		new_env[i] = ft_strdup("ZZZ=aaa");
		if (!new_env[i])
			return ;
		new_env[i + 1] = NULL;
		this_env = new_env;
	*/

	}

	/*
	else
	{
		equal_sign = ft_strchr(input_args[1], '=');
		if (equal_sign == NULL)
		{
			//
		}
	}
	// ...
	*/
}
