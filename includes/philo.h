/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:31 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 07:06:40 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/time.h>
# include <pthread.h>



typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal;
	pthread_t			thread;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		timing_mutex;
	struct s_rules		*rules;

}			  t_philo;


typedef struct	s_rules
{
	int					nb_philos;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_die;
	int					must_eat;
	int					all_ate;
	long long 			start_time;
	int					philo_died;
	
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		meal_check;
	t_philo				*philos;
	
}	t_rules;

//utils.c
void		ft_putstr_fd(char *str, int fd);
int			ft_atoi(const char *str);
long long	get_current_time_in_ms(void);
int			check_death(t_rules *rules);
void		smart_sleep(int time_in_ms, t_philo *philo);
void		print_action(t_philo *philo, char *action);
int			is_dead(t_rules *rules);


//init.c
void		init_data(char **argv, t_rules *data);
char		valid_argument(int argc, char **argv);
long		is_positive_int(const char *str);

//philo.c
void		start_simulation(t_rules *rules);
void		*one_philo_case(t_philo *philo);
void 		*philo_routine(void *arg);
t_rules		*init_sim_rules(int argc, char **argv);
t_philo		*init_philo(t_rules *rules);
void		free_rules(t_rules *rules);

//eating.c
void		philo_eat_cycle(t_philo *philo);
int			grab_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *second);
void		start_eating(t_philo *philo);

//monitoring.h
void		*monitor_routine(void *arg);
int			check_philos(t_rules *rules);
void		handle_done(t_rules *rules);
void		handle_death(t_rules *rules, int i);
int			check_death(t_rules *rules);


#endif