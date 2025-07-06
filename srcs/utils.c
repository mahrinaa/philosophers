/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:27 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 22:03:27 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_error_and_free(char *msg, t_rules *rules)
{
	free_rules(rules);         // Libère tout ce qui a déjà été initialisé
	exit_print_error(msg);     // Affiche et quitte
}

void	exit_print_error(char *msg)
{
	int	i;

	i = 0;
	while (msg && msg[i])
	{
		write(STDERR_FILENO, &msg[i], 1);
		i++;
	}
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}

long long	get_current_time_in_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("error with function gettimeofday\n");
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_atoi(const char *str)
{
	size_t	i;
	size_t	nb;
	size_t	sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
	{
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			sign *= -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

void	ft_putstr_fd(char *str, int fd)
{
	size_t	i;

	i = 0;
	if (str == 0 || fd < 0)
		return ;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
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
