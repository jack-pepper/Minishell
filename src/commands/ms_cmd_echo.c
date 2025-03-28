/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:00:15 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/28 22:46:25 by mmalie           ###   ########.fr       */
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

/* Function written before I started using my brain to solve the trailing
 * newline issue with -n. Keeping for sensible use somewhere else.
char	*ft_remove_newline(char *str)
{
	char	*trimmed_str;
	size_t	str_len;

	if (!str)
		return (NULL);
	str_len = ft_strlen(str);
	printf("str_len: %lu", str_len);
	if (str[str_len] == '\n')
	{
		trimmed_str = malloc(sizeof(char) * (str_len + 1));
		if (!trimmed_str)
			return (str);
		ft_strlcpy(trimmed_str, str, str_len);
		free(str);
		return (trimmed_str);
	}
	else
		return (str);
}
*/
