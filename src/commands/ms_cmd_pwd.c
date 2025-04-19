/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/18 22:58:45 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_pwd(void)
{
	char	buf[4096]; // Allegedly max path in Linux. Should excess be handled in another way (see man)?
	char	*cwd;	

	ft_memset(buf, 0, 4097);

	cwd = malloc(sizeof(char) * (ft_strlen(getcwd(buf, 4096)) + 1));
	if (!cwd)
		return (-1);

	ft_strlcpy(cwd, buf, ft_strlen(buf) + 1);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
