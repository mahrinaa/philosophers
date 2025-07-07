/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 06:15:55 by mai               #+#    #+#             */
/*   Updated: 2025/07/07 03:50:55 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//met a jour letat global philo_diead 
void	handle_death(t_rules *rules, int i)
{
	long long	time; //calcul du tps ecoulee, marque lheure du deces
	if (!rules)
		return ;
	pthread_mutex_lock(&rules->death_mutex);
	if (rules->philo_died) //si un philo deja mort
		return ((void)pthread_mutex_unlock(&rules->death_mutex));

	rules->philo_died = 1; //sim fini a cause d1 mort
	pthread_mutex_unlock(&rules->death_mutex);
	pthread_mutex_lock(&rules->print_mutex);//lock laffiche des msg, 1 seul thread peut parler

	time = get_current_time_in_ms() - rules->start_time;
	printf("%lld %d died\n", time, rules->philos[i].id);//annonce la mort
	pthread_mutex_unlock(&rules->print_mutex);//liberer la parole
}

void	handle_done(t_rules *rules)
{
	if (!rules)
		return ;
	pthread_mutex_lock(&rules->death_mutex);
	rules->philo_died = 1;//fin normale
	pthread_mutex_unlock(&rules->death_mutex);
	
}

//verifie si assez manger 
static int	check_full(t_rules *rules)
{
	int	i;
	int	full; //philos ont mangé au moins must_eat fois

	if (rules->must_eat <= 0)
		return (0);
	i = 0;
	full = 0;
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].timing_mutex);
		if (rules->philos[i].meals_eaten >= rules->must_eat)
			full++; //compte philo qui a fini son repas
		pthread_mutex_unlock(&rules->philos[i].timing_mutex);
		i++; //passe au philo suivant
	}
	return (full == rules->nb_philos); //tlm est full, a atteit son quota
}

//verifie si un philo est mort
int	check_death(t_rules *rules)
{
	int			i;
	long long 	time; //tmps ecoule depuis dernier repas

	i = 0;
	while (i < rules->nb_philos)
	{
		pthread_mutex_lock(&rules->philos[i].timing_mutex); //verouille lacces aux donnes last_meal
		time = get_current_time_in_ms() - rules->philos[i].last_meal;
		if (time > rules->time_to_die) // si depasse time_to_die = mort
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

int	check_philos(t_rules *rules)
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

//monitor surveille en boucle letat des philo, verife si qqn meurt
void	*monitor_routine(void *arg)
{
	t_rules *rules;
	int		stopped;

	rules = (t_rules *)arg; //recuperer regles du prog
	stopped = 0; //flag darret

	while (!stopped)
	{
		pthread_mutex_lock(&rules->death_mutex);
		if (rules->philo_died)
			stopped = 1; //si philo meurt arrete de surveiller
		pthread_mutex_unlock(&rules->death_mutex); //si tlm finit de manger
		if (!stopped && check_philos(rules))
		{
			pthread_mutex_lock(&rules->death_mutex);
			rules->philo_died = 1;
			pthread_mutex_unlock(&rules->death_mutex);
			break ;
		}
		usleep (200); //pause pour eviter de surchager
	}
	return (NULL);
}
