/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/25 13:23:55 by lusimon          ###   ########.fr       */
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

void	even_philo_eat(t_philo *philo)
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
	philo->last_meal_time = get_timestamp(philo->table);
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is eating\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&philo->next->fork);
}

void	odd_philo_eat(t_philo *philo)
{
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
	philo->last_meal_time = get_timestamp(philo->table);
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is eating\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	usleep(philo->table->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->next->fork);
	pthread_mutex_unlock(&philo->fork);
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
	if (check_stop_condition(philo))
		return;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%lu %d is thinking\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	usleep(philo->table->time_to_sleep * 1000);
	//not correct
	//maybe try and use pthread_mutex_trylock?
}

void *philo_routine(void *data)
{
	t_philo *philo = (t_philo *)data;

	if (philo->table->nbr_philo == 1)
    {
        pthread_mutex_lock(&philo->fork);
        pthread_mutex_lock(&philo->table->print_lock);
        printf("%lu %d has taken a fork\n", get_timestamp(philo->table), philo->id);
        pthread_mutex_unlock(&philo->table->print_lock);
        
        // Wait for death - can't get second fork
        while (!check_stop_condition(philo))
            usleep(1000);
            
        pthread_mutex_unlock(&philo->fork);
        return (NULL);
    }
    while (!check_stop_condition(philo))
    {
		if (philo->id % 2 == 1)
		{
			usleep(1000);
			odd_philo_eat(philo);
		}
		else
			even_philo_eat(philo);
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

void	*monitor_routine(void *data)
{
	t_table *table = (t_table *)data;
	t_philo *philo = table->philos;
	while (1)
	{
		if (get_timestamp(table) - philo->last_meal_time > table->time_to_die)
			break;
		usleep(1000);
		philo = philo->next;
	}
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
	pthread_mutex_lock(&table->print_lock);
	printf("%lu %d died\n", get_timestamp(table), philo->id);
	pthread_mutex_unlock(&table->print_lock);
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
