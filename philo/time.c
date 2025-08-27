/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:36:54 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/27 18:00:07 by lusimon          ###   ########.fr       */
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

// void custom_usleep(useconds_t microseconds)
// {
//     struct timeval start, current;
//     long elapsed_us;
    
//     gettimeofday(&start, NULL);
    
//     while (1) {
//         gettimeofday(&current, NULL);
        
//         elapsed_us = (current.tv_sec - start.tv_sec) * 1000000;
//         elapsed_us += (current.tv_usec - start.tv_usec);
        
//         if (elapsed_us >= microseconds)
// 		{
//             break;
//         }
        
//         // Optional: tiny sleep to avoid CPU hogging for longer waits
//         if (microseconds - elapsed_us > 1000) { // if more than 1ms remaining
//             usleep(1); // minimal system call
//         }
//     }
// }

void custom_usleep(useconds_t microseconds)
{
    struct timeval start, current;
    long elapsed_us;
    long target_us = microseconds;
    
    gettimeofday(&start, NULL);
    
    // Pure busy wait for maximum precision
    while (1)
	{
        gettimeofday(&current, NULL);
        elapsed_us = (current.tv_sec - start.tv_sec) * 1000000L;
        elapsed_us += (current.tv_usec - start.tv_usec);
		if (elapsed_us >= target_us)
			break;
    }
}
//custom_usleep() uses busy waiting (active polling) to achieve higher timing precision, at the cost of 100% CPU usage while waiting.
//custom_usleep() = precise, but wastes CPU cycles.