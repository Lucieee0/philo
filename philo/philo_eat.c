/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:38:35 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 16:59:14 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	even_philo_take_forks(t_philo *philo)
{
	if (check_stop_condition(philo) == 1)
		return ;
	while (pthread_mutex_trylock(&philo->fork) != 0)
    {
        if (check_stop_condition(philo) == 1)
            return ;
        custom_usleep(1000, philo); // Small delay before retry
    }
	//pthread_mutex_lock(&philo->fork);
	if (check_stop_condition(philo) == 1)
	{
		pthread_mutex_unlock(&philo->fork);
		return ;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	while (pthread_mutex_trylock(&philo->next->fork) != 0)
    {
        if (check_stop_condition(philo) == 1)
            return ;
        custom_usleep(1000, philo); // Small delay before retry
    }
	//pthread_mutex_lock(&philo->next->fork);
	if (check_stop_condition(philo) == 1)
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
		return ;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	philo_eat(philo);
}

void	odd_philo_take_forks(t_philo *philo)
{
	custom_usleep(1000, philo);
	if (check_stop_condition(philo) == 1)
		return ;
	while (pthread_mutex_trylock(&philo->next->fork) != 0)
    {
        if (check_stop_condition(philo) == 1)
            return ;
        custom_usleep(1000, philo); // Small delay before retry
    }
	//pthread_mutex_lock(&philo->next->fork);
	if (check_stop_condition(philo) == 1)
	{
		pthread_mutex_unlock(&philo->next->fork);
		return ;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	while (pthread_mutex_trylock(&philo->fork) != 0)
    {
        if (check_stop_condition(philo) == 1)
            return ;
        custom_usleep(1000, philo); // Small delay before retry
    }
	//pthread_mutex_lock(&philo->fork);
	if (check_stop_condition(philo) == 1)
	{
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
		return ;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	philo_eat(philo);
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal);
	philo->last_meal_time = get_timestamp(philo->table);
	pthread_mutex_unlock(&philo->last_meal);
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is eating\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->count_meal);
	philo->times_eaten += 1;
	if (philo->times_eaten == philo->nbr_of_meals)
	{
		pthread_mutex_lock(&philo->table->meal_reached);
		philo->table->philo_finished_eating += 1;
		pthread_mutex_unlock(&philo->table->meal_reached);
	}
	pthread_mutex_unlock(&philo->count_meal);
	custom_usleep(philo->table->time_to_eat * 1000, philo);
	pthread_mutex_unlock(&philo->next->fork);
	pthread_mutex_unlock(&philo->fork);
}
