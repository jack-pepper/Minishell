/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/07 14:50:20 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Check if all str chars are a digit while accepting + or - at first pos
int	ft_isnum(char *str)
{
	size_t	i;

	i = 0;
	if (!ft_isdigit(str[i]))
	{
		if (str[i] != '+' && str[i] != '-'
			&& str[i + 1] != '\0')
		return (1);
	}
	i++;
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

// Check overflow. str must be strictly a decimal number (signed or unsigned)
int	ft_overflow(char *str, char *type)
{
	long long	num;
	size_t		str_len;
	char		*llong_min;
	char		*llong_max;

	// first strip the leading white spaces and leading zeros (see pushswap)
	num = ft_atoll(str);
	if (ft_strcmp(type, "int") == 0)
		return (num < INT_MIN || num > INT_MAX);
	else if (ft_strcmp(type, "ll") == 0)
	{
		str_len = ft_strlen(str);

		if ((ft_isdigit(str[0]) && str_len > 19)
			|| (str[0] == '+' && str_len > 20)
			|| (str[0] == '-' && str_len > 20))
			return (1);
		if (str_len == 19 || str_len == 20)
		{
			llong_min = "-9223372036854775808";
			llong_max = "9223372036854775807";
			if (str[0] == '-')
				return (ft_strcmp(str, llong_min) < 0);
			else if (str[0] == '+')
				return (ft_strcmp(&str[1], llong_max) > 0);
			else
				return (ft_strcmp(str, llong_max) > 0);
		}
		else
			return (0);
	}
	return (0);
}

int	cmd_exit(t_shell *sh, unsigned int status)
{
	// [TODO]: should show the NUM_ARG_REQ error on overflow (and status to 2)

	size_t	nb_args;
	//long long	num;

	nb_args = ft_strslen(sh->input_args);
	printf("exit\n");
	if (ft_strcmp(sh->input_args[0], "exit") == 0)
	{
		if (nb_args == 1)
			;
		else if (nb_args > 2)
			return (ft_ret(1, EXIT_TOO_MANY_ARGS, STDERR));
		else if (nb_args == 2 && 
			(ft_isnum(sh->input_args[1]) != 0 || ft_overflow(sh->input_args[1], "ll") != 0))
		{
			ft_putstr_fd(EXIT_NUM_ARG_REQ, STDERR);
			status = 2;
		}
		else
		{	
			//num = ft_atoll(sh->input_args[1]);
			status = (unsigned int)(ft_atoll(sh->input_args[1]));
		}
		free_memory(sh);
		exit(status);
	}
	return (0);
}
