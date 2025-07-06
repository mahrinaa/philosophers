/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mai <mai@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:27 by mapham            #+#    #+#             */
/*   Updated: 2025/07/06 07:05:59 by mai              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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

void	print_action(t_philo *philo, char *action)
{
	if (is_dead(philo->rules))
		return ;
	pthread_mutex_lock(&philo->rules->print_mutex);
	printf("%lld %d %s\n",
		get_current_time_in_ms() - philo->rules->start_time,
		philo->id, action);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

int	is_dead(t_rules *rules)
{
	int	status;

	pthread_mutex_lock(&rules->death_mutex);
	status = rules->philo_died;
	pthread_mutex_unlock(&rules->death_mutex);
	return (status);
}

void	smart_sleep(int time_in_ms, t_philo *philo)
{
	long long	start;

	start = get_current_time_in_ms();
	while (!is_dead(philo->rules))
	{
		if (get_current_time_in_ms() - start >= time_in_ms)
			break ;
		usleep(500);
	}
}