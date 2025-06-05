/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_handler_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 12:05:52 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/06/05 12:14:47 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/minishell.h"

// bool	validate_all_redirections(char **tokens, t_shell *sh)
// {
// 	int	i;

// 	i = 0;
// 	while (tokens[i])
// 	{
// 		if (is_token_control_char(tokens[i], CC_REDIR_IN)
// 			|| is_token_control_char(tokens[i], CC_HEREDOC))
// 		{
// 			if (!handle_input_redirection(tokens, &i, sh))
// 				return (false);
// 			continue;
// 		}
// 		else if (is_token_control_char(tokens[i], CC_REDIR_OUT)
// 			|| is_token_control_char(tokens[i], CC_APPEND))
// 		{
// 			if (!handle_output_redirection(tokens, &i, sh))
// 				return (false);
// 			continue;
// 		}
// 		else if (is_token_control_char(tokens[i], CC_PIPE))
// 		{
// 			if (!handle_pipe(tokens, i, sh))
// 				return (false);
// 		}
// 		i++;
// 	}
// 	return (true);
// } 