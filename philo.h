#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdbool.h>

/* Structure to hold simulation parameters */
typedef struct s_params
{
    int             number_of_philosophers;
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    int             number_of_times_each_philosopher_must_eat;
    bool            must_eat_count; // Flag to check if the above argument is provided
}               t_params;

/* Structure representing a philosopher */
typedef struct s_philosopher
{
    int             id;
    pthread_t       thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    long            last_meal_time;
    int             meals_eaten;
    struct s_table  *table;
}               t_philosopher;

/* Structure representing the table and shared resources */
typedef struct s_table
{
    t_params        params;
    pthread_mutex_t *forks;
    t_philosopher   *philosophers;
    pthread_mutex_t writing_mutex; // To prevent log messages from mixing
    long            start_time;
    bool            someone_died;
    pthread_mutex_t death_mutex; // To protect 'someone_died' flag
}               t_table;

#endif
