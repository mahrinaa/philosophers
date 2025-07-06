/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:41:46 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 22:02:07 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//init un mutex pour chqaue fourchette
void	init_forks_mutex(t_rules *rules)
{
	int i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philos);
	if (!rules->forks)
		print_error_and_free("Error: malloc failed for forks", rules);
	i = 0;
	while (i < rules->nb_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL))
		{
			// Si l’initialisation d’un mutex échoue,
			// on détruit ceux déjà créés pour éviter les fuites
			while (--i >= 0)
				pthread_mutex_destroy(&rules->forks[i]);
			free(rules->forks);
			rules->forks = NULL;
			print_error_and_free("Error: mutex init failed for forks", rules);
		}
		i++;
	}
}

void	init_global_mutex(t_rules *rules)
{
	if (pthread_mutex_init(&rules->print_mutex, NULL))
		print_error_and_free("Error: mutex init failed (print)", rules);
	if (pthread_mutex_init(&rules->death_mutex, NULL))
	{
		pthread_mutex_destroy(&rules->print_mutex);
		print_error_and_free("Error: mutex init failed (death)", rules);
	}
	if (pthread_mutex_init(&rules->meal_check, NULL))
	{
		pthread_mutex_destroy(&rules->print_mutex);
		pthread_mutex_destroy(&rules->death_mutex);
		print_error_and_free("Error: mutex init failed (meal_check)", rules);
	}
}
//config param de la simulation
static void	set_rules_values(t_rules *rules, int ac, char **av)
{
	rules->nb_philos = ft_atoi(av[1]);
	rules->time_to_die = ft_atoi(av[2]);
	rules->time_to_eat = ft_atoi(av[3]);
	rules->time_to_sleep = ft_atoi(av[4]);
	rules->must_eat = -1; //pas de limite
	if (ac == 6)
		rules->must_eat = ft_atoi(av[5]);
	rules->philo_died = 0;
	rules->all_ate = 0;
	rules->start_time = get_current_time_in_ms();
	rules->philos = NULL;
	rules->forks = NULL;
}

//allouer structure t_rules avec les parametres generaux

t_rules *init_sim_rules(int ac, char **av)
{
	t_rules *rules;

	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (NULL);
	set_rules_values(rules, ac, av);
	init_forks_mutex(rules);
	init_global_mutex(rules);
	return (rules);
}

static void	init_one_philo(t_philo *philo, t_rules *rules, int i)
{
	philo->id = i + 1;
	philo->meals_eaten = 0;
	philo->last_meal = rules->start_time; //dernier repas a lheure du depart du prog
	philo->rules = rules; //acces aux regles communes
	philo->left_fork = &rules->forks[i];
	philo->right_fork = &rules->forks[(i + 1) % rules->nb_philos]; //fourchette droite du philo suivant
	pthread_mutex_init(&philo->timing_mutex, NULL); //proteger les acces a last meal et meal eaten
}

t_philo	*init_philo(t_rules *rules)
{
	t_philo *philo;
	int		 i;

	philo = malloc((sizeof(t_philo) * rules->nb_philos));
	if (!philo)
		return (NULL);
	i = 0;
	while (i < rules->nb_philos)
	{
		init_one_philo(&philo[i], rules, i);
		i++;
	}
	rules->philos = philo;
	return (philo);
}
