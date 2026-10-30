#include "main.h"

static float rand_range(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / (float)RAND_MAX);
}

/*  
Generates the next reading for one device (only the active union member is written).
    First reading : uniform random value inside the type's GEN range.
    Later readings: previous value + random step (+/- STEP), clamped to the GEN range;
                    with SPIKE_PERCENT % probability the value jumps anywhere in the range.
    Also resets the status/alert fields and remembers the previous reading. 
    */
void generate_reading(Device *d) {
    float lo, hi, step;
    switch (d->type) {
        case TYPE_TEMPERATURE: lo = TEMP_GEN_MIN;        hi = TEMP_GEN_MAX;        step = TEMP_STEP; break;
        case TYPE_PRESSURE:    lo = (float)PRES_GEN_MIN; hi = (float)PRES_GEN_MAX; step = (float)PRES_STEP; break;
        case TYPE_BATTERY:     lo = VOLT_GEN_MIN;        hi = VOLT_GEN_MAX;        step = VOLT_STEP; break;
        default: return;
    }
    float v;
    if (d->has_reading) {
        d->prev_value = device_value(d);
        d->has_prev = 1;
        if (rand() % 100 < SPIKE_PERCENT) v = rand_range(lo, hi);
        else v = d->prev_value + rand_range(-step, step);
    } else {
        v = rand_range(lo, hi);
    }
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    set_device_value(d, v);
    d->has_reading = 1;
    d->status = STATUS_NORMAL;
    d->alert_callback = NULL;
    d->last_drift = 0.0f;
}

/* Menu option: every registered device produces N readings; each goes through
   its type callback and then the custom drift callback. */
void simulate_readings(Monitor *m) {
    int rounds;
    if (m->count == 0) { printf("No devices registered. Register a device first.\n"); return; }
    if (!read_int("Number of reading rounds (1-20, each device reads once per round): ", &rounds)
        || rounds < 1 || rounds > 20) {
        printf("Invalid number. Enter a value from 1 to 20.\n");
        return;
    }
    printf("\n%-4s %-5s %-18s %-12s %-10s %s\n", "#", "ID", "Name", "Type", "Value", "Result");
    printf("--------------------------------------------------------------------------------\n");

    for (int r = 0; r < rounds; r++) {
        for (Device *d = m->devices; d < m->devices + m->count; d++) {
            generate_reading(d);

            /* callback 1 = chosen by device type through the table, callback 2 = custom */
            Callback cbs[2] = { select_callback(d->type), sensor_drift_detector };
            int raised = 0;
            char reasons[2][96];
            const char *names[2];
            for (int k = 0; k < 2; k++) {
                d->alert_callback = NULL;
                process_device(d, cbs[k]);
                if (d->alert_callback != NULL) {          /* this callback raised an alert */
                    log_alert(m, d);
                    snprintf(reasons[raised], sizeof(reasons[raised]), "%s", d->alert_reason);
                    names[raised] = d->alert_callback;
                    raised++;
                }
            }

            m->readings_total++;
            if (d->last_drift > m->max_drift) {
                m->max_drift = d->last_drift;
                m->max_drift_id = d->id;
                snprintf(m->max_drift_name, sizeof(m->max_drift_name), "%s", d->name);
            }
            if (d->status == STATUS_ALERT) m->readings_abnormal++; else m->readings_normal++;

            char val[24];
            snprintf(val, sizeof(val), d->type == TYPE_PRESSURE ? "%.0f %s" : "%.2f %s",
                     device_value(d), type_unit(d->type));
            printf("%-4ld %-5d %-18.18s %-12s %-10s %s\n", m->readings_total, d->id, d->name,
                   type_name(d->type), val, raised ? "ALERT" : "NORMAL");
            if (!raised)
                printf("       checked by %s + sensor_drift_detector: within limits\n", callback_name(cbs[0]));
            for (int k = 0; k < raised; k++)
                printf("       ! %s  [by %s]\n", reasons[k], names[k]);
        }
    }
    printf("--------------------------------------------------------------------------------\n");
}