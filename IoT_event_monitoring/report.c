#include "main.h"

/* Appends an alert (copy of the device's current alert data) to the dynamic log. */
void log_alert(Monitor *m, const Device *d) {
    if (m->alerts == NULL) {
        m->alerts = (Alert *)calloc(8, sizeof(Alert));
        if (m->alerts == NULL) { printf("Memory allocation failed! Alert not stored.\n"); return; }
        m->alert_cap = 8;
    }
    if (m->alert_count == m->alert_cap) {
        Alert *tmp = (Alert *)realloc(m->alerts, (size_t)m->alert_cap * 2 * sizeof(Alert));
        if (tmp == NULL) { printf("Memory allocation failed! Alert not stored.\n"); return; }
        m->alerts = tmp;
        m->alert_cap *= 2;
    }
    Alert *a = m->alerts + m->alert_count++;
    a->device_id = d->id;
    snprintf(a->device_name, sizeof(a->device_name), "%s", d->name);
    a->type = d->type;
    a->value = device_value(d);
    snprintf(a->range, sizeof(a->range), "%s", d->alert_range);
    snprintf(a->reason, sizeof(a->reason), "%s", d->alert_reason);
    a->callback = d->alert_callback;
    if (strcmp(d->alert_callback, "sensor_drift_detector") == 0) m->drift_alerts++;
    else m->alerts_by_type[d->type]++;
}

void display_alerts(Monitor *m) {
    if (m->alert_count == 0) { printf("No alerts detected yet.\n"); return; }
    printf("\n=============== DETECTED ALERTS (%d) ===============\n", m->alert_count);
    for (const Alert *a = m->alerts; a < m->alerts + m->alert_count; a++) {
        printf("Alert %d\n", (int)(a - m->alerts) + 1);
        printf("  Device         : %d - %s (%s)\n", a->device_id, a->device_name, type_name(a->type));
        printf("  Reading        : ");
        printf(a->type == TYPE_PRESSURE ? "%.0f %s\n" : "%.2f %s\n", a->value, type_unit(a->type));
        printf("  Expected range : %s\n", a->range);
        printf("  Reason         : %s\n", a->reason);
        printf("  Processed by   : %s()\n", a->callback);
    }
    printf("====================================================\n");
}

void monitoring_summary(Monitor *m) {
    int in_alert = 0;
    for (const Device *d = m->devices; d < m->devices + m->count; d++)
        if (d->status == STATUS_ALERT) in_alert++;

    printf("\n============ MONITORING SUMMARY ============\n");
    printf("Registered devices        : %d (%d currently in ALERT)\n", m->count, in_alert);
    printf("Total readings processed  : %ld\n", m->readings_total);
    printf("Normal readings           : %ld\n", m->readings_normal);
    printf("Abnormal readings         : %ld\n", m->readings_abnormal);
    printf("Alerts per device type (limit callbacks):\n");
    for (int t = 0; t < TYPE_COUNT; t++)
        printf("  %-12s : %d\n", type_name(t), m->alerts_by_type[t]);
    printf("Custom callback (sensor_drift_detector):\n");
    printf("  Drift alerts raised     : %d\n", m->drift_alerts);
    if (m->max_drift > 0)
        printf("  Largest drift seen      : %.2f (device %d - %s)\n", m->max_drift, m->max_drift_id, m->max_drift_name);
    else
        printf("  Largest drift seen      : none yet\n");
    printf("============================================\n");
}

void terminate_program(Monitor *m) {
    printf("Exiting the program. Goodbye!\n");
    m->running = 0;
}