# STM32-Based Step Counter

A straightforward step-counting application running on an STM32 microcontroller. It reads accelerometer data, applies filtering and peak detection, and updates a display and LEDs to show step count and progress toward a goal.

---

## Table of Contents
1. [Project Overview](#project-overview)  
2. [Hardware](#hardware)  
3. [Software Architecture](#software-architecture)  
4. [Module Breakdown](#module-breakdown)  
5. [Build Instructions](#build-instructions)  
---

## Project Overview
This project implements a real‐time step counter on an STM32 MCU. It samples a 3-axis accelerometer (LSM6DS), filters the data, computes acceleration magnitude and variance, detects peaks (steps), and displays the count on an LCD. LEDs indicate goal progress, and a buzzer provides feedback when milestones are reached. 

---

## Hardware
- **Microcontroller:** STM32 (e.g., STM32F4 series) running at 12 MHz SysTick timer  
- **3-Axis Accelerometer:** LSM6DS (I²C or SPI interface)  
- **Display:** Character or graphical LCD (driven via parallel/SPI)  
- **RGB LED(s):** Connected to PWM channels for progress indication  
- **Buzzer:** Simple piezo buzzer driven by PWM or GPIO  
- **Joystick & Potentiometer:** Connected to ADC channels for setting goals, navigating menu  
- **Buttons:** Debounced GPIO inputs for mode switches or manual resets  

---

## Software Architecture
The firmware uses a time-driven interrupt scheduler (SysTick) to trigger tasks at fixed rates. All heavy calculation (filtering, variance, peak detection) happens outside of interrupts, inside scheduled tasks. Global state is managed through “getter” functions to keep modules decoupled.

- **Kernel:**  
  - **SysTick ISR**: Increments a tick counter (2.6 µs each run).  
  - **Task Scheduler**: Checks tick count to execute each task at its required period.

- **Tasks & Their Rates**  
  | Task            | Frequency | Period (µs) | Measured Time (µs) |
  |-----------------|-----------|-------------|--------------------|
  | SysTick ISR     | N/A       | N/A         | 2.6                |
  | Read IMU        | 100 Hz    | 1 000       | 5.8                |
  | Buttons         | 100 Hz    | 1 000       | 3.0                |
  | Joystick        | 8 Hz      | 125 000     | 1.9                |
  | READ_ADC        | 8 Hz      | 125 000     | 1.6                |
  | LEDs            | 4 Hz      | 250 000     | 2.0                |
  | Display         | 4 Hz      | 250 000     | 10.6               |
  | Buzzer          | 1 Hz      | 1 000 000   | 3.5                |
  | **Total**       | —         | —           | **31.0**           |

Because all tasks combined (31 µs) finish well within the fastest required period (1 000 µs), there is ample headroom.

---

## Module Breakdown
1. **Read IMU (`task_read_imu.c` / `task_read_imu.h`)**  
   - Reads raw X/Y/Z from LSM6DS.  
   - Applies offset calibration.  
   - Passes data to `filter` module.  

2. **Filter (`filter.c` / `filter.h`)**  
   - Implements an Exponential Moving Average (EMA) on each axis:  
     ```c
     x_filtered += (new_x – x_filtered) >> ALPHA_SHIFT;
     ```  
   - Computes instantaneous acceleration magnitude:  
     `magnitude = \sqrt{X^2 + Y^2 + Z^2}`
     
   - Maintains a running buffer to compute mean and variance:  
     `variance = E[\text{mag}^2] - (E[\text{mag}])^2`

3. **Peak Detection (`peak_detection.c` / `peak_detection.h`)**  
   - Uses variance threshold (`Nvar_threshold`) to decide if current reading qualifies as a potential step.  
   - Debounces peaks over `debounce_samples`.  
   - Once a valid peak is confirmed, calls `state_machine_StepIncrement()` to update the count.

4. **State Machine (`state_machine.c` / `state_machine.h`)**  
   - Manages current step count, goal count, and overall state (e.g., Idle, Counting, GoalReached).  
   - Provides getters for `StepCount`, `Goal`, and goal progress.  
   - Interfaces with LEDs and Buzzer modules when milestones are reached (e.g., 25%, 50%, 75%, 100% of goal).

5. **LED Module (`leds.c` / `leds.h`)**  
   - Reads `goal_progress` (0–1000 scale).  
   - Controls RGB LED output via PWM:  
     - 0–250 → only left LED PWM duty = `goal_progress`.  
     - 251–500 → left LED ON.  
     - 501–750 → left + down LEDs ON.  
     - 751–1000 → left + down + right LEDs ON.

6. **Display Module (`display.c` / `display.h`)**  
   - Draws text (step count, goal, progress percentage) on the LCD.  
   - Refresh rate = 4 Hz (every 250 ms).  
   - Uses SPI or parallel interface (depending on your display) and standard HAL drivers.

7. **Buzzer Module (`buzzer.c` / `buzzer.h`)**  
   - Generates a short beep when:  
     - A valid step is detected (optional).  
     - Goal is reached (mandatory).  
   - Uses a GPIO or PWM channel for tone generation (e.g., 2 kHz for 100 ms).

8. **Button & Joystick Module (`input.c` / `input.h`)**  
   - Debounces two buttons (mode switch, reset).  
   - Reads Joystick X/Y via ADC to set or adjust goal.  
   - Provides functions:  
     ```c
     bool buttons_Pressed(void);
     uint16_t joystick_X(void);
     uint16_t joystick_Y(void);
     ```

9. **ADC Reader (`adc.c` / `adc.h`)**  
   - Configures ADC channels for Joystick X/Y and Potentiometer (if used).  
   - Reads raw ADC values at 8 Hz.

---

## Build Instructions
1. **Prerequisites**  
   - STM32CubeMX (optional, for auto-generating peripheral initialization).  
   - HAL drivers for your specific STM32 MCU. 

2. **Clone Repository**  
   ```bash
   git clone https://github.com/recoveringNIHILIST/StepCounter
   cd StepCounter
