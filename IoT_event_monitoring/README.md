# Dynamic IoT Event Monitoring and Callback Engine

A simulation of an IoT monitoring system written in C. Devices are registered dynamically, generate readings, and each reading is processed by a callback chosen with a function pointer. Abnormal readings raise alerts that are stored and reported.

## Files

| File | Purpose |
|------|---------|
| `main.h` | Shared header: `Device` (with union), `Alert`, `Monitor`, limits and prototypes |
| `main.c` | Header output, starting devices, menu and the function-pointer dispatch table |
| `input.c` | Safe input helpers |
| `devices.c` | Register, display and remove devices; union access by device type |
| `callbacks.c` | Type callbacks, the custom drift detector, callback selection and `process_device` |
| `simulation.c` | Reading generation and the processing loop |
| `report.c` | Alert log, alert display and monitoring summary |
| `sample_output.txt` | A complete sample run (40 readings) |

## Features

- `struct Device` holds a `union` with the temperature, pressure or voltage reading; the device type decides which member is valid and the code never reads another member.
- The device array starts at 4 slots, grows with `realloc` and shrinks when less than half is used. Duplicate IDs are rejected and all allocations are checked.
- Six starting devices (two per type) are registered at start-up; more can be added or removed from the menu.
- Readings are generated with `rand()`, seeded from the clock (or from a seed given on the command line).
- Every alert records the device, value and unit, expected range, reason and the callback that raised it.
- All memory (devices and the alert log) is freed before exit.

## Menu

```
1. Register a new device
2. Display all devices
3. Remove a device
4. Generate and process readings
5. Display detected alerts
6. Monitoring summary
7. Exit
```

## Build and run

```bash
bash compiler.sh
./iot_monitor          # random seed from the clock
./iot_monitor 2        # repeatable run with seed 2
```

or directly:

```bash
gcc -Wall -Wextra -std=c11 -g main.c input.c devices.c callbacks.c simulation.c report.c -o iot_monitor
```

## Limits (all `#define`s in `main.h`)

| Type | Generated range | Normal range | Max step | Drift limit |
|------|-----------------|--------------|----------|-------------|
| Temperature | 5 to 55 C | 15 to 45 C | 6 C | 10 C |
| Pressure | 920 to 1080 hPa | 950 to 1050 hPa | 30 hPa | 60 hPa |
| Battery voltage | 3.0 to 4.4 V | 3.3 to 4.2 V | 0.15 V | 0.40 V |

A reading is the previous value plus a random step, clamped to the generated range. With 25% probability it jumps anywhere in the range, which simulates a sudden fault.

## Callback dispatch

```c
static const Callback type_callbacks[TYPE_COUNT] = {
    temperature_monitor, pressure_monitor, battery_monitor
};
```

`select_callback(type)` returns the function for the device type and `process_device(device, callback)` calls it. Each reading is processed twice: by the type callback and then by the custom callback.

## Custom callback: Sensor Drift Detector

**Purpose:** catch sudden jumps that stay inside the safe range, which the limit callbacks cannot see (a failing sensor or a sudden event).

**Algorithm:**

```
drift = |current reading - previous reading|
alert when drift > drift limit for the device type
```

**Triggering conditions:** it runs on every device type after the type callback and skips the first reading of a device (nothing to compare). It also records `last_drift`, so the summary can report the number of drift alerts and the largest drift seen.

## Sample session

```
Student ID: 00001
Project: Dynamic IoT Event Monitoring and Callback Engine
Custom Callback: Sensor Drift Detector
Random seed: 2
[memory] device array grew from 4 to 8 slots

#    ID    Name               Type         Value      Result
1    101   Boiler-Temp        Temperature  40.05 C    NORMAL
       checked by temperature_monitor + sensor_drift_detector: within limits
2    102   ColdRoom-Temp      Temperature  45.48 C    ALERT
       ! Overheating: 45.5 C is 0.5 above the limit  [by temperature_monitor]
...
18   102   ColdRoom-Temp      Temperature  20.89 C    ALERT
       ! Sensor drift: reading moved 22.15 C since last reading (43.03 -> 20.89)  [by sensor_drift_detector]

============ MONITORING SUMMARY ============
Registered devices        : 8 (1 currently in ALERT)
Total readings processed  : 40
Normal readings           : 24
Abnormal readings         : 16
Alerts per device type (limit callbacks):
  Temperature  : 2
  Pressure     : 12
  Battery      : 0
Custom callback (sensor_drift_detector):
  Drift alerts raised     : 2
  Largest drift seen      : 117.00 (device 202 - Tank-Pressure)
============================================
```

`sample_output.txt` also shows registration, a rejected duplicate ID, invalid input, removals, and the device array shrinking from 8 to 4 slots.

## Design notes

- Callbacks only receive a `Device *`, so they record the alert details inside the device; the engine then copies them into the alert log. Alerts keep their own copy of the name, so removing a device never invalidates a stored alert.
- No pointer into the device array is kept between operations, because `realloc` may move the array.
- Because the pressure range is wider around the limits, pressure alerts are the most common in the sample run; adjust the limits in `main.h` to change this.

