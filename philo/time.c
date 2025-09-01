/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:36:54 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 16:18:48 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	get_timestamp(t_table *table)
{
	return (get_time_ms() - table->start_time);
}

long	get_elapsed_time_microseconds(struct timeval start, struct timeval end)
{
	return ((end.tv_sec - start.tv_sec) * 1000000L
		+ (end.tv_usec - start.tv_usec));
}

void	custom_usleep(long usec, t_philo *philo)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed;
	long			remaining;

	elapsed = 0;
	gettimeofday(&start, NULL);
	while (elapsed < usec)
	{
		if (check_stop_condition(philo) == 1)
			break ;
		gettimeofday(&current, NULL);
		elapsed = get_elapsed_time_microseconds(start, current);
		remaining = usec - elapsed;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
			usleep(50);
	}
}

// A thread cannot run itself - it needs the CPU to execute instructions.

// After the sleep of one thread (one philo),
// if the CPU is currently busy running other philosophers or system processes,
// the scheduler (traffic controller of the CPU)
// may delay giving the CPU back to the woken-up philosopher.
// A CPU has a limited number of threads it can run at the same time,
// so the scheduler decides which thread runs next.

// With the custom_usleep() function,
// the thread reduces the time it spends completely asleep.
// This way, it re-enters the ready state more often,
// which lowers the risk of oversleeping when the target time is reached.

// The custom_usleep() function makes the thread
// always sleep for half of the remaining time,
// so it wakes up more frequently and finishes with a short busy-wait.
// This gives a much more accurate total sleep duration.