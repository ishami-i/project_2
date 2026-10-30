#include "main.h"

const char *type_name(int type) {
    static const char *names[TYPE_COUNT] = {"Temperature", "Pressure", "Battery"};
    return (type >= 0 && type < TYPE_COUNT) ? names[type] : "Unknown";
}

const char *type_unit(int type) {
    static const char *units[TYPE_COUNT] = {"C", "hPa", "V"};
    return (type >= 0 && type < TYPE_COUNT) ? units[type] : "?";
}

// Reads the union member that matches the device type (never any other). 
float device_value(const Device *d) {
    switch (d->type) {
        case TYPE_TEMPERATURE: return d->readings.temperature;
        case TYPE_PRESSURE:    return (float)d->readings.pressure;
        case TYPE_BATTERY:     return d->readings.voltage;
        default:               return 0.0f;
    }
}

// Writes the union member that matches the device type.
void set_device_value(Device *d, float v) {
    switch (d->type) {
        case TYPE_TEMPERATURE: d->readings.temperature = v; break;
        case TYPE_PRESSURE:    d->readings.pressure = (int)(v + (v < 0 ? -0.5f : 0.5f)); break;
        case TYPE_BATTERY:     d->readings.voltage = v; break;
        default: break;
    }
}

int find_device_index(const Monitor *m, int id) {
    for (const Device *p = m->devices; p < m->devices + m->count; p++)
        if (p->id == id) return (int)(p - m->devices);
    return -1;
}

// Adds a device; grows the array with realloc. Returns 1 ok, 0 duplicate, -1 no memory.
int add_device(Monitor *m, int id, const char *name, int type) {
    if (find_device_index(m, id) >= 0) return 0;
    if (m->devices == NULL) {
        m->devices = (Device *)calloc(INITIAL_CAPACITY, sizeof(Device));
        if (m->devices == NULL) return -1;
        m->capacity = INITIAL_CAPACITY;
        m->count = 0;
    }
    if (m->count == m->capacity) {
        int new_cap = m->capacity * 2;
        Device *tmp = (Device *)realloc(m->devices, (size_t)new_cap * sizeof(Device));
        if (tmp == NULL) return -1;
        printf("[memory] device array grew from %d to %d slots\n", m->capacity, new_cap);
        m->devices = tmp;
        m->capacity = new_cap;
    }
    Device *d = m->devices + m->count;
    memset(d, 0, sizeof(Device));
    d->id = id;
    strncpy(d->name, name, NAME_MAX_LEN - 1);
    d->name[NAME_MAX_LEN - 1] = '\0';
    d->type = type;
    d->status = STATUS_NORMAL;
    m->count++;
    return 1;
}

void register_device(Monitor *m) {
    int id, type;
    char name[NAME_MAX_LEN];
    if (!read_int("Enter device ID (positive number): ", &id) || id <= 0) {
        printf("Invalid ID.\n"); return;
    }
    if (find_device_index(m, id) >= 0) {
        printf("Error: a device with ID %d already exists.\n", id); return;
    }
    read_line("Enter device name: ", name, sizeof(name));
    printf("Device type:\n  1. Temperature sensor\n  2. Pressure sensor\n  3. Battery voltage monitor\n");
    if (!read_int("Enter type (1-3): ", &type) || type < 1 || type > TYPE_COUNT) {
        printf("Invalid device type.\n"); return;
    }
    int r = add_device(m, id, name, type - 1);
    if (r == 1) printf("Device %d (%s, %s) registered. Total devices: %d\n",
                       id, name, type_name(type - 1), m->count);
    else if (r == 0) printf("Error: duplicate device ID.\n");
    else printf("Memory allocation failed! Device not registered.\n");
}

void display_devices(Monitor *m) {
    if (m->count == 0) { printf("No devices registered.\n"); return; }
    printf("\n%-6s %-20s %-12s %-14s %s\n", "ID", "Name", "Type", "Last reading", "Status");
    printf("-----------------------------------------------------------------\n");
    for (const Device *p = m->devices; p < m->devices + m->count; p++) {
        char val[32];
        if (p->has_reading) snprintf(val, sizeof(val), p->type == TYPE_PRESSURE ? "%.0f %s" : "%.2f %s",
                                     device_value(p), type_unit(p->type));
        else snprintf(val, sizeof(val), "-");
        printf("%-6d %-20.20s %-12s %-14s %s\n", p->id, p->name, type_name(p->type), val,
               p->status == STATUS_ALERT ? "ALERT" : "Normal");
    }
    printf("-----------------------------------------------------------------\n");
    printf("Devices: %d (capacity %d)\n", m->count, m->capacity);
}

// Removes by ID, closes the gap, and shrinks the array when it is mostly empty.
void remove_device(Monitor *m) {
    int id;
    if (m->count == 0) { printf("No devices to remove.\n"); return; }
    display_devices(m);
    if (!read_int("Enter the ID of the device to remove: ", &id)) { printf("Invalid ID.\n"); return; }
    int i = find_device_index(m, id);
    if (i < 0) { printf("Device with ID %d not found.\n", id); return; }

    memmove(m->devices + i, m->devices + i + 1, (size_t)(m->count - i - 1) * sizeof(Device));
    m->count--;
    printf("Device %d removed. Total devices: %d\n", id, m->count);

    if (m->capacity > INITIAL_CAPACITY && m->count < m->capacity / 2) {
        int new_cap = m->capacity / 2;
        if (new_cap < INITIAL_CAPACITY) new_cap = INITIAL_CAPACITY;
        Device *tmp = (Device *)realloc(m->devices, (size_t)new_cap * sizeof(Device));
        if (tmp != NULL) {
            printf("[memory] device array shrank from %d to %d slots\n", m->capacity, new_cap);
            m->devices = tmp;
            m->capacity = new_cap;
        }
    }
}