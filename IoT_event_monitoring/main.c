#include "main.h"

static void print_header(void) {
    printf("Student ID: %s\n", STUDENT_ID_HEADER);
    printf("Project: Dynamic IoT Event Monitoring and Callback Engine\n");
    printf("Custom Callback: Sensor Drift Detector\n");
}

static void print_menu(void) {
    printf("\n----------------- MENU -----------------\n");
    printf(" 1. Register a new device\n");
    printf(" 2. Display all devices\n");
    printf(" 3. Remove a device\n");
    printf(" 4. Generate and process readings\n");
    printf(" 5. Display detected alerts\n");
    printf(" 6. Monitoring summary\n");
    printf(" 7. Exit\n");
}

int main(int argc, char *argv[]) {
    Monitor m = {0};
    m.running = 1;

    /* random seed: time by default, or ./iot_monitor <seed> for repeatable runs */
    unsigned int seed = (argc > 1) ? (unsigned int)strtoul(argv[1], NULL, 10)
                                   : (unsigned int)time(NULL);
    srand(seed);

    /* function-pointer table for the menu */
    MenuAction actions[] = {
        register_device,
        display_devices,
        remove_device,
        simulate_readings,
        display_alerts,
        monitoring_summary,
        terminate_program,
    };
    int num_actions = (int)(sizeof(actions) / sizeof(actions[0]));

    print_header();
    printf("Random seed: %u\n", seed);

    /* starting set of devices: two of each required type */
    static const struct { int id; const char *name; int type; } demo[] = {
        {101, "Boiler-Temp",     TYPE_TEMPERATURE}, {102, "ColdRoom-Temp",   TYPE_TEMPERATURE},
        {201, "Line-Pressure",   TYPE_PRESSURE},    {202, "Tank-Pressure",   TYPE_PRESSURE},
        {301, "Drone-Battery",   TYPE_BATTERY},     {302, "Sensor-Battery",  TYPE_BATTERY},
    };
    for (size_t i = 0; i < sizeof(demo) / sizeof(demo[0]); i++) {
        if (add_device(&m, demo[i].id, demo[i].name, demo[i].type) != 1) {
            printf("Memory allocation failed!\n");
            free(m.devices);
            return 1;
        }
    }
    printf("Registered %d starting devices.\n", m.count);

    while (m.running) {
        int choice;
        print_menu();
        if (!read_int("Enter your choice (1-7): ", &choice)) {
            if (feof(stdin)) break;
            printf("Invalid input. Please enter a number between 1 and %d.\n", num_actions);
            continue;
        }
        if (choice < 1 || choice > num_actions) {
            printf("Invalid choice. Please enter a number between 1 and %d.\n", num_actions);
            continue;
        }
        actions[choice - 1](&m);
    }

    free(m.devices);     /* release all heap memory */
    free(m.alerts);
    return 0;
}