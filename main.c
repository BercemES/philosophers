/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekinci- <bekinci-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:04:48 by bekinci-          #+#    #+#             */
/*   Updated: 2025/05/09 15:42:39 by bekinci-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_simulation(int ac, char **av)
{
	t_simulator	*sim;

	sim = malloc(sizeof(t_simulator));
	if (!sim)
		return (0);
	memset(sim, 0, sizeof(t_simulator));
	if (!ft_init(sim, ac, av))
	{
		free(sim);
		return (1);
	}
	if (!create_thread(sim))
	{
		free(sim);
		return (1);
	}
	ft_free(sim);
	return (0);
}

int	main(int ac, char **av)
{
	if (ft_atoi(av[1]) == 0)
	{
		printf(RED"Error\nPhilosopher number should'nt be 0\n"RESET);
		return (0);
	}
	if (ac != 5 && ac != 6)
	{
		printf(RED"Error\nIncorrect number of arguments.\n"RESET);
		return (0);
	}
	if (!digit_control(av))
	{
		printf(RED"Error\nArguments should be (positive) digit.\n"RESET);
		return (0);
	}
	start_simulation(ac, av);
	return (0);
}
