/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_commands_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:25:28 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/17 00:17:15 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Could be initialized like this:
/*
t_command commands[] = {
                { "pwd", cmd_pwd, "Print name of current/working directory" },
                { "cd", cmd_cd, "Change the working directory" }, 
                { "echo", cmd_echo, "Display a line of text" }, 
                { "exit", cmd_exit, "Cause the shell to exit" },
                { "export", cmd_export, "Set the export attribute for variables" },
                { "unset", cmd_unset, "Unset values and attributes of variables and functions" },
                { "env", cmd_env, "Display the env variables" },
                { "help", cmd_help, "Display this text" },
                { (char *)NULL, (Function *)NULL, (char *)NULL } /// should allow to loop through the cmd list
        };
*/

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

        sh->cmds[3] = register_cmd("env", cmd_env, "Display the env variables");
        if (sh->cmds[3] == NULL)
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


