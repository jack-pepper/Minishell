/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cmd_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 13:03:40 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/17 13:20:58 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cmd_export(t_shell *sh)
{
	// BASH: would NOT accept "export AAA" (only: export AAA=").

	t_list	*last_node;
	t_list	*set_var;
	char	**split_str;
	int	i;
	t_list	*stashed_var;

	if (!sh->input_args)
		return ;
	if (!(sh->input_args[1])) // if no args, man says result is unspecified
		cmd_env((sh)); // ... we may just display the env variables
	else
	{
		i = 1;
		while (sh->input_args[i])
		{
			// if arg without = ...
			if (ft_strchr(sh->input_args[i], '=') == NULL)
			{
				stashed_var = NULL;
				stashed_var = ft_getenv(sh->input_args[i], &env_stash);
				if (stashed_var != NULL) // if this arg is in env_stash...
					// update env value
				else // if it is not: INVALID, stop and cmd not found 
					// cmd not found
			}

			else // if the arg contains a '=', not at first index

			// 1-check (ft_strchr) if the arg contains a "=" and it's not first)
			// 2- if no "=": check the env STASH
			// 3- if there is a "=": check if VAR_NAME already exists and updates it, or creates new one if not

			split_str = NULL;
			split_str = ft_split(sh->input_args[i], '=');
			if (!split_str)
				return ;

			// export_update_var

			set_var = ft_getenv(split_str[0], &sh->this_env);
			if (set_var != NULL)
			{
				if (ft_update_env_value(set_var, split_str) != 0)
				{
					free_args(split_str);
					return ;
				}
			}
			else
			{
				last_node = ft_lstlast(sh->this_env);
				last_node->next = ft_lstnew((char **)split_str);
				if (!last_node->next)
				{
					free_args(split_str);
					return ;
				}
			}
			i++;
		}
	}
}

void	export_stash_var(t_shell *sh)
{
	t_list	*node;
	char    **split_str;
        size_t  i;
	size_t	nb_args;

        i = 0;
        node = NULL;
        split_str = NULL;
	nb_args = ft_strslen(sh->input_args);
	printf("[export_stash_var] nb_args: %ld", nb_args); // DEBUG
        while (i < nb_args)
        {
		split_str = ft_split(sh->input_args[i], '=');
                if (!split_str)
			return ;
		node = ft_lstnew((char **)split_str);
		if (!node)
			return ;
		ft_lstadd_back(&sh->env_stash, node);
                i++;
        }
        return ;
}

/*	size_t	i;
	size_t	arg_len;

	if (sh->env_stash == NULL)
	{
		sh->env_stash = (char **)malloc(sizeof(char **));
		if (!sh->env_stash)
			return ;
		i = 0;
	}
	else
		i = ft_strslen(sh->env_stash);
	printf("stashing var: %s\n", sh->input_args[0]);
	arg_len = ft_strlen(sh->input_args[0]);
	sh->env_stash[i] = malloc(sizeof(char) * (arg_len + 1));
	if (!sh->env_stash[i])
		return ;
	ft_strlcpy(sh->env_stash[i], sh->input_args[0], arg_len);
	//sh->env_stash[arg_len] = '\0';
	printf("Stashed: ~%s~\n", sh->env_stash[i]);
	printf("Nb of strs in stash: %ld\n", ft_strslen(sh->env_stash));

}*/
