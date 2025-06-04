/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/04 11:39:36 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	cmd_pwd(void)
{
	char	*cwd;	

	cwd = NULL;
	cwd = store_cwd(cwd);
	if (cwd != NULL)
		printf("%s\n", cwd);
	free(cwd);
	return (0);
}

char	*store_cwd(char *cwd)
{
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}
