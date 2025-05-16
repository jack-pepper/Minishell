/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_nametoval.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:03:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/16 23:34:58 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// each arg starts with a var to interpret.
// To isolate the name:
// - find first space in quote 
// - if first char in arg is '?': exit status ()
// - else: interpret env
//
char    *ft_nametoval(t_shell *sh, char *rejoined_arg, char **split_args)
{
        char    *first_space;
        int             i;

        i = 0;
        while (split_args[i])
	{
		first_space = ft_strpbrk(split_args[i], (char[]){CTRL_CHAR_SPACE_IN_QUOTE, '\0'});
		if (first_space != NULL)
		{
			first_space[0] = CTRL_CHAR_SUBARG_DELIM;
			rejoined_arg = handle_space_in_quote_case(sh, rejoined_arg, split_args, i);
		}
                else
                {
                        if (split_args[i][0] == '?')
                                split_args[i] = handle_exit_status_case(sh, split_args[i]);
                        else
				split_args[i] = handle_var_case(sh, split_args, i);
			rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
		}
		i++;
	}
	return (rejoined_arg);
}

char    *handle_space_in_quote_case(t_shell *sh, char *rejoined_arg, char **split_args, int i)
{
        char    **subargs;
        t_list  *set_var;

        subargs = ft_split(split_args[i], CTRL_CHAR_SUBARG_DELIM);
        ft_show_strs(subargs, "[case space in quote] subargs ");
	if (subargs[0][0] == '?')
                subargs[0] = handle_exit_status_case(sh, subargs[0]);
        else
        {
                set_var = ft_getenv(subargs[0], &sh->this_env);
                free(subargs[0]);
                if (set_var != NULL)
                        subargs[0] = ft_strdup(((char **)set_var->content)[1]);
                else
                        subargs[0] = ft_strdup("");
        }
        free(split_args[i]);
        if (subargs[1] != NULL)
                split_args[i] = ft_strjoin_delim(subargs[0], subargs[1], " ");
        else
                split_args[i] = ft_strjoin_delim(subargs[0], "", " ");
        free_args(subargs);
        rejoined_arg = ft_rejoin_subarg(split_args, rejoined_arg, i);
        return (rejoined_arg);
}

char    *handle_exit_status_case(t_shell *sh, char *subarg)
{
        char    *exit_status;
        char    *temp;

        exit_status = ft_itoa(sh->last_exit_status);
        if (subarg[1] != '\0')
        {
                temp = ft_strdup(&subarg[1]);
                free(subarg);
                subarg = ft_strjoin(exit_status, temp);
                free(temp);
        }
        else
        {
                free(subarg);
                subarg = ft_strdup(exit_status);
        }
        free(exit_status);
        return (subarg);
}


char	*handle_var_case(t_shell *sh, char **split_args, int i)
{
	char	**split_subargs;
	t_list	*set_var;

	if (ft_strrchr(split_args[i], CTRL_CHAR_STICKY_VAR) != NULL)
	{
		//interpret_sticky_var(sh, split_args, i);
		split_subargs = ft_split(split_args[i], CTRL_CHAR_STICKY_VAR);
		ft_show_strs(split_subargs, "[split_subargs]");;
		set_var = ft_getenv(split_subargs[0], &sh->this_env);
		free(split_subargs[0]);
        	if (set_var != NULL)
        	{
        		split_subargs[0] = ft_strdup(((char **)set_var->content)[1]);        
			printf("split_subargs[0]: %s\n", split_subargs[0]);
		}	
        	else
			split_subargs[0] = ft_strdup("");
        	free(split_args[i]);
        	split_args[i] = join_all_subargs(split_subargs, 'n');
        	free_args(split_subargs);
	}
	else
	{
		//interpret_var(sh, );
		printf("[nametoval - split_args[%d]] %s\n", i, split_args[i]);
		set_var = ft_getenv(split_args[i], &sh->this_env);
		free(split_args[i]);
		if (set_var != NULL)
			split_args[i] = ft_strdup(((char **)set_var->content)[1]);
		else
			split_args[i] = ft_strdup("");
	}
	return (split_args[i]);
}
