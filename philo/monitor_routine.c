/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 14:57:35 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/27 16:36:57 by lusimon          ###   ########.fr       */
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
	while (1)
	{
		pthread_mutex_lock(&table->meal_reached);
		if (table->philo_finished_eating >= table->nbr_philo)
		{
			pthread_mutex_unlock(&table->meal_reached);
			break;
		}
		pthread_mutex_unlock(&table->meal_reached);
		pthread_mutex_lock(&table->died);
		if (table->philo_died > 0)
		{
			pthread_mutex_unlock(&table->died);
			break;
		}
		pthread_mutex_unlock(&table->died);
		usleep(1000);
	}
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
	pthread_mutex_lock(&table->print_lock);
	if (table->philo_died == 0)
		printf("%lu All philos have eaten %d meals\n", get_timestamp(table), table->nbr_of_meals);
	else
		printf("%lu %d died\n", get_timestamp(table), table->id_dead_philo);
	pthread_mutex_unlock(&table->print_lock);
	return (NULL);
}