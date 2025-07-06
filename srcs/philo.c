/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:18 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 06:53:36 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//philo routine
void	set_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->timing_mutex);
	philo->last_meal = get_current_time_in_ms();
	pthread_mutex_unlock(&philo->timing_mutex);
}

void *philo_routine(void *arg)
{
	t_philo	*phi;

	phi = (t_philo *)arg;
	set_last_meal(phi);
	if (phi->rules->nb_philos == 1)
		return (one_philo_case(phi));
	if (phi->id % 2 == 0)
		usleep(500);
	while (!check_death(phi->rules))
	{
		print_action(phi, "is thinking");
		philo_eat_cycle(phi);
		if (check_death(phi->rules))
			break ;
		print_action(phi, "is sleeping");
		smart_sleep(phi->rules->time_to_sleep, phi);
	}
	return (NULL);
}

void	*one_philo_case(t_philo *philo)
{
	long long	start;

	start = get_current_time_in_ms(); 
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	while (get_current_time_in_ms() - start < philo->rules->time_to_die)
		usleep(100);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
	
}

static int	create_philos(t_rules *rules)
{
	int i;

	i = 0;
	while (i < rules->nb_philos)
	{
		if (pthread_create(&rules->philos[i].thread, NULL, philo_routine, &rules->philos[i]))
			return (1);
		usleep(100);
		i++;
	}
	return (0);
}

static void	join_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philos)
	{
		pthread_join(rules->philos[i].thread, NULL);
		i++;
	}
}

void	start_simulation(t_rules *rules)
{
	pthread_t	monitor;

	if (create_philos(rules))
		exit (1);
	if (pthread_create(&monitor, NULL, monitor_routine, rules))
		exit (1);
	pthread_join(monitor, NULL);
	join_philos(rules);
}
