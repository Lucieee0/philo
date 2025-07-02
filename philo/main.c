/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:39:17 by lusimon           #+#    #+#             */
/*   Updated: 2025/07/02 19:52:04 by lusimon          ###   ########.fr       */
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

int	main(int argc, char *argv[])
{
	t_table	*table;
	t_philo	*philo_list;
	int		i;

	i = 0;
	table = (t_table *)malloc(sizeof(t_table));
	if (!table)
		return (1);
	philo_list = NULL;
	if ((argc == 5 || argc == 6) && (check_arguments(argv) > 0))
	{
		initialize_table_struct(argv, table, philo_list);
		philo_list = create_philo_circular_linked_list(table);
		while (i < table->nbr_philo)
		{
			printf("%d\n", philo_list->id);
			philo_list = philo_list->next;
			i++;
		}
	}
	else
		printf("Invalid arguments\n");
	return (0);
}
