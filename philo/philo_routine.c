/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 16:38:56 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_sleeps(t_philo *philo)
{
	if (check_stop_condition(philo))
		return ;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is sleeping\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	custom_usleep(philo->table->time_to_sleep * 1000);
}

void	philo_thinks(t_philo *philo)
{
	unsigned long	think_time;

	think_time = (philo->table->time_to_die
			- philo->table->time_to_eat - philo->table->time_to_sleep) / 2;
	if (check_stop_condition(philo))
		return ;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is thinking\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	if (think_time > 0)
		custom_usleep(think_time * 1000);
}

void	one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	while (!check_stop_condition(philo))
	{
		if (check_own_death(philo))
			break ;
		custom_usleep(100);
	}
	pthread_mutex_unlock(&philo->fork);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->table->nbr_philo == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	while (!check_stop_condition(philo) && !check_own_death(philo))
	{
		if (philo->id % 2 == 1)
			odd_philo_take_forks(philo);
		else
			even_philo_take_forks(philo);
		if (check_stop_condition(philo))
			break ;
		philo_sleeps(philo);
		if (check_stop_condition(philo))
			break ;
		philo_thinks(philo);
	}
	return (NULL);
}
