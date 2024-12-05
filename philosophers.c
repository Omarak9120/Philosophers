// philosopher.c
#include "philo.h"

/* Function to print log messages safely */
void log_action(t_table *table, int philosopher_id, char *action)
{
    long timestamp;

    pthread_mutex_lock(&table->writing_mutex);
    if (!table->someone_died)
    {
        timestamp = get_timestamp_in_ms() - table->start_time;
        printf("%ld %d %s\n", timestamp, philosopher_id, action);
    }
    pthread_mutex_unlock(&table->writing_mutex);
}

/* Function to check if someone has died */
bool has_someone_died(t_table *table)
{
    bool died;

    pthread_mutex_lock(&table->death_mutex);
    died = table->someone_died;
    pthread_mutex_unlock(&table->death_mutex);
    return died;
}

/* Function to set the death flag */
void set_death(t_table *table)
{
    pthread_mutex_lock(&table->death_mutex);
    table->someone_died = true;
    pthread_mutex_unlock(&table->death_mutex);
}

/* Philosopher routine */
void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_table *table = philo->table;

    if (philo->id % 2 == 0)
        usleep(100); // Slight delay for even-numbered philosophers to prevent deadlock

    while (!has_someone_died(table))
    {
        // Thinking
        log_action(table, philo->id, "is thinking");

        // Taking forks
        pthread_mutex_lock(philo->left_fork);
        log_action(table, philo->id, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        log_action(table, philo->id, "has taken a fork");

        // Eating
        pthread_mutex_lock(&table->death_mutex);
        philo->last_meal_time = get_timestamp_in_ms();
        pthread_mutex_unlock(&table->death_mutex);

        log_action(table, philo->id, "is eating");
        usleep(table->params.time_to_eat * 1000); // Convert ms to us

        // Update meals eaten
        philo->meals_eaten += 1;

        // Release forks
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);

        // Sleeping
        log_action(table, philo->id, "is sleeping");
        usleep(table->params.time_to_sleep * 1000);
    }
    return NULL;
}
