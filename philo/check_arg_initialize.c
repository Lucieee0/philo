/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg_initialize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 16:36:51 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 16:37:19 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monkey_testing(char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (argv[i])
	{
		while (argv[i][j])
		{
			if (argv[i][j] == '-' || argv[i][j] == '+')
				j++;
			if (!(argv[i][j] >= 48 && argv[i][j] <= 57))
				return (1);
			j++;
		}
		if (j > 9)
		{
			printf("here2\n");
			return (1);
		}
		j = 0;
		i++;
	}
	return (0);
}

int	check_arguments(char **argv)
{
	int		i;
	long	check;

	i = 1;
	check = 0;
	check = monkey_testing(argv);
	if (check == 1)
		return (0);
	check = ft_atol(argv[i]);
	if (check > 200)
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
	table->philo_died = 0;
	table->philo_finished_eating = 0;
	if (initialize_table_threads(table) == 1)
	{
		free(table);
		return ;
	}
	table->philos = philos;
}
