/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/27 15:14:21 by mmalie           ###   ########.fr       */
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
	cwd = getcwd(NULL, 0); // Better than buf[4096]: malloc directly the needed size (see man getcwd)!
	if (!cwd)
		return (NULL);
	return (cwd);
}
