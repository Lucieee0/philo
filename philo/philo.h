/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:41:07 by lusimon           #+#    #+#             */
/*   Updated: 2025/07/02 19:41:23 by lusimon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

//Can I have a linked list?
//Where each philo is a node
//

typedef enum e_state
{
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
}	t_state;

typedef struct s_philo	t_philo;

typedef struct s_table
{
	int				nbr_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nbr_of_meals;
	int				stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	t_philo			*philos; // pointer to the first philo in the circular linked list
}	t_table;

//I want a circular linked list
typedef struct s_philo
{
	int				id; // philo nbr 1, philo nbr 2 ... 
	pthread_mutex_t	fork;
	struct s_philo	*next;
	int				times_eaten;
	long			last_meal_time;
	t_state			state;
	t_table			*table;
}	t_philo;

//helper_functions
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
//main
int		check_arguments(char **argv);
void	initialize_table_struct(char **argv, t_table *table, t_philo *philos);
int		main(int argc, char *argv[]);
//philo_linked_list
t_philo	*ft_lstnew(int id, t_table *table);
t_philo	*ft_lstlast(t_philo *philo);
void	ft_lstadd_back(t_philo **lst_philo, int id, t_table *table);
t_philo	*create_philo_circular_linked_list(t_table *table);

#endif
