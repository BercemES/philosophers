/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bekinci- <bekinci-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:05:02 by bekinci-          #+#    #+#             */
/*   Updated: 2025/05/09 15:46:53 by bekinci-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define RESET		"\033[0m"
# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define YELLOW 	"\033[33m"
# define BLUE		"\033[34m"

typedef struct s_simulator	t_simulator;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal;
	int				total_flag;
	pthread_t		thread_id;
	pthread_mutex_t	meal;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_simulator		*sim;
}	t_philo;

typedef struct s_simulator
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				stop_sim;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	print_lock;
	t_philo			*philos;
	pthread_t		observer;
}	t_simulator;

int			ft_init(t_simulator *sim, int ac, char **av);
int			create_thread(t_simulator *sim);
int			ft_atoi(char *str);
int			digit_control(char **av);
long long	current_time(void);
void		print_action(t_philo *philo, char *s);
void		*check_philo(void *sim);
void		*routine(void *philos);
void		ft_free(t_simulator *sim);

#endif
