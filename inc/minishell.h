/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:35:22 by mmalie            #+#    #+#             */
/*   Updated: 2025/03/11 22:35:25 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Standard libraries */

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <stdlib.h>
# include <curses.h>
# include <term.h>

/* Custom libraries */
# include "../libft/libft.h"

/* Structures */


/* Prototypes */ // "ms_filename.c"

	// minishell.c

	// ms_prompt.c

	// ms_history.c

	// ms_environment.c

	// ms_signals.c
void	init_signals(void);
void	init_sigset(void);
void	init_sigaction(void);
void	signal_handler(int signum);

	// ms_launcher.c

	// ms_clean_memory.c

#endif
