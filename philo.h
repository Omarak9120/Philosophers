#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdbool.h>

/*
 * philo.h
 *
 * This header defines all necessary structures and function prototypes
 * for the philosophers simulation.
 */

/* Structure to hold simulation parameters */
typedef struct s_params
{
    int             number_of_philosophers;
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    int             number_of_times_each_philosopher_must_eat;
    bool            must_eat_count; // Set to true if the optional argument is provided
}               t_params;

/* Forward declaration of s_table to use it inside s_philosopher */
struct s_table;

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

typedef struct s_table
{
    t_params        params;
    pthread_mutex_t *forks;
    t_philosopher   *philosophers;
    pthread_mutex_t writing_mutex; // To prevent log messages mixing
    long            start_time;
    bool            someone_died;
    pthread_mutex_t death_mutex;   // To protect 'someone_died' flag
}               t_table;

/* Function prototypes */
bool parse_arguments(int argc, char **argv, t_params *params);
bool initialize_table(t_table *table);
bool create_philosopher_threads(t_table *table);
bool monitor_philosophers(t_table *table);
void cleanup_table(t_table *table);
long get_timestamp_in_ms(void);
bool has_someone_died(t_table *table);
void log_action(t_table *table, int philosopher_id, char *action);
void set_death(t_table *table);

#endif
