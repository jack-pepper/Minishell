/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_commands_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:25:28 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/25 19:08:33 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int     init_cmds(t_shell *sh)
{
        sh->cmds = malloc(sizeof(t_command) * NB_CMDS);
        if (!sh)
                return (-1);

        // We can use open, read and write, so we may want to loop this logic from a file
        // containing the names and doc

        // exit
        sh->cmds[0] = register_cmd("exit", cmd_exit, "Cause the shell to exit");
        if (sh->cmds[0] == NULL)
                return (-1);

        sh->cmds[1] = register_cmd("pwd", cmd_pwd, "Print name of current/working directory");
        if (sh->cmds[1] == NULL)
                return (-1);

        sh->cmds[2] = register_cmd("cd", cmd_cd, "Change the working directory");
        if (sh->cmds[2] == NULL)
                return (-1);


        return (0);
}

t_command       *register_cmd(char *name, void *func, char *doc)
{
        t_command       *cmd;
        size_t          name_len;
        size_t          doc_len;

        cmd = malloc(sizeof(t_command));
        if (!cmd)
                return (NULL);


        name_len = ft_strlen(name);
        doc_len = ft_strlen(doc);

        cmd->name = malloc(sizeof(char) * name_len);
        if (!cmd->name)
                return (NULL);

        cmd->doc = malloc(sizeof(char) * doc_len);
        if (!cmd->doc)
                return (NULL);

        ft_strlcpy(cmd->name, name, name_len);
        cmd->func = func;
        ft_strlcpy(cmd->doc, doc, doc_len);

        return (cmd);
}


