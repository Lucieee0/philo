/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/19 17:35:37 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_routine(void *data)
{
	t_philo *philo = (t_philo *)data;
	int cafe = 0;

    // small desync trick: odd philosophers wait a bit
	// otherwise possiblity to have a philo reaction time
	//what is the best?
    if (philo->id % 2 == 1)
        usleep(100);

    while (philo->table->stop == 0)
    {
		cafe += 1;
		printf("Thread ID: %lu\n", (unsigned long)(uintptr_t)philo->thread_id);
		usleep(5000000);
        // think(philo);
        // take_forks(philo);
        // eat(philo);
        // put_forks(philo);
        // sleep_philo(philo);
    }
	printf("Philo thread ID: %lu number_coffee: %d\n", (unsigned long)(uintptr_t)philo->thread_id, cafe);
	return (NULL);
}

void	*monitor_routine(void *data)
{
	t_table *table = (t_table *)data;
	printf("Monitor thread ID: %lu\n", (unsigned long)(uintptr_t)table->monitor_thread_id);
	usleep(22000000);
	table->stop = 1;
	printf("Monitor thread ID: %lu Activated the stop\n", (unsigned long)(uintptr_t)table->monitor_thread_id);
	return (NULL);
}
