/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekinci- <bekinci-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:04:41 by bekinci-          #+#    #+#             */
/*   Updated: 2025/05/09 15:40:30 by bekinci-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_thread(t_simulator *sim)
{
	int	i;

	if (pthread_create(&sim->observer, NULL, &check_philo, sim) != 0)
	{
		perror("pthread_create failed for observer thread");
		return (0);
	}
	i = -1;
	while (++i < sim->num_philos)
	{
		if (pthread_create(&sim->philos[i].thread_id,
				NULL, &routine, &sim->philos[i]) != 0)
		{
			ft_free(sim);
			return (0);
		}
	}
	i = -1;
	while (++i < sim->num_philos)
		pthread_join(sim->philos[i].thread_id, NULL);
	pthread_join(sim->observer, NULL);
	return (1);
}

static int	ft_philo_init(t_simulator *sim)
{
	int	i;

	sim->philos = malloc(sizeof(t_philo) * sim->num_philos);
	if (!sim->philos)
	{
		free(sim->forks);
		return (0);
	}
	i = -1;
	while (++i < sim->num_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].last_meal = sim->start_time;
		sim->philos[i].left_fork = &sim->forks[i];
		sim->philos[i].right_fork = &sim->forks[(i + 1) % sim->num_philos];
		sim->philos[i].sim = sim;
		sim->philos[i].total_flag = 0;
		pthread_mutex_init(&sim->philos[i].meal, NULL);
	}
	return (1);
}

int	ft_init(t_simulator *sim, int ac, char **av)
{
	int	i;

	sim->num_philos = ft_atoi(av[1]);
	sim->time_to_die = ft_atoi(av[2]);
	sim->time_to_eat = ft_atoi(av[3]);
	sim->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		sim->must_eat = ft_atoi(av[5]);
	else
		sim->must_eat = -1;
	sim->stop_sim = 0;
	sim->start_time = current_time();
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->num_philos);
	if (!sim->forks)
		return (0);
	i = -1;
	while (++i < sim->num_philos)
		pthread_mutex_init(&sim->forks[i], NULL);
	pthread_mutex_init(&sim->print_lock, NULL);
	pthread_mutex_init(&sim->death_lock, NULL);
	if (!ft_philo_init(sim))
		return (0);
	return (1);
}
