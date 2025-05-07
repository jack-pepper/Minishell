/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:01:23 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/08 00:22:26 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_exit(t_shell *sh, unsigned int status)
{
	size_t	nb_args;

	nb_args = ft_strslen(sh->input_args);
	printf("exit\n");
	if (ft_strcmp(sh->input_args[0], "exit") == 0)
	{
		if (nb_args == 1)
			;
		else if (nb_args > 2)
			return (ft_ret(1, EXIT_TOO_MANY_ARGS, STDERR));
		else if (nb_args == 2
			&& (ft_isnum(sh->input_args[1]) != 0
				|| ft_overflow(sh->input_args[1], "ll") != 0))
		{
			ft_putstr_fd(EXIT_NUM_ARG_REQ, STDERR);
			status = 2;
		}
		else
			status = (unsigned char)(ft_atoll(sh->input_args[1]));
		free_memory(sh);
		exit(status);
	}
	return (0);
}

// Check if all str chars are a digit while accepting + or - at first pos
int	ft_isnum(char *str)
{
	size_t	i;

	i = 0;
	if (!ft_isdigit(str[i]))
	{
		if (str[i] != '+' && str[i] != '-' && str[i + 1] != '\0')
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

// Trim each arg (remove + sign and leading zeros)
char	*ms_trim(char *trimmed_arg, char *arg, int len, int start_i)
{
	int	i;

	i = 0;
	if (ft_strnopbrk(arg, "0") == NULL || ft_strnopbrk(arg, "-0") == NULL
		|| ft_strnopbrk(arg, "+0") == NULL)
	{
		trimmed_arg = (char *)malloc(1 + 1);
		if (!trimmed_arg)
			return (NULL);
		ft_strlcpy(trimmed_arg, "0", 1);
		return (trimmed_arg);
	}
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i] == '0')
		i++;
	trimmed_arg = (char *)malloc((len - i + (arg[0] == '-') + 1));
	if (!trimmed_arg)
		return (NULL);
	if (arg[0] == '-')
		trimmed_arg[start_i++] = '-';
	while (i < len)
		trimmed_arg[start_i++] = arg[i++];
	trimmed_arg[start_i] = '\0';
	return (trimmed_arg);
}

// Check overflow. str must be strictly a decimal number (signed or unsigned)
int	exit_arg_overflow(char *str)
{
	size_t		str_len;
	char		*llong_min;
	char		*llong_max;
	char		*trimmed_str;
	int			res;

	trimmed_str = NULL;
	res = 0;
	trimmed_str = ms_trim(trimmed_str, str, ft_strlen(str), 0);
	str_len = ft_strlen(trimmed_str);
	if ((ft_isdigit(trimmed_str[0]) && str_len > 19)
		|| (trimmed_str[0] == '-' && str_len > 20))
		res = 1;
	else if (str_len == 19 || str_len == 20)
	{
		llong_min = "-9223372036854775808";
		llong_max = "9223372036854775807";
		if (trimmed_str[0] == '-')
			res = (ft_strcmp(trimmed_str, llong_min) < 0);
		else
			res = (ft_strcmp(trimmed_str, llong_max) > 0);
	}
	free(trimmed_str);
	return (res);
}
