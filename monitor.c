#include "philo.h"
#include <unistd.h>
#include <stdio.h>

void *monitor_routine(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;

    while (!has_someone_died(table)) {
        int completed_eaters = 0;
        for (i = 0; i < table->params.number_of_philosophers; i++) {
            pthread_mutex_lock(&table->data_mutex);
            long current_time = get_timestamp_in_ms();
            long time_since_last_meal = current_time - table->philosophers[i].last_meal_time;

            if (time_since_last_meal > table->params.time_to_die && !table->someone_died) {
                // Philosopher died
                table->someone_died = true;
                pthread_mutex_unlock(&table->data_mutex);
                log_action(table, table->philosophers[i].id, "died");
                return NULL;
            }

            // printf("Philosopher %d has eaten %d times.\n", i+1, table->philosophers[i].meals_eaten);


            if (table->params.must_eat_count &&
                table->philosophers[i].meals_eaten >= table->params.number_of_times_each_philosopher_must_eat)
            {
                completed_eaters++;
            }

            pthread_mutex_unlock(&table->data_mutex);
        }

        if (table->params.must_eat_count && completed_eaters == table->params.number_of_philosophers) {
            pthread_mutex_lock(&table->data_mutex);
            table->someone_died = true; // Signal everyone to stop
            pthread_mutex_unlock(&table->data_mutex);
            return NULL;
        }
        usleep(1000);
    }
    return NULL;
}

bool create_philosopher_threads(t_table *table)
{
    for (int i = 0; i < table->params.number_of_philosophers; i++) {
        if (pthread_create(&table->philosophers[i].thread, NULL, philosopher_routine, &table->philosophers[i]) != 0)
            return false;
    }
    return true;
}

bool monitor_philosophers(t_table *table)
{
    pthread_t monitor_thread;
    if (pthread_create(&monitor_thread, NULL, monitor_routine, table) != 0)
        return false;
    pthread_join(monitor_thread, NULL);
    return true;
}
