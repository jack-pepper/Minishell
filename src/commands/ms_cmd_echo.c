/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:00:15 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/28 18:24:52 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_echo(char **input_args, char **env)
{
	char	*joined_input;
	bool	opt_n;
	int	i;

	env++; //
	env--; //
	if (!input_args || !(input_args[1]))
		return ;
	
	i = 1;
	opt_n = false;
	if (ft_strncmp(input_args[1], "-n", ft_strlen(input_args[1])) == 0)
	{
		opt_n = true;
		i++;
	}
	joined_input = ft_strdup("");
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
	if (opt_n == true)
		printf("opt n\n");// write a function to remove the trailing newline from joined_input
	printf("%s\n", joined_input);
	free(joined_input);
}
