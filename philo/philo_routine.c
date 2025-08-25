/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/25 11:06:54 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_last_meal(t_philo *philo)
{
	//update philo last_meal timestamp
//lock the printf mutex
//print timestamp_in_ms X is eating
//unlock the printf mutex
//usleep(time_to_eat)
//unlock both fork mutexes when done
}

void	even_philo_eat(t_philo *philo)
{
//Even philo take left fork
//lock mutex of the left fork
//lock the printf mutex
//print timestamp_in_ms X has taken a fork
//unlock the printf mutex

//Even philo take right fork
//lock the printf mutex
//print timestamp_in_ms X has taken a fork
//unlock the printf mutex

	update_last_meal(philo);
}

void	odd_philo_eat(t_philo *philo)
{
//take right fork
//lock mutex of the right fork
//lock the printf mutex
//print timestamp_in_ms X has taken a fork
//unlock the printf mutex

//philo take left fork
//lock the printf mutex
//print timestamp_in_ms X has taken a fork
//unlock the printf mutex

	update_last_meal(philo);
}

void	philo_sleeps(t_philo *philo)
{
	//lock printf mutex
//print timestamp_in_ms X is sleeping
//unlock the printf mutex
//usleep(time_to_sleep)
}

void	philo_thinks(t_philo *philo)
{
	//lock the printf mutex
//print timestamp_in_ms X is thinking
//unlock the printf mutex
}



void *philo_routine(void *data)
{
	t_philo *philo = (t_philo *)data;
	int cafe = 0;

    // small desync trick: odd philosophers wait a bit
	// otherwise possiblity to have a philo reaction time
	//what is the best?
    if (philo->id % 2 == 1)
	{
		usleep(1000);
		odd_philo_eat(philo);
	}
	else
		even_philo_eat(philo);
        

    while (philo->table->stop == 0)
    {
		//printf("Thread ID: %lu\n", (unsigned long)(uintptr_t)philo->thread_id);
		printf("philo id: %d, timestamp: %ld, cafe: %d\n", philo->id, get_timestamp(philo->table), cafe);
		usleep(5000000); //5 seconds
		cafe += 1;
        // think(philo);
        // take_forks(philo);
        // eat(philo);
        // put_forks(philo);
        // sleep_philo(philo);
    }
	//printf("Philo thread ID: %lu number_coffee: %d\n", (unsigned long)(uintptr_t)philo->thread_id, cafe);
	return (NULL);
}


//Monitor thread routine
//checks if current_time - last meal > time_to_die
//timestamp_in_ms X died
//stops the simulation

void	*monitor_routine(void *data)
{
	t_table *table = (t_table *)data;
	t_philo *philo = table->philos;
	while (1)
	{
		if (get_timestamp(table) - philo->last_meal_time > table->time_to_die)
			break;
		philo = philo->next;
	}
	table->stop = 1;
	printf("%lu %lu died\n", get_timestamp(table), philo->id);
	return (NULL);
}

// void	*monitor_routine(void *data)
// {
// 	t_table *table = (t_table *)data;
// 	printf("Monitor thread ID: %lu\n", (unsigned long)(uintptr_t)table->monitor_thread_id);
// 	usleep(22000000); //22 seconds
// 	table->stop = 1;
// 	printf("Monitor thread ID: %lu Activated the stop\n", (unsigned long)(uintptr_t)table->monitor_thread_id);
// 	return (NULL);
// }
