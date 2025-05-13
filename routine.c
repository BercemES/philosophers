/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekinci- <bekinci-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:44:53 by bekinci-          #+#    #+#             */
/*   Updated: 2025/05/09 15:45:10 by bekinci-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	must_eat_check(t_simulator *sim)
{
	int	i;
	int	total;

	i = -1;
	total = 0;
	while (++i < sim->num_philos)
	{
		pthread_mutex_lock(&sim->philos[i].meal);
		if (sim->philos[i].meals_eaten == sim->must_eat)
			sim->philos[i].total_flag = 1;
		if (sim->philos[i].total_flag == 1)
			total++;
		pthread_mutex_unlock(&sim->philos[i].meal);
	}
	if (total == sim->num_philos)
	{
		pthread_mutex_lock(&sim->death_lock);
		sim->stop_sim = 1;
		pthread_mutex_unlock(&sim->death_lock);
	}
}

void	time_control(t_simulator *sim)
{
	int	i;

	i = -1;
	while (++i < sim->num_philos)
	{
		pthread_mutex_lock(&sim->philos[i].meal);
		if (current_time() - sim->philos[i].last_meal >= sim->time_to_die)
		{
			print_action(&sim->philos[i], "died");
			pthread_mutex_unlock(&sim->philos[i].meal);
			pthread_mutex_lock(&sim->death_lock);
			sim->stop_sim = 1;
			pthread_mutex_unlock(&sim->death_lock);
			return ;
		}
		pthread_mutex_unlock(&sim->philos[i].meal);
	}
}

void	*check_philo(void *simulator)
{
	t_simulator	*sim;

	sim = (t_simulator *)simulator;
	while (1)
	{
		pthread_mutex_lock(&sim->death_lock);
		if (sim->stop_sim == 1)
		{
			pthread_mutex_unlock(&sim->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->death_lock);
		time_control(sim);
		if (sim->must_eat != -1)
			must_eat_check(sim);
		usleep(1000);
	}
	return (NULL);
}

static void	philo_routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
	}
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->meal);
	philo->last_meal = current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal);
	usleep(philo->sim->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_action(philo, "is sleeping");
	usleep(philo->sim->time_to_sleep * 1000);
	print_action(philo, "is thinking");
}

void	*routine(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (philo->id % 2 == 0)
		usleep(200);
	if (philo->sim->num_philos == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		usleep(philo->sim->time_to_die * 1000);
		pthread_mutex_unlock(philo->right_fork);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&philo->sim->death_lock);
		if (philo->sim->stop_sim)
		{
			pthread_mutex_unlock(&philo->sim->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->sim->death_lock);
		philo_routine(philo);
	}
	return (philo);
}
