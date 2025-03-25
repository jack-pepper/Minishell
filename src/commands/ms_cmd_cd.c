/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:59:33 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/25 19:31:03 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_cd(char *path)
{
	// if no path? Should return to /home/$USER ?

	if (chdir(path) != 0)
		return (-1);
	return (0);
}
