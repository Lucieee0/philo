/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lusimon <lusimon@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:41:07 by lusimon           #+#    #+#             */
/*   Updated: 2025/08/28 17:14:01 by lusimon          ###   ########.fr       */
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

typedef enum e_state
{
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
}	t_state;

// typedef struct s_timeval
// {
// 	time_t		tv_sec;       seconds since Epoch (Jan 1, 1970)
// 	suseconds_t	tv_usec;      microseconds (0–999999)
// }	t_timeval;

typedef struct s_philo	t_philo;

typedef struct s_table
{
	long			start_time;
	int				nbr_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nbr_of_meals;
	int				stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	t_philo			*philos;
	pthread_t		monitor_thread_id;
	pthread_mutex_t	died;
	int				philo_died;
	pthread_mutex_t	meal_reached;
	int				philo_finished_eating;
	int				id_dead_philo;
}	t_table;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	pthread_mutex_t	fork;
	struct s_philo	*next;
	pthread_mutex_t	count_meal;
	int				times_eaten;
	pthread_mutex_t	last_meal;
	long			last_meal_time;
	t_state			state;
	t_table			*table;
	unsigned long	reaction_time;
	int				nbr_of_meals;
}	t_philo;

//helper_functions
int		ft_atoi(const char *str);
long	ft_atol(const char *str);

//check_arg_initialize
int		check_arguments(char **argv);
int		initialize_table_threads(t_table *table);
void	initialize_table_struct(char **argv, t_table *table, t_philo *philos);

//main
void	start_eating_start_monitor(t_table *table);
int		join_threads(t_table *table, t_philo *philo);
int		main(int argc, char *argv[]);
void	free_philo_linked_list(t_philo *philo);

//philo_linked_list
int		initialize_philo_threads(t_philo *philo);
t_philo	*ft_lstnew(int id, t_table *table);
t_philo	*ft_lstlast(t_philo *philo);
void	ft_lstadd_back(t_philo **lst_philo, int id, t_table *table);
t_philo	*create_philo_circular_linked_list(t_table *table);

//philo_eat
int		check_own_death(t_philo *philo);
void	philo_eat(t_philo *philo);
void	even_philo_take_forks(t_philo *philo);
void	odd_philo_take_forks(t_philo *philo);

//philo_routine
void	philo_sleeps(t_philo *philo);
void	philo_thinks(t_philo *philo);
void	one_philo(t_philo *philo);
void	*philo_routine(void *data);

//monitor routine
int		check_stop_condition(t_philo *philo);
int		check_stop_routine(t_table *table);
void	*monitor_routine(void *data);

//time
long	get_time_ms(void);
long	get_timestamp(t_table *table);
long	get_elapsed_time_microseconds(struct timeval start, struct timeval end);
void	custom_usleep(long usec);

#endif
