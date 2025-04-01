/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/01 11:23:11 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_export(char **input_args, char **env)
{
	//char	*equal_sign;

	printf("[cmd_export]\n");
	if (!input_args)
		return ;
	if (!(input_args[1])) // if no args, man says result is unspecified
	{
		cmd_env(env); // ... we may just display the env variables
		return ;
	}
	
	// TEST
	else
	{
		size_t nb = ft_strslen(env);
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
