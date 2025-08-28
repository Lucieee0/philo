/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:36:54 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 13:49:21 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

long	get_timestamp(t_table *table)
{
	return (get_time_ms() - table->start_time);
}

void	custom_usleep(useconds_t microseconds)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed_us;
	long			target_us;

	target_us = microseconds;
	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed_us = (current.tv_sec - start.tv_sec) * 1000000L;
		elapsed_us += (current.tv_usec - start.tv_usec);
		if (elapsed_us >= target_us)
			break ;
	}
}
