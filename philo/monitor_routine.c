/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 14:57:35 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/27 15:27:07 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_stop_condition(t_philo *philo)
{
	int	stop;

	stop = 0;
	pthread_mutex_lock(&philo->table->stop_lock);
	stop = philo->table->stop;
	pthread_mutex_unlock(&philo->table->stop_lock);
	return (stop);
}

void	*monitor_routine(void *data)
{
	t_table *table = (t_table *)data;
	t_philo *philo = table->philos;
	while (1)
	{
		pthread_mutex_lock(&philo->last_meal);
		if (get_timestamp(table) - philo->last_meal_time > table->time_to_die)
		{
			pthread_mutex_unlock(&philo->last_meal);
			break;
		}
		pthread_mutex_unlock(&philo->last_meal);
		if (max_meal(philo))
			break;
		usleep(1000);
		philo = philo->next;
	}
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
	pthread_mutex_lock(&table->print_lock);
	if (max_meal(philo))
		printf("%lu All philos have eaten %d meals\n", get_timestamp(table), table->nbr_of_meals);
	else
		printf("%lu %d died\n", get_timestamp(table), philo->id);
	pthread_mutex_unlock(&table->print_lock);
	return (NULL);
}