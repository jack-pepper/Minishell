/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/06 14:18:39 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// like is digit, but accepts + or - at first pos
int	ft_isnum(char *str)
{
	// [TODO]: correct to handle "exit +, exit -"
	if (!ft_isdigit(*str) && *str != '+' && *str != '-')
	{
		//printf("Not a num!\n");
		return (1);
	}
	str++;
	while (*str != '\0')
	{
		//printf("%c\n", *str);
		if (!ft_isdigit(*str))
		{
		//	printf("Not a num (2)!\n");
			return (0);
		}
		str++;
	}
	return (1);
}

int	cmd_exit(t_shell *sh, unsigned int status)
{
	// [TODO]: should show the NUM_ARG_REQ error on overflow (and status to 2)
	// strnoprbk is not a fit: create a version of isnum accepting signs at start

	size_t	nb_args;

	nb_args = ft_strslen(sh->input_args);
	printf("exit\n");
	if (ft_strcmp(sh->input_args[0], "exit") == 0)
	{
		if (nb_args > 2)
			return (ft_ret(1, EXIT_TOO_MANY_ARGS, STDERR));
		else if (nb_args == 2)
		{
		//	if (ft_strnopbrk(sh->input_args[1], "0123456789-+") != NULL)
			if (!ft_isnum(sh->input_args[1]))
			{
				ft_putstr_fd(EXIT_NUM_ARG_REQ, STDERR);
				status = 2;
			}
			else
				status = (unsigned int)(ft_atoi(sh->input_args[1]));
		}
		free_memory(sh);
		exit(status);
	}
	return (0);
}
