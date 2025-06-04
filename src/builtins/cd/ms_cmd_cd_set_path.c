/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_cd_set_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:33:38 by mmalie            #+#    #+#             */
/*   Updated: 2025/06/04 23:36:13 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	cd_set_path(t_shell *sh, t_list *home_var, char **path)
{
	if (!sh->input_args[1] || ft_strcmp(sh->input_args[1], "~") == 0)
	{
		if (!home_var)
			return (ms_err("cd", "", HOME_NON_SET, 1));
		else if (home_var && !((char **)home_var->content)[1])
			return (0);
		*path = ((char **)home_var->content)[1];
	}
	else if (sh->input_args[2])
		return (ms_err("cd", "", TOO_MANY_ARGS, 1));
	else
		*path = sh->input_args[1];
	return (2);
}
