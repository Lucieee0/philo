/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:39:17 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 13:34:08 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arguments(char **argv)
{
	int		i;
	long	check;

	i = 1;
	check = 0;
	check = ft_atol(argv[i]);
	if (check > 500)
		return (0);
	while (argv[i])
	{
		check = ft_atol(argv[i]);
		if (check < 0)
			return (0);
		check = 0;
		i++;
	}
	return (1);
}

int	initialize_table_threads(t_table *table)
{
	if (pthread_mutex_init(&table->stop_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->died, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->meal_reached, NULL) != 0)
		return (1);
	return (0);
}

void	initialize_table_struct(char **argv, t_table *table, t_philo *philos)
{
	table->start_time = get_time_ms();
	table->nbr_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (argv[5])
		table->nbr_of_meals = ft_atoi(argv[5]);
	else
		table->nbr_of_meals = 0;
	table->stop = 0;
	table->philo_died = 0;
	table->philo_finished_eating = 0;
	if (initialize_table_threads(table) == 1)
	{
		free(table);
		return ;
	}
	table->philos = philos;
}

void	start_eating_start_monitor(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = table->philos;
	while (i < table->nbr_philo)
	{
		if (pthread_create(&philo->thread_id, NULL, philo_routine, philo) != 0)
			printf("failde\n");
		philo = philo->next;
		i++;
	}
	pthread_create(&table->monitor_thread_id, NULL, monitor_routine, table);
}

int	join_threads(t_table *table, t_philo *philo)
{
	int		i;
	t_philo	*cur;

	i = 0;
	if (pthread_join(table->monitor_thread_id, NULL) != 0)
	{
		printf("capturing the monitor thread failed\n");
		return (1);
	}
	cur = philo;
	while (i < table->nbr_philo)
	{
		if (pthread_join(cur->thread_id, NULL) != 0)
		{
			printf("capturing all the philo threads failed\n");
			return (1);
		}
		cur = cur->next;
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_table	*table;
	t_philo	*philo;

	table = (t_table *)malloc(sizeof(t_table));
	if (!table)
		return (1);
	philo = NULL;
	if ((argc == 5 || argc == 6) && (check_arguments(argv) > 0))
	{
		initialize_table_struct(argv, table, philo);
		philo = create_philo_circular_linked_list(table);
		start_eating_start_monitor(table);
		if (join_threads(table, philo) == 1)
			return (1);
	}
	else
		printf("Invalid arguments\n");
	free_philo_linked_list(philo);
	free(table);
	return (0);
}
