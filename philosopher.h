/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:58:38 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 12:58:38 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <sys/time.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				philo_take;
}	t_fork;

typedef struct s_philo
{
	int						left_fork;
	int						right_fork;
	int						eat_count;
	long int				lastmeal;
	int						id;
	struct s_simulation		*simulation;
	pthread_t				thread;
}	t_philo;

typedef struct s_simulation
{
	long int		start_time;
	int				num_philosophers;
	int				time_to_sleep;
	int				end;
	int				died;
	int				meals_required;
	int				time_to_die;
	int				time_to_eat;
	pthread_mutex_t	eat;
	pthread_mutex_t	print;
	pthread_mutex_t	m_end;
	pthread_mutex_t	m_died;
	pthread_mutex_t	meal;
	t_philo			*philo;
	t_fork			*fork;
}	t_simulation;


// Utility Functions
int			ft_atoi(const char *str);
long int	current_timestamp(void);
void		accurate_sleep(long int time_in_ms, t_simulation *simulation);
void		log_philosopher_action(int id, t_simulation *simulation, char *action);

// Initialization Functions
int			initialize_philosophers(t_simulation *simulation);
int			init_mutexes(t_simulation *simulation);
int	create_philosopher_thread(t_simulation *simulation, int i, void *routine);

// Cleanup Functions
void		destroy_mutexes(t_simulation *simulation);
void		terminate_simulation(t_simulation *simulation);

// Monitoring Functions
int			has_any_philosopher_died(t_simulation *simulation);
void	handle_philosopher_death(t_simulation *simulation, int i);
int			has_simulation_end(t_simulation *simulation);

// Philosopher Routines
void		*single_philosopher_routine(void *philo_void);
void		*philosopher_thread(void *philo_void);

// Simulation Control
void		monitor_death(t_simulation *simulation);
void		launch_simulation(t_simulation *simulation);
void		terminate_simulation(t_simulation *simulation);

#endif
