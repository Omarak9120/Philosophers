/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 12:45:28 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 15:08:02 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	handle_philosopher_death(t_simulation *simulation, int i)
{
	pthread_mutex_lock(&simulation->m_died);
	simulation->died = 1;
	pthread_mutex_unlock(&simulation->m_died);
	pthread_mutex_lock(&simulation->print);
	printf(
		"%ld %d %s\n",
		current_timestamp() - simulation->start_time,
		simulation->philo[i].id,
		"died."
		);
	pthread_mutex_unlock(&simulation->print);
}

void	destroy_mutexes(t_simulation *simulation)
{
	int	i;

	i = -1;
	while (++i < simulation->num_philosophers)
	{
		pthread_mutex_destroy(&simulation->fork[i].mutex);
	}
	pthread_mutex_destroy(&simulation->meal);
	pthread_mutex_destroy(&simulation->m_end);
	pthread_mutex_destroy(&simulation->print);
	pthread_mutex_destroy(&simulation->m_died);
	pthread_mutex_destroy(&simulation->eat);
	free(simulation->philo);
	free(simulation->fork);
}

void	terminate_simulation(t_simulation *simulation)
{
	int	i;

	i = -1;
	while (++i < simulation->num_philosophers)
	{
		pthread_join(simulation->philo[i].thread, NULL);
	}
	destroy_mutexes(simulation);
}
