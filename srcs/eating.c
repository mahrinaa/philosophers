/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 05:01:45 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 10:24:28 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	start_eating(t_philo *philo)
{
	long long	now;

	if (is_philo_dead(philo) == 1)
		return ;
	display_action(philo, "is eating");
	now = get_current_time_in_ms();
	pthread_mutex_lock(&philo->timing_mutex);
	philo->last_meal = now;
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->timing_mutex);
	sleep_if_alive(philo->rules->time_to_eat, philo);
}


int	grab_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	display_action(philo, "has taken a fork");
	if (is_philo_dead(philo))
	{
		pthread_mutex_unlock(first);//repose la 1er fourchette pour eviter deadlock
		return (0);
	}
	pthread_mutex_lock(second);
	display_action(philo, "has taken a fork");
	if (is_philo_dead(philo))
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);//repose la 1er fourchette pour eviter deadlock
		return (0);
	}
	return (1);
}

//determine 
//ordre des fourchettes + les prendre
//lancer le repas
//reposer fourchettes
void	philo_eat_cycle(t_philo *philo)
{
	int				attempts;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = philo->left_fork; //impair prennent dabord gauche
	second = philo->right_fork;
	if (philo->id % 2 == 0) //philo pair prennent dabord les forks droite
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	attempts = 0;
	//while (!philo->rules->philo_died && attempts++ < 1000)
	while (!check_death_status(philo->rules) && attempts++ < 1000)
	{
		if (!grab_forks(philo, first, second))
		{
			usleep(100);//attend avant de reeessayer
			continue ;
		}
		// // Re-check après avoir pris les fourchettes
		// if (check_death_status(philo->rules))
		// {
		// 	pthread_mutex_unlock(second);
		// 	pthread_mutex_unlock(first);
		// 	break ;
		// }
		start_eating(philo); //manger puis
		pthread_mutex_unlock(second); //liberer fourchette
		pthread_mutex_unlock(first);
		break ;
	}
}

