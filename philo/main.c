/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:39:17 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 16:39:50 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	free_philo_linked_list(philo);
	free(table);
	return (0);
}

void	free_philo_linked_list(t_philo *philo)
{
	t_philo	*current;
	t_philo	*next;

	if (!philo)
		return ;
	current = philo;
	next = NULL;
	while (1)
	{
		next = current->next;
		pthread_mutex_destroy(&current->fork);
		pthread_mutex_destroy(&current->count_meal);
		pthread_mutex_destroy(&current->last_meal);
		free(current);
		if (next == philo)
			break ;
		current = next;
	}
}
