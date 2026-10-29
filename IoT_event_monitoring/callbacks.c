#include "main.h"

/* Marks the device as ALERT and records who raised it and why. */
void raise_alert(Device *d, const char *cb, const char *range, const char *reason) {
    d->status = STATUS_ALERT;
    d->alert_callback = cb;
    snprintf(d->alert_range, sizeof(d->alert_range), "%s", range);
    snprintf(d->alert_reason, sizeof(d->alert_reason), "%s", reason);
}

/* ---- required callbacks: one per device type -------------------------- */

void temperature_monitor(Device *d) {
    if (d->type != TYPE_TEMPERATURE) return;          /* never read a wrong union member */
    float t = d->readings.temperature;
    char range[64], reason[96];
    snprintf(range, sizeof(range), "%.1f to %.1f C", TEMP_OK_MIN, TEMP_OK_MAX);
    if (t > TEMP_OK_MAX) {
        snprintf(reason, sizeof(reason), "Overheating: %.1f C is %.1f above the limit", t, t - TEMP_OK_MAX);
        raise_alert(d, "temperature_monitor", range, reason);
    } else if (t < TEMP_OK_MIN) {
        snprintf(reason, sizeof(reason), "Too cold: %.1f C is %.1f below the limit", t, TEMP_OK_MIN - t);
        raise_alert(d, "temperature_monitor", range, reason);
    }
}

void pressure_monitor(Device *d) {
    if (d->type != TYPE_PRESSURE) return;
    int p = d->readings.pressure;
    char range[64], reason[96];
    snprintf(range, sizeof(range), "%d to %d hPa", PRES_OK_MIN, PRES_OK_MAX);
    if (p > PRES_OK_MAX) {
        snprintf(reason, sizeof(reason), "Over-pressure: %d hPa is %d above the limit", p, p - PRES_OK_MAX);
        raise_alert(d, "pressure_monitor", range, reason);
    } else if (p < PRES_OK_MIN) {
        snprintf(reason, sizeof(reason), "Under-pressure: %d hPa is %d below the limit", p, PRES_OK_MIN - p);
        raise_alert(d, "pressure_monitor", range, reason);
    }
}

void battery_monitor(Device *d) {
    if (d->type != TYPE_BATTERY) return;
    float v = d->readings.voltage;
    char range[64], reason[96];
    snprintf(range, sizeof(range), "%.2f to %.2f V", VOLT_OK_MIN, VOLT_OK_MAX);
    if (v < VOLT_OK_MIN) {
        snprintf(reason, sizeof(reason), "Battery low: %.2f V is %.2f below the minimum", v, VOLT_OK_MIN - v);
        raise_alert(d, "battery_monitor", range, reason);
    } else if (v > VOLT_OK_MAX) {
        snprintf(reason, sizeof(reason), "Over-voltage: %.2f V is %.2f above the maximum", v, v - VOLT_OK_MAX);
        raise_alert(d, "battery_monitor", range, reason);
    }
}

/*  ---- CUSTOM CALLBACK: SENSOR DRIFT DETECTOR ----------------------------
    Purpose   : catch sudden jumps that stay inside the safe range (a failing
                sensor or a sudden event) and that the limit checks cannot see.
    Operates  : on every device type, after its type-specific callback.
    Algorithm : drift = |current - previous reading|
                alert when drift > DRIFT limit for the type
                (temperature 10 C, pressure 60 hPa, battery 0.40 V).
    Skipped   : the first reading of a device (nothing to compare with).
    Result    : stores last_drift in the device; the engine counts drift
                alerts and tracks the largest drift seen.                  */
void sensor_drift_detector(Device *d) {
    float limit;
    switch (d->type) {
        case TYPE_TEMPERATURE: limit = TEMP_DRIFT; break;
        case TYPE_PRESSURE:    limit = (float)PRES_DRIFT; break;
        case TYPE_BATTERY:     limit = VOLT_DRIFT; break;
        default: return;
    }
    d->last_drift = 0.0f;
    if (!d->has_prev) return;
    float diff = device_value(d) - d->prev_value;
    if (diff < 0) diff = -diff;
    d->last_drift = diff;
    if (diff > limit) {
        char range[64], reason[96];
        snprintf(range, sizeof(range), "change of at most %.2f %s between readings", limit, type_unit(d->type));
        snprintf(reason, sizeof(reason), "Sensor drift: reading moved %.2f %s since last reading (%.2f -> %.2f)",
                 diff, type_unit(d->type), d->prev_value, device_value(d));
        raise_alert(d, "sensor_drift_detector", range, reason);
    }
}

/* ---- dispatch ----------------------------------------------------------- */

/* The function-pointer table: device type -> its callback. */
static const Callback type_callbacks[TYPE_COUNT] = {
    temperature_monitor,   /* TYPE_TEMPERATURE */
    pressure_monitor,      /* TYPE_PRESSURE    */
    battery_monitor        /* TYPE_BATTERY     */
};

Callback select_callback(int type) {
    if (type < 0 || type >= TYPE_COUNT) return NULL;
    return type_callbacks[type];
}

const char *callback_name(Callback cb) {
    if (cb == temperature_monitor)    return "temperature_monitor";
    if (cb == pressure_monitor)       return "pressure_monitor";
    if (cb == battery_monitor)        return "battery_monitor";
    if (cb == sensor_drift_detector)  return "sensor_drift_detector";
    return "unknown";
}

/* Required interface: runs the given callback on the device. */
void process_device(struct Device *device, void (*callback)(struct Device *)) {
    if (device == NULL || callback == NULL) return;
    callback(device);
}