/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_replace_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:40:44 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/06 17:18:45 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_replace_if_space(char *cur_c, char new_c)
{
	if (ft_isspace(*cur_c))
		*cur_c = new_c;
}

void	ft_replace_char(char *cur_c, char new_c)
{
	*cur_c = new_c;
}

void	ft_replace_all_chars(char **input_args, char old, char new)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input_args[i])
	{
		j = 0;
		while (input_args[i][j])
		{
			if (input_args[i][j] == old)
				input_args[i][j] = new;
			j++;
		}
		i++;
	}
}

// If delim is repeated or met at end or start, flag it so it can be converted back later
void    ft_flag_delim(char *str, char delim, char flag)
{
        size_t  i;

        i = 0;
        if (str[0] == delim)
                str[0] = flag;
        while (str[i + 1] != '\0')
        {
                if (str[i] == delim && str[i + 1] == delim)
                        str[i] = flag;
                i++;
        }
        if (str[i] == delim)
                str[i] = flag;
}

void	ft_unflag_delim(char *str, char delim, char flag)
{
	size_t	i;

	i = 0;
	if (!str || str[i] == '\0')
		return;
	while (str[i] != '\0')
	{
		if (str[i] == flag)
			str[i] = delim;
		i++;
	}
}
