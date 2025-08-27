/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/27 15:40:43 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int max_meal(t_philo *philo)
{
	int i;
	int	nbr_philo;
	
	i = 0;
	nbr_philo = philo->table->nbr_philo;
	if (philo->table->nbr_of_meals == 0)
		return (0);
	while (i < nbr_philo)
	{
		pthread_mutex_lock(&philo->count_meal);
		if (philo->times_eaten < philo->table->nbr_of_meals)
		{
			pthread_mutex_unlock(&philo->count_meal);
			return (0); // someone hasn’t finished yet
		}
		pthread_mutex_unlock(&philo->count_meal);
		philo = philo->next;
		i++;
	}
	return (1); // all philosophers reached nbr_of_meals
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
	pthread_mutex_unlock(&philo->count_meal);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->next->fork);
	pthread_mutex_unlock(&philo->fork);
}

void	even_philo_take_forks(t_philo *philo)
{
	if (check_stop_condition(philo))
		return;
	pthread_mutex_lock(&philo->fork);
	if (check_stop_condition(philo))
	{
		pthread_mutex_unlock(&philo->fork);
		return;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->next->fork);
	if (check_stop_condition(philo)) 
	{
	    pthread_mutex_unlock(&philo->fork);
	    pthread_mutex_unlock(&philo->next->fork);
	    return;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	philo_eat(philo);
}

void	odd_philo_take_forks(t_philo *philo)
{
	usleep(1000);
	if (check_stop_condition(philo))
		return;
	pthread_mutex_lock(&philo->next->fork);
	if (check_stop_condition(philo))
	{
		pthread_mutex_unlock(&philo->next->fork);
		return;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->fork);
	if (check_stop_condition(philo)) 
	{
	    pthread_mutex_unlock(&philo->fork);
	    pthread_mutex_unlock(&philo->next->fork);
	    return;
	}
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	philo_eat(philo);
}

void	philo_sleeps(t_philo *philo)
{
	if (check_stop_condition(philo))
		return;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is sleeping\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	usleep(philo->table->time_to_sleep * 1000);
}

void	philo_thinks(t_philo *philo)
{
	unsigned long	think_time;

	think_time = (philo->table->time_to_die - philo->table->time_to_eat - philo->table->time_to_sleep) / 2;
	if (check_stop_condition(philo))
		return;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is thinking\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	if (think_time > 0)
		usleep(think_time * 1000);
	//not correct
	//maybe try and use pthread_mutex_trylock?
}

void	one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	while (!check_stop_condition(philo))
		usleep(1000);
	pthread_mutex_unlock(&philo->fork);
}

void *philo_routine(void *data)
{
	t_philo *philo;
	
	philo = (t_philo *)data;
	if (philo->table->nbr_philo == 1)
    {
		one_philo(philo);
		return (NULL);
    }
    while (!check_stop_condition(philo))
    {
		if (philo->id % 2 == 1)
			odd_philo_take_forks(philo);
		else
			even_philo_take_forks(philo);
		if (check_stop_condition(philo))
		    break;
		philo_sleeps(philo);
		if (check_stop_condition(philo))
			break;
		philo_thinks(philo);
    }
	return (NULL);
}


//Monitor thread routine
//checks if current_time - last meal > time_to_die
//timestamp_in_ms X died
//stops the simulation
