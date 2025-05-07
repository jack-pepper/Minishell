/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libstdlib.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 11:25:28 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/07 14:23:32 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBSTDLIB_H
# define LIBSTDLIB_H

/* STANDARD LIBRARIES */
# include <stdlib.h>
# include <stddef.h>
# include <stdint.h> 
# include <unistd.h>
# include <fcntl.h>
# include <stdarg.h>
# include <stdio.h>

/* LIBSTDLIB */
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_memdel(void **ap);
int			ft_atoi(const char *nptr);
long		ft_atol(const char *nptr);
long long	ft_atoll(const char *nptr);

#endif
