#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STUDENT_ID_HEADER "00001"            /* <-- put your own student ID here */

/* ---- device types (index into the callback table) ---- */
enum { TYPE_TEMPERATURE = 0, TYPE_PRESSURE = 1, TYPE_BATTERY = 2, TYPE_COUNT = 3 };
enum { STATUS_NORMAL = 0, STATUS_ALERT = 1 };

/* ---- documented limits (all configurable here) ---------------------------
   GEN  = range the simulator generates values in
   OK   = normal operating range; outside it the reading is an ALERT
   STEP = largest random change between two consecutive readings
   DRIFT= change between consecutive readings that the custom callback flags */
#define TEMP_GEN_MIN     5.0f   /* degrees C */
#define TEMP_GEN_MAX    55.0f
#define TEMP_OK_MIN     15.0f
#define TEMP_OK_MAX     45.0f
#define TEMP_STEP        6.0f
#define TEMP_DRIFT      10.0f

#define PRES_GEN_MIN    920       /* hPa */
#define PRES_GEN_MAX   1080
#define PRES_OK_MIN     950
#define PRES_OK_MAX    1050
#define PRES_STEP        30
#define PRES_DRIFT       60

#define VOLT_GEN_MIN     3.0f     /* volts */
#define VOLT_GEN_MAX     4.4f
#define VOLT_OK_MIN      3.3f
#define VOLT_OK_MAX      4.2f
#define VOLT_STEP        0.15f
#define VOLT_DRIFT       0.40f

#define SPIKE_PERCENT    25       /* chance a reading jumps anywhere in GEN range */
#define INITIAL_CAPACITY 4
#define NAME_MAX_LEN     40

struct Device {
    int  id;
    char name[NAME_MAX_LEN];
    int  type;
    union {                       /* only the member matching `type` is valid */
        float temperature;
        int   pressure;
        float voltage;
    } readings;
    int  status;                  /* STATUS_NORMAL or STATUS_ALERT */

    /* bookkeeping used by the monitors */
    int   has_reading;            /* 0 until the first reading is generated */
    float prev_value;             /* previous reading (for drift detection) */
    int   has_prev;
    float last_drift;             /* |current - previous| of the latest reading */
    const char *alert_callback;   /* set by a callback that raised an alert */
    char  alert_range[64];
    char  alert_reason[96];
};
typedef struct Device Device;

/* callback type: receives the device to evaluate */
typedef void (*Callback)(struct Device *);

/* one stored alert (copies the data, so removing a device can't invalidate it) */
typedef struct Alert {
    int   device_id;
    char  device_name[NAME_MAX_LEN];
    int   type;
    float value;
    char  range[64];
    char  reason[96];
    const char *callback;
} Alert;

/* the whole monitoring system, passed to every menu action */
typedef struct Monitor {
    Device *devices;
    int     count, capacity;
    Alert  *alerts;
    int     alert_count, alert_cap;
    long    readings_total, readings_normal, readings_abnormal;
    int     alerts_by_type[TYPE_COUNT];
    int     drift_alerts;             /* custom callback results */
    float   max_drift;
    int     max_drift_id;
    char    max_drift_name[NAME_MAX_LEN];
    int     running;
} Monitor;

typedef void (*MenuAction)(Monitor *);

/* input.c */
void read_line(const char *prompt, char *buf, size_t size);
int  read_int(const char *prompt, int *out);

/* devices.c */
const char *type_name(int type);
const char *type_unit(int type);
float device_value(const Device *d);
void  set_device_value(Device *d, float v);
int   find_device_index(const Monitor *m, int id);
int   add_device(Monitor *m, int id, const char *name, int type);
void  register_device(Monitor *m);
void  display_devices(Monitor *m);
void  remove_device(Monitor *m);

/* callbacks.c */
void temperature_monitor(Device *d);
void pressure_monitor(Device *d);
void battery_monitor(Device *d);
void sensor_drift_detector(Device *d);
void raise_alert(Device *d, const char *cb, const char *range, const char *reason);
Callback select_callback(int type);
const char *callback_name(Callback cb);
void process_device(struct Device *device, void (*callback)(struct Device *));

/* simulation.c */
void generate_reading(Device *d);
void simulate_readings(Monitor *m);

/* report.c */
void log_alert(Monitor *m, const Device *d);
void display_alerts(Monitor *m);
void monitoring_summary(Monitor *m);
void terminate_program(Monitor *m);

#endif /* MAIN_H */