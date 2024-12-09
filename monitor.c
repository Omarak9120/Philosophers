/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:35:44 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 13:35:44 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	has_any_philosopher_died(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->m_died);
	if (simulation->died)
	{
		pthread_mutex_unlock(&simulation->m_died);
		return (1);
	}
	pthread_mutex_unlock(&simulation->m_died);
	return (0);
}

int	has_simulation_end(t_simulation *sim)
{
	pthread_mutex_lock(&sim->m_end);
	if (sim->end)
	{
		pthread_mutex_unlock(&sim->m_end);
		return (1);
	}
	pthread_mutex_unlock(&sim->m_end);
	return (0);
}

static int	have_all_philosophers_eaten(t_philo *philo)
{
	pthread_mutex_lock(&philo->simulation->eat);
	if (philo->eat_count >= philo->simulation->meals_required)
	{
		pthread_mutex_unlock(&philo->simulation->eat);
		return (1);
	}
	pthread_mutex_unlock(&philo->simulation->eat);
	return (0);
}

static void	check_end_condition(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (simulation->meals_required != -1 && i < simulation->num_philosophers
		&& have_all_philosophers_eaten(&simulation->philo[i]))
		i++;
	if (i == simulation->num_philosophers)
	{
		pthread_mutex_lock(&simulation->m_end);
		simulation->end = 1;
		pthread_mutex_unlock(&simulation->m_end);
		pthread_mutex_lock(&simulation->print);
		printf("number of meals eaten: %d\n",
			simulation->meals_required);
		pthread_mutex_unlock(&simulation->print);
	}
}

void	monitor_death(t_simulation *simulation)
{
	int	i;

	while (!has_simulation_end(simulation))
	{
		i = -1;
		while (
			++i < simulation->num_philosophers
			&& !has_any_philosopher_died(simulation)
		)
		{
			pthread_mutex_lock(&simulation->meal);
			if (
				(current_timestamp() - simulation->philo[i].lastmeal)
				> simulation->time_to_die
			)
			{
				handle_philosopher_death(simulation, i);
			}
			pthread_mutex_unlock(&simulation->meal);
			usleep(200);
		}
		if (has_any_philosopher_died(simulation))
			break ;
		check_end_condition(simulation);
	}
}
