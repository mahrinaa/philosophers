/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:18 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 21:44:11 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//philo routine
void	set_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->timing_mutex); //lock acces a last meal
	philo->last_meal = get_current_time_in_ms(); //enregistre temps actuel
	pthread_mutex_unlock(&philo->timing_mutex);//unlock
}

//script principal de chaque philo
void *philo_routine(void *arg)
{
	t_philo	*phi;

	phi = (t_philo *)arg;
	set_last_meal(phi); //init heure du dernier repas au depart
	if (phi->rules->nb_philos == 1)
		return (one_philo_case(phi)); //cas si 1 philo
	if (phi->id % 2 == 0)
		usleep(1000); //pasue pour desynchro les philo pairs
	while (!check_death(phi->rules))
	{
		display_action(phi, "is thinking");
		philo_eat_cycle(phi);
		if (check_death(phi->rules))
			break ;
		display_action(phi, "is sleeping");
		sleep_if_alive(phi->rules->time_to_sleep, phi); //pause pdt sommeil
	}
	return (NULL);
}

void	*one_philo_case(t_philo *philo)
{
	long long	start;

	start = get_current_time_in_ms(); //demarre chrno
	pthread_mutex_lock(philo->left_fork);
	display_action(philo, "has taken a fork"); //prend sa seule fork
	while (get_current_time_in_ms() - start < philo->rules->time_to_die)
		usleep(100); //atend la mort
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
	
}

//creation threead poru chaque philo
static void	create_philos(t_rules *rules)
{
	int i;

	i = 0;
	while (i < rules->nb_philos)
	{
		if (pthread_create(&rules->philos[i].thread, NULL, philo_routine, &rules->philos[i]))
			exit_print_error("Error: failed to create philosopher thread");
		usleep(100);
		i++;
	}
}
//qttend que tous les philos finissent leurs role, attqnd que chacun quitte la table proprement
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
	pthread_t	monitor;

	create_philos(rules); //cree les philos
	if (pthread_create(&monitor, NULL, monitor_routine, rules)) //cree thread e surveillance monitor
		exit_print_error("Error: failed to create monitor thread");
	pthread_join(monitor, NULL); //att mpniteur termine
	join_philos(rules); //attend tous les philos terminent
}
