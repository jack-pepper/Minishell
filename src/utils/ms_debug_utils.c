/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_debug_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:29:10 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/09 11:29:30 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Display the content of strs
void	ft_show_strs(char **strs, char *debug_msg)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		printf("%s: strs[%d]: %s\n", debug_msg, i, strs[i]);
		i++;
	}
}

// Save a line on returning by sending a message to a fd
int	ft_ret(int return_val, char *msg, int fd)
{
	ft_putstr_fd(msg, fd);
	return (return_val);
}
