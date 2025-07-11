/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:18 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 13:35:22 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	set_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->timing_mutex);
	philo->last_meal = get_current_time_in_ms();
	pthread_mutex_unlock(&philo->timing_mutex);
}

void	*philo_routine(void *arg)
{
	t_philo	*phi;

	phi = (t_philo *)arg;
	if (phi->rules->nb_philos == 1)
		return (one_philo_case(phi));
	display_action(phi, "is thinking");
	if (phi->id % 2 == 0)
		usleep(100);
	while (!is_philo_dead(phi))
	{
		pthread_mutex_lock(&phi->timing_mutex);
		if (phi->rules->must_eat > 0 && \
			phi->meals_eaten >= phi->rules->must_eat)
			pthread_mutex_unlock(&phi->timing_mutex);
		pthread_mutex_unlock(&phi->timing_mutex);
		philo_eat_cycle(phi);
		display_action(phi, "is sleeping");
		sleep_if_alive(phi->rules->time_to_sleep, phi);
		display_action(phi, "is thinking");
		if (phi->rules->nb_philos % 2 != 0)
			sleep_if_alive(phi->rules->time_to_think, phi);
	}
	return (NULL);
}

void	*one_philo_case(t_philo *philo)
{
	long long	start;

	start = get_current_time_in_ms();
	pthread_mutex_lock(philo->left_fork);
	display_action(philo, "has taken a fork");
	while (get_current_time_in_ms() - start < philo->rules->time_to_die)
		usleep(100);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

static void	join_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philos)
	{
		if (pthread_join(rules->philos[i].thread, NULL))
			exit_print_error("Error: thread join failed");
		i++;
	}
}

void	start_simulation(t_rules *rules)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	while (i < rules->nb_philos)
	{
		set_last_meal(&rules->philos[i]);
		if (pthread_create(&rules->philos[i].thread, NULL,
				philo_routine, &rules->philos[i]))
			exit_print_error("Error : failed to create philosopher thread");
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, rules))
		exit_print_error("Error : failed to create monitor thread");
	if (pthread_join(monitor, NULL))
		exit_print_error("Error : monitor thread join failed");
	join_philos(rules);
}
