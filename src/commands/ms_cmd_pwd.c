/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/22 11:55:54 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	cmd_pwd(void)
{
	char	*cwd;	

	cwd = NULL;
	cwd = store_cwd(cwd);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

char	*store_cwd(char *cwd)
{
	char	buf[4096]; // Allegedly max path in Linux. Should excess be handled in another way (see man)?
	
	ft_memset(buf, 0, 4097);
	cwd = malloc(sizeof(char) * (ft_strlen(getcwd(buf, 4096)) + 1));
	if (!cwd)
		return (NULL);
	ft_strlcpy(cwd, buf, ft_strlen(buf) + 1);
	return (cwd);
}
