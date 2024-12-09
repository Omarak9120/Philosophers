/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:34:04 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 14:34:04 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	init_all_mutexes(t_simulation *simulation)
{
	if (pthread_mutex_init(&simulation->meal, NULL))
		return (printf("meal Error\n"), 0);
	if (pthread_mutex_init(&simulation->m_end, NULL))
		return (printf("end Error\n"), 0);
	if (pthread_mutex_init(&simulation->print, NULL))
		return (printf("print Error\n"), 0);
	if (pthread_mutex_init(&simulation->m_died, NULL))
		return (printf("died Error\n"), 0);
	if (pthread_mutex_init(&simulation->eat, NULL))
		return (printf("eat Error\n"), 0);
	return (1);
}

int	init_mutexes(t_simulation *simulation)
{
	int	i;

	i = -1;
	simulation->fork = malloc(sizeof(t_fork) * simulation->num_philosophers);
	if (!simulation->fork)
		return (printf("error\n"), 0);
	while (++i < simulation->num_philosophers)
	{
		if (pthread_mutex_init(&simulation->fork[i].mutex, NULL))
			return (printf("fork error!\n"), 0);
		simulation->fork[i].philo_take = 0;
	}
	return (init_all_mutexes(simulation));
}

void	launch_simulation(t_simulation *simulation)
{
	int		i;
	void	*routine;

	i = -1;
	if (simulation->num_philosophers == 1)
		routine = single_philosopher_routine;
	else
		routine = philosopher_thread;
	simulation->start_time = current_timestamp();
	while (++i < simulation->num_philosophers)
	{
		simulation->philo[i].lastmeal = simulation->start_time;
		if (pthread_create(
				&simulation->philo[i].thread,
				NULL,
				routine,
				&simulation->philo[i]
			) != 0
		)
		{
			printf("thread creating error %d\n", simulation->philo[i].id);
		}
	}
	if (routine == philosopher_thread)
		monitor_death(simulation);
}

int	initialize_philosophers(t_simulation *simulation)
{
	int	i;

	i = -1;
	simulation->philo = malloc(sizeof(t_philo) * simulation->num_philosophers);
	if (simulation->philo == NULL)
	{
		printf("memory allocating error!\n");
		return (0);
	}
	while (++i < simulation->num_philosophers)
	{
		simulation->philo[i].id = i + 1;
		simulation->philo[i].lastmeal = 0;
		simulation->philo[i].right_fork = i;
		simulation->philo[i].left_fork = (i + 1) % simulation->num_philosophers;
		simulation->philo[i].eat_count = 0;
		simulation->philo[i].simulation = simulation;
	}
	return (1);
}
