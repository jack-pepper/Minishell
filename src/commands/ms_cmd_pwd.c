/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/25 14:13:06 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_pwd(void)
{
	char	buf[4096]; // Allegedly max path in Linux. Should excess be handled in another way (see man)?
	char	*cwd;	

	ft_memset(buf, 0, 4097);

	printf("[cmd_pwd]\n");
	cwd = malloc(sizeof(char) * (ft_strlen(getcwd(buf, 4096)) + 1));
	if (!cwd)
		return ;

	ft_strlcpy(cwd, buf, ft_strlen(buf) + 1);
	printf("%s\n", cwd);
	return ;
}
