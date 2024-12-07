#include "philo.h"
#include <unistd.h>
#include <stdio.h>

void log_action(t_table *table, int philosopher_id, char *action)
{
    // Check someone_died under data_mutex
    pthread_mutex_lock(&table->data_mutex);
    bool died = table->someone_died;
    pthread_mutex_unlock(&table->data_mutex);

    pthread_mutex_lock(&table->writing_mutex);
    if (!died) {
        long timestamp = get_timestamp_in_ms() - table->start_time;
        printf("%ld %d %s\n", timestamp, philosopher_id, action);
    }
    pthread_mutex_unlock(&table->writing_mutex);
}

static void think(t_philosopher *philo)
{
    log_action(philo->table, philo->id, "is thinking");
}

static void eat(t_philosopher *philo)
{
    pthread_mutex_t *first;
    pthread_mutex_t *second;

    // Determine fork order based on IDs to maintain consistent lock ordering
    // Let's say always lock the fork with the smaller address (or index) first
    if (philo->left_fork < philo->right_fork)
    {
        first = philo->left_fork;
        second = philo->right_fork;
    }
    else
    {
        first = philo->right_fork;
        second = philo->left_fork;
    }

    // Lock first fork
    pthread_mutex_lock(first);
    log_action(philo->table, philo->id, "has taken a fork");

    // Lock second fork
    pthread_mutex_lock(second);
    log_action(philo->table, philo->id, "has taken a fork");

    pthread_mutex_lock(&philo->table->data_mutex);
    philo->last_meal_time = get_timestamp_in_ms();
    pthread_mutex_unlock(&philo->table->data_mutex);

    log_action(philo->table, philo->id, "is eating");
    usleep(philo->table->params.time_to_eat * 1000);

    pthread_mutex_lock(&philo->table->data_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->table->data_mutex);

    // Unlock in reverse order
    pthread_mutex_unlock(second);
    pthread_mutex_unlock(first);
}


static void my_sleep(t_philosopher *philo)
{
    log_action(philo->table, philo->id, "is sleeping");
    usleep(philo->table->params.time_to_sleep * 1000);
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    // Special case: only one philosopher
    if (philo->table->params.number_of_philosophers == 1) {
        pthread_mutex_lock(philo->left_fork);
        log_action(philo->table, philo->id, "has taken a fork");
        // Philosopher cannot take second fork, waits until time_to_die
        usleep(philo->table->params.time_to_die * 1000);
        log_action(philo->table, philo->id, "died");
        pthread_mutex_unlock(philo->left_fork);
        // Set someone_died flag
        set_death(philo->table);
        return NULL;
    }

    // To reduce chance of deadlock, stagger philosopher start
    if (philo->id % 2 == 0)
        usleep(100);

    while (!has_someone_died(philo->table)) {
        think(philo);
        eat(philo);
        if (philo->table->params.must_eat_count) {
            pthread_mutex_lock(&philo->table->data_mutex);
            if (philo->meals_eaten >= philo->table->params.number_of_times_each_philosopher_must_eat) {
                pthread_mutex_unlock(&philo->table->data_mutex);
                break;
            }
            pthread_mutex_unlock(&philo->table->data_mutex);
        }
        my_sleep(philo);
    }

    return NULL;
}
