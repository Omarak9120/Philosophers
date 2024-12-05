// main.c (continuing from previous code)
#include "philo.h"

// Prototype declarations (optional, depending on your project settings)
bool initialize_table(t_table *table);
bool create_philosopher_threads(t_table *table);
bool monitor_philosophers(t_table *table);
void cleanup_table(t_table *table);

int main(int argc, char **argv)
{
    t_table table;

    memset(&table, 0, sizeof(t_table)); // Initialize the table structure to zero
    if (!parse_arguments(argc, argv, &table.params))
    {
        printf("Error: Invalid arguments.\n");
        return 1;
    }

    if (!initialize_table(&table))
    {
        printf("Error: Initialization failed.\n");
        return 1;
    }

    if (!create_philosopher_threads(&table))
    {
        printf("Error: Thread creation failed.\n");
        return 1;
    }

    if (!monitor_philosophers(&table))
    {
        printf("Error: Monitoring failed.\n");
        return 1;
    }

    cleanup_table(&table);
    return 0;
}
