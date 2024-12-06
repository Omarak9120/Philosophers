// monitor.c
#include "philo.h"

// monitor.c (update monitor_routine)
void *monitor_routine(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    long current_time;
    int completed_eaters;

    while (!has_someone_died(table))
    {
        completed_eaters = 0;
        for (i = 0; i < table->params.number_of_philosophers; i++)
        {
            pthread_mutex_lock(&table->death_mutex);
            current_time = get_timestamp_in_ms();
            if ((current_time - table->philosophers[i].last_meal_time) > table->params.time_to_die)
            {
                log_action(table, table->philosophers[i].id, "died");
                set_death(table);
                pthread_mutex_unlock(&table->death_mutex);
                return NULL;
            }

            // printf("Philosopher %d has eaten %d times.\n", i+1, table->philosophers[i].meals_eaten);

            // Check if philosopher has eaten enough
            if (table->params.must_eat_count && table->philosophers[i].meals_eaten >= table->params.number_of_times_each_philosopher_must_eat)
                completed_eaters++;
            pthread_mutex_unlock(&table->death_mutex);
        }
        if (table->params.must_eat_count && completed_eaters >= table->params.number_of_philosophers)
        {
            pthread_mutex_lock(&table->death_mutex);
            table->someone_died = true;
            pthread_mutex_unlock(&table->death_mutex);
            return NULL;
        }
        usleep(1000); // Check every 1 ms
    }
    return NULL;
}


/* Function to monitor philosophers */
bool monitor_philosophers(t_table *table)
{
    pthread_t monitor_thread;

    if (pthread_create(&monitor_thread, NULL, monitor_routine, table) != 0)
        return false;
    pthread_join(monitor_thread, NULL);
    return true;
}
