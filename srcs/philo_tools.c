/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 01:14:34 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 10:11:23 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	is_philo_dead(t_philo *philo)
{
	int	philo_dead;

	pthread_mutex_lock(&philo->rules->death_mutex);
	philo_dead = philo->rules->philo_died;
	pthread_mutex_unlock(&philo->rules->death_mutex);
	return (philo_dead);
}

void	display_action(t_philo *philo, const char *msg)
{
	long long	time;

	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!check_death_status(philo->rules))
	{
		time = get_current_time_in_ms() - philo->rules->start_time;
		printf("%lld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

int	check_death_status(t_rules *rules)
{
	pthread_mutex_lock(&rules->death_mutex);
	if (rules->philo_died)
		return (pthread_mutex_unlock(&rules->death_mutex), 1);
	pthread_mutex_unlock(&rules->death_mutex);
	return (0);
}

void	sleep_if_alive(int time_in_ms, t_philo *philo)
{
	long long	end_time;
	long long	current_time;
	int			time_left;

	end_time = get_current_time_in_ms() + time_in_ms;
	while (!check_death_status(philo->rules))
	{
		current_time = get_current_time_in_ms();
		time_left = end_time - current_time;
		if (time_left <= 0)
			break ;
		if (time_left > 5)
			usleep(2000);
		else
			usleep(500);
	}
}
