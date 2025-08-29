/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 14:57:35 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 13:25:58 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_own_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal);
	if (get_timestamp(philo->table) - philo->last_meal_time
		> philo->table->time_to_die)
	{
		pthread_mutex_lock(&philo->table->died);
		philo->table->philo_died += 1;
		philo->table->id_dead_philo = philo->id;
		pthread_mutex_unlock(&philo->table->died);
		pthread_mutex_unlock(&philo->last_meal);
		return (1);
	}
	pthread_mutex_unlock(&philo->last_meal);
	return (0);
}

int	check_stop_condition(t_philo *philo)
{
	if (check_own_death(philo))
		return (1);
	pthread_mutex_lock(&philo->table->meal_reached);
	if (philo->table->philo_finished_eating >= philo->table->nbr_philo)
	{
		pthread_mutex_unlock(&philo->table->meal_reached);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->meal_reached);
	pthread_mutex_lock(&philo->table->died);
	if (philo->table->philo_died > 0)
	{
		pthread_mutex_unlock(&philo->table->died);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->died);
	return (0);
}

void	*monitor_routine(void *data)
{
	t_table	*table;

	table = (t_table *)data;
	while (1)
	{
		if (check_stop_condition(table->philos) == 1)
			break ;
		custom_usleep(1000, table->philos);
	}
	pthread_mutex_lock(&table->print_lock);
	if (table->philo_died == 0)
		printf("%lu All philos have eaten %d meals\n",
			get_timestamp(table), table->nbr_of_meals);
	else
		printf("%lu %d died\n", get_timestamp(table), table->id_dead_philo);
	pthread_mutex_unlock(&table->print_lock);
	return (NULL);
}
