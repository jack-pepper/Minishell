/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_consec_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 06:24:39 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/12 06:29:36 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	check_consecutive_redir(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (is_token_control_char(args[i], CC_HEREDOC)
			|| is_token_control_char(args[i], CC_REDIR_IN)
			|| is_token_control_char(args[i], CC_REDIR_OUT)
			|| is_token_control_char(args[i], CC_APPEND)
			|| is_token_control_char(args[i], CC_PIPE))
		{
			if (!args[i + 1])
				return (false);
			if (is_token_control_char(args[i + 1], CC_HEREDOC)
				|| is_token_control_char(args[i + 1], CC_REDIR_IN)
				|| is_token_control_char(args[i + 1], CC_REDIR_OUT)
				|| is_token_control_char(args[i + 1], CC_APPEND)
				|| is_token_control_char(args[i + 1], CC_PIPE))
				return (false);
		}
		i++;
	}
	return (true);
}
