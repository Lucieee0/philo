/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_linked_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:42:35 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/27 16:21:07 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//I want to create here a circular linked list of philos
//the program take as argument the "number_of_philosophers"
//one node per philo
//one philo + one fork

t_philo	*ft_lstnew(int id, t_table *table)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->next = NULL;
	philo->times_eaten = 0;
	philo->last_meal_time = 0;
	philo->state = THINKING;
	philo->table = table;
	philo->nbr_of_meals = philo->table->nbr_of_meals;
	philo->reaction_time = (id % 2) * 1000;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	if (pthread_mutex_init(&philo->count_meal, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	if (pthread_mutex_init(&philo->last_meal, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	return (philo);
}

t_philo	*ft_lstlast(t_philo *philo)
{
	if (!philo)
		return (NULL);
	while (philo->next)
	{
		philo = philo->next;
	}
	return (philo);
}

void	ft_lstadd_back(t_philo **lst_philo, int id, t_table *table)
{
	t_philo	*last;
	t_philo	*new_philo;

	if (!lst_philo)
		return ;
	new_philo = ft_lstnew(id, table);
	if (!new_philo)
		return ;
	last = ft_lstlast(*lst_philo);
	if (!last)
	{
		*lst_philo = new_philo;
		return ;
	}
	last->next = new_philo;
}

t_philo	*create_philo_circular_linked_list(t_table *table)
{
	int		i;
	t_philo	*lst_philo;
	t_philo	*last;

	lst_philo = NULL;
	i = 0;
	while (i < table->nbr_philo)
	{
		ft_lstadd_back(&lst_philo, i + 1, table);
		i++;
	}
	//make the last node point to the first one
	//circular linked list
	if (lst_philo)
	{
		last = ft_lstlast(lst_philo);
		last->next = lst_philo;
	}
	table->philos = lst_philo;
	return (lst_philo);
}

void free_philo_linked_list(t_philo *philo)
{
    if (!philo)
        return;

    t_philo *current = philo;
    t_philo *next = NULL;

    while (1)
    {
        next = current->next;

        pthread_mutex_destroy(&current->fork);
        pthread_mutex_destroy(&current->count_meal);
        pthread_mutex_destroy(&current->last_meal);

        free(current);

        if (next == philo) // we completed a full circle
            break;

        current = next;
    }
}
