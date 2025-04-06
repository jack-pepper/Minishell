/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:00:15 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/05 16:46:45 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_echo(char **input_args, t_list **env)
{
	char	*joined_input;
	bool	opt_n;
	int	i;

	env++; //
	env--; //
	if (!input_args || !(input_args[1])) // Does bash output a newline if no args as zsh?
		return ;
	i = 1;
	opt_n = false;
	if (ft_strncmp(input_args[1], "-n", ft_strlen(input_args[1])) == 0)
	{
		opt_n = true;
		i++;
	}
	joined_input = ft_strdup("");
	if (!joined_input)
		return ;
	while (input_args[i] != NULL)
	{		
		joined_input = ft_strjoin(joined_input, input_args[i]);
		if (!joined_input)
			return ;
		i++;
		if (input_args[i] != NULL)
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
