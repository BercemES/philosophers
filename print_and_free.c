/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bercem <bercem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:04:33 by bekinci-          #+#    #+#             */
/*   Updated: 2025/05/07 17:09:08 by bercem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *philo, char *s)
{
	long long	time;
	int			stop;

	pthread_mutex_lock(&philo->sim->print_lock);
	time = current_time() - philo->sim->start_time;
	pthread_mutex_lock(&philo->sim->death_lock);
	stop = philo->sim->stop_sim;
	pthread_mutex_unlock(&philo->sim->death_lock);
	if (!stop || s[0] == 'd')
	{
		if (s[0] == 'd')
			printf(RED"%lld %d %s\n"RESET, time, philo->id, s);
		else if (s[3] == 'e')
			printf(GREEN"%lld %d %s\n"RESET, time, philo->id, s);
		else
			printf("%lld %d %s\n", time, philo->id, s);
	}
	pthread_mutex_unlock(&philo->sim->print_lock);
}

void	ft_free(t_simulator *sim)
{
	int	i;

	if (sim->philos)
	{
		i = -1;
		while (++i < sim->num_philos)
		{
			pthread_mutex_destroy(&sim->philos[i].meal);
		}
		free(sim->philos);
	}
	if (sim->forks)
	{
		i = -1;
		while (++i < sim->num_philos)
			pthread_mutex_destroy(&sim->forks[i]);
		free(sim->forks);
	}
	pthread_mutex_destroy(&sim->death_lock);
	pthread_mutex_destroy(&sim->print_lock);
	free(sim);
}
