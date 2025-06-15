/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:51 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/15 19:45:06 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	cmd_pwd(t_shell *sh)
{
	t_list	*pwd_var;
	char	*cwd;

	pwd_var = ft_getenv("PWD", &sh->this_env);
	if (pwd_var && ((char **)pwd_var->content)[1])
	{
		cwd = ((char **)pwd_var->content)[1];
		printf("%s\n", cwd);
		return (0);
	}
	cwd = NULL;
	cwd = store_cwd(sh, cwd);
	if (cwd == NULL)	
		return (ms_err("pwd", "", NO_CUR_DIR, 1));
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}

char	*store_cwd(t_shell *sh, char *cwd)
{
	t_list	*pwd_var;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		pwd_var = ft_getenv("PWD", &sh->this_env);
		if (pwd_var && (const char *)((char **)pwd_var->content)[1])
			cwd = ft_strdup((const char *)((const char **)pwd_var->content)[1]);
		return (cwd);
	}
	return (cwd);
}
