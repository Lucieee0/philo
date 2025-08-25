/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:39:17 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/25 17:48:05 by lusimon          ###   ########.fr       */
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
	if (pthread_mutex_init(&table->stop_lock, NULL) != 0)
	{
		free(table);
		return ;
	}
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
	{
		free(table);
		return ;
	}
	table->philos = philos;
}

void	start_eating_start_monitor(t_table *table)
{
	int i = 0;
	t_philo *philo = table->philos;

	while (i < table->nbr_philo)
    {
		//Pass the function pointer without calling it, and pass philo as the argument instead:
        if (pthread_create(&philo->thread_id, NULL, philo_routine, philo) != 0)
			printf("failde\n");
        philo = philo->next;
		i++;
    }
	pthread_create(&table->monitor_thread_id, NULL, monitor_routine, table);
}

int	main(int argc, char *argv[])
{
	t_table	*table;
	t_philo	*philo;
	int		i;

	i = 0;
	table = (t_table *)malloc(sizeof(t_table));
	if (!table)
		return (1);
	philo = NULL;
	if ((argc == 5 || argc == 6) && (check_arguments(argv) > 0))
	{
		initialize_table_struct(argv, table, philo);
		philo = create_philo_circular_linked_list(table);
		start_eating_start_monitor(table);
		if (pthread_join(table->monitor_thread_id, NULL) != 0)
		{
			printf("capturing the monitor thread failed\n");
			return (1);
		}
		t_philo *cur = philo;
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
	}
	else
		printf("Invalid arguments\n");
	return (0);
}

//Parse arguments and initalize table
//Create circular linked list of philosophers
//Create one thread per philo (start_eating function)
//The pthread_create(&thread_id, NULL, philo_routine, philo);
//will call the action that the threads will execute -> the philo routine
//In the same time we will create the thread for the monitor
//The monitor will check the deads or if the number of meals is reached
//When the monitor detects something wrong, he will send a stop message to each philo
//So each philos when then receive this message, they will exit their routine loop
//During this time in the main
//pthread_join doesn’t force a thread to stop.
//It just says: “I will wait here until this thread finishes.”
//so in the main we wait for each finised thread of each philo
//then we wait for the finised thread of the monitor
