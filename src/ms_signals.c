/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:49:10 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/06 11:38:47 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/* Define which signals are caught and how they are handled.
 * 
 * - SIGINT (ctrl + c): display a new prompt on new line
 * - [TODO] Not sure about SICQUIT. ctrl + \ may need to be handled in a different way
 * - NB: ctrl + d sends NO signal, but an EOF to the main function reading from stdin
 *
 */

void	init_signals(void)
{
	init_sigset();
	init_sigaction();
}

void	init_sigset(void) // define which signals are handled
{
	sigset_t	sigset;

	sigemptyset(&sigset); // necessary for init
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGQUIT);
}

void	init_sigaction(void) // define how signals are handled
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = signal_handler;
	act.sa_flags = 0;
}

void	signal_handler(int signum)
{
	if (signum == SIGINT) // ctrl+c: should display a new prompt on new line instead of quitting
	{
		// should free memory as well
	}
	else if (signum == SIGQUIT) // ctrl+\: should do nothing instead of interrupting
	{
		//
	}
}
