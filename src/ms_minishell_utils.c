/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_minishell_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 11:40:20 by yel-bouk          #+#    #+#             */
/*   Updated: 2025/05/31 11:40:55 by yel-bouk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_error_code(char *error_msg, t_shell *sh, int code)
{
	perror(error_msg);
	sh->last_exit_status = code;
}

int	validate_direct_path(char **argv, t_shell *sh)
{
	struct stat	st;

	if (access(argv[0], F_OK) != 0)
	{
		print_error_code("No such file or directory", sh, 127);
		return (1);
	}
	if (stat(argv[0], &st) != 0)
	{
		print_error_code(argv[0], sh, 127);
		return (1);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd(" Is a directory\n", 2);
		sh->last_exit_status = 126;
		return (1);
	}
	if (access(argv[0], X_OK) != 0)
	{
		print_error_code(argv[0], sh, 126);
		return (1);
	}
	return (0);
}

int	validate_in_path(char **argv, char **envp, t_shell *sh)
{
	char	*cmd_path;

	cmd_path = get_cmd_path(argv[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd(" command not found\n", 2);
		sh->last_exit_status = 127;
		return (1);
	}
	free(cmd_path);
	return (0);
}
