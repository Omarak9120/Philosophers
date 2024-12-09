/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oabdelka <oabdelka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 14:17:44 by oabdelka          #+#    #+#             */
/*   Updated: 2024/12/09 14:17:44 by oabdelka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	validate_argument(int value, int index)
{
	if (index == 1 && value > 200)
		return (printf("max  %d\n", 200), 0);
	if (index == 5)
	{
		if (value == -1)
			return (1);
		if (value < 1 && value != -1)
			return (printf("over 1 meal\n"), 0);
	}
	return (1);
}

/**
(sim) Pointer to the simulation structure.
 */
static int	parse_arguments(int ac, t_simulation *simulation, char **av)
{
	simulation->num_philosophers = ft_atoi(av[1]);
	simulation->time_to_die = ft_atoi(av[2]);
	simulation->time_to_eat = ft_atoi(av[3]);
	simulation->time_to_sleep = ft_atoi(av[4]);
	simulation->end = 0;
	simulation->died = 0;
	simulation->meals_required = -1;
	if (ac == 6)
		simulation->meals_required = ft_atoi(av[5]);
	return (1);
}

static int	validate_inputs(int ac, char **av)
{
	int	i;
	int	value;

	i = 1;
	while (i < ac)
	{
		value = ft_atoi(av[i]);
		if (validate_argument(value, i))
		{
			if (value <= 0 && !(i == 5 && value == -1))
			{
				printf("incorrect value: %s !\n", av[i]);
				return (0);
			}
		}
		else
			return (0);
		i++;
	}
	return (1);
}

static int	validate_number_of_arguments(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die "
			"time_to_eat time_to_sleep [meals_required]\n");
		return (0);
	}
	while (i < ac)
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] > '9' || av[i][j] < '0')
				return (printf("Invalid input: %s\n", av[i]), 0);
			j++;
		}
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_simulation	simulation;

	if (!validate_number_of_arguments(ac, av))
		return (1);
	if (!validate_inputs(ac, av))
		return (1);
	if (!parse_arguments(ac, &simulation, av))
		return (1);
	if (!initialize_philosophers(&simulation))
	{
		destroy_mutexes(&simulation);
		return (1);
	}
	if (!init_mutexes(&simulation))
	{
		destroy_mutexes(&simulation);
		return (1);
	}
	launch_simulation(&simulation);
	terminate_simulation(&simulation);
	return (0);
}
