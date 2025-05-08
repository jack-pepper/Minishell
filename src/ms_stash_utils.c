/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_stash_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:42:15 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/08 17:57:15 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int     stash_var(t_shell *sh)
{
        t_list  *stashed_var;
        t_list  *node;
        char    **split_str;
        size_t  i;
        size_t  nb_args;

        i = 0;
        node = NULL;
        split_str = NULL;
        nb_args = ft_strslen(sh->input_args);
        while (i < nb_args)
        {
                ft_flag_delim(sh->input_args[i], '=', CTRL_CHAR_EXTRA_DELIM, "f");
                split_str = ft_split(sh->input_args[i], '=');
                ft_unflag_delim(split_str[1], '=', CTRL_CHAR_EXTRA_DELIM);
                if (!split_str)
                        return (-1) ;
                if (!is_valid_env_name(split_str[0]))
                {
                        free_args(split_str);
                        return (2);
                }
                stashed_var = ft_getenv(split_str[0], &sh->env_stash);
                if (stashed_var != NULL)
                {
                        if (ft_update_env_value(stashed_var, split_str) != 0)
                        {
                                free_args(split_str);
                                return (-1) ;
                        }
                }
                else
                {
                        node = ft_lstnew((char **)split_str);
                        if (!node)
                        {
                                free_args(split_str);
                                return (-1);
                        }
                        ft_lstadd_back(&sh->env_stash, node);
                }
                i++;
        }
        return (0);
}

int     is_invalid_for_stash(char *arg)
{
        char    *equal_char;
        char    *var_name;
        size_t  len;

        equal_char = ft_strchr(arg, '=');
        len = (size_t)(equal_char - arg);
        var_name = ft_substr(arg, 0, len);
        if (!is_valid_env_name(var_name))
        {
                free(var_name);
                return (1);
        }
        free(var_name);
        return (0);
}

int     are_args_stashable(char **args)
{
        char    *invalid_cmd;
        int             i;

        i = 0;
        while (args[i] != NULL)
        {
                invalid_cmd = ft_strchr(args[i], '=');
                if ((invalid_cmd == NULL) || (is_invalid_for_stash(args[i]) != 0))
                {
                        printf("minishell: %s: command not found\n", args[i]);
                        return (127);
                }
                i++;
        }
        return (0);
}
