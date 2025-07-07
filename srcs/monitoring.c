/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 05:01:52 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 08:08:08 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	handle_death(t_rules *rules, int i)
{
	long long	time;

	if (!rules)
		return ;
	pthread_mutex_lock(&rules->death_mutex);
	if (rules->philo_died)
		return ((void)pthread_mutex_unlock(&rules->death_mutex));
	rules->philo_died = 1;
	pthread_mutex_unlock(&rules->death_mutex);
	pthread_mutex_lock(&rules->print_mutex);
	time = get_current_time_in_ms() - rules->start_time;
	printf("%lld %d died\n", time, rules->philos[i].id);
	pthread_mutex_unlock(&rules->print_mutex);
}

void	handle_done(t_rules *rules)
{
	if (!rules)
		return ;
	pthread_mutex_lock(&rules->death_mutex);
	rules->philo_died = 1;
	pthread_mutex_unlock(&rules->death_mutex);
}

static int	check_full(t_rules *rules)
{
	int	i;
	int	full;

	if (rules->must_eat <= 0)
		return (0);
	i = 0;
	full = 0;
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].timing_mutex);
		if (rules->philos[i].meals_eaten >= rules->must_eat)
			full++;
		pthread_mutex_unlock(&rules->philos[i].timing_mutex);
		i++;
	}
	return (full == rules->nb_philos);
}

int	check_death(t_rules *rules)
{
	int			i;
	long long 	time;

	i = 0;
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].timing_mutex);
		time = get_current_time_in_ms() - rules->philos[i].last_meal;
		if (time > rules->time_to_die)
		{
			pthread_mutex_unlock(&rules->philos[i].timing_mutex);
			handle_death(rules, i);
			return (1);
		}
		pthread_mutex_unlock(&rules->philos[i].timing_mutex);
		i++;
	}
	return (0);
}

int	check_end_condition(t_rules *rules)
{
	if (check_death(rules))
		return (1);
	if (check_full(rules))
	{
		handle_done(rules);
		usleep(2000);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_rules *rules;
	int		stopped;

	rules = (t_rules *)arg;
	stopped = 0;

	while (!stopped)
	{
		pthread_mutex_lock(&rules->death_mutex);
		if (rules->philo_died)
			stopped = 1;
		pthread_mutex_unlock(&rules->death_mutex);
		if (!stopped && check_end_condition(rules))
		{
			pthread_mutex_lock(&rules->death_mutex);
			rules->philo_died = 1;
			pthread_mutex_unlock(&rules->death_mutex);
			break ;
		}
		usleep (200);
	}
	return (NULL);
}
