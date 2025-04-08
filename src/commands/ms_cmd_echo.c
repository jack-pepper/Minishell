/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:00:15 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/08 19:25:31 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_echo(t_shell *sh)
{
	char	*joined_input;
	bool	opt_n;
	int	i;

	if (!sh->input_args || !(sh->input_args[1])) // Does bash output a newline if no args as zsh?
		return ;
	i = 1;
	opt_n = false;
	if (ft_strncmp(sh->input_args[1], "-n", ft_strlen(sh->input_args[1])) == 0)
	{
		opt_n = true;
		i++;
	}
	joined_input = ft_strdup("");
	if (!joined_input)
		return ;
	while (sh->input_args[i] != NULL)
	{		
		joined_input = ft_strjoin(joined_input, sh->input_args[i]);
		if (!joined_input)
			return ;
		i++;
		if (sh->input_args[i] != NULL)
		{
			joined_input = ft_strjoin(joined_input, " ");
			if (!joined_input)
				return ;
		}
	}
	printf("%s", joined_input);
	if (opt_n == false) // My guess here... is that this behavior matches bash's.
		printf("\n"); // It might be worth (for bonus) using the "%" method as zsh?
	free(joined_input);
}
