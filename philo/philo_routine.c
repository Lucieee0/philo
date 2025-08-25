/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:47:22 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/25 18:03:41 by lusimon          ###   ########.fr       */
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
	usleep(80);
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

// void	*monitor_routine(void *data)
// {
// 	t_table *table = (t_table *)data;
// 	printf("Monitor thread ID: %lu\n", (unsigned long)(uintptr_t)table->monitor_thread_id);
// 	usleep(22000000); //22 seconds
// 	table->stop = 1;
// 	printf("Monitor thread ID: %lu Activated the stop\n", (unsigned long)(uintptr_t)table->monitor_thread_id);
// 	return (NULL);
// }
