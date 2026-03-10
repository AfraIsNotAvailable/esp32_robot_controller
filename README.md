# ESP32 Mecanum Robot Controller

## Overview

This repository contains the source code for a real-time, highly modular embedded control system designed for a four-wheel Mecanum drive robot. Built around the ESP32 microcontroller, the project leverages the Arduino framework (backed by ESP-IDF and FreeRTOS) to provide deterministic control loops, hardware peripheral abstraction, and extensible subsystem management.

The firmware is compiled using C++17 and managed via PlatformIO, structured to support complex robotics applications that require strict timing guarantees and clean architectural boundaries.

## System Architecture

The software architecture relies on object-oriented design patterns to separate hardware interaction, task scheduling, and kinematic control.

### 1. Deterministic Task Scheduling (`Looper` and `Loop`)

To maintain system stability and accurate control calculations, the firmware utilizes a custom scheduling framework rather than relying on the standard Arduino `loop()`.

* **FreeRTOS Integration:** The `Looper` class creates an RTOS task pinned to a specific core using `xTaskCreatePinnedToCore`.
* **Strict Timing:** It utilizes `vTaskDelayUntil` to guarantee absolute execution frequencies (configured by default to 50Hz / 20ms periods).
* **Polymorphic Loops:** Individual components implement the `Loop` interface (`onStart`, `onLoop`, `onStop`), allowing the scheduler to iterate through registered control tasks deterministically.

### 2. Subsystem Abstraction (`Subsystem`)

Hardware components are abstracted using the `Subsystem` base class. This enforces a strict operational lifecycle:

* `readPeriodicInputs()`: Samples sensors and reads hardware registers at the beginning of a cycle.
* `writePeriodicOutputs()`: Commits calculated demands to the hardware actuators at the end of a cycle.
* This separation of inputs, control calculation (handled in the `Loop`), and outputs prevents race conditions, minimizes bus contention, and simplifies off-target unit testing.

### 3. Kinematic Control (`Drive`)

The drivetrain is managed by the `Drive` Singleton.

* **Mecanum Kinematics:** Translates desired Cartesian velocity commands ($v_x$, $v_y$, $\omega$) into normalized individual wheel speeds.
* **Hardware PWM Abstraction:** Maps the calculated wheel speeds to hardware timer signals. It utilizes the ESP32's internal `ledc` API rather than standard software PWM to generate clean, high-resolution (8-bit, 20kHz) hardware PWM signals.

## Hardware Configuration

The system is configured for an ESP32 Development Board (e.g., `esp32doit-devkit-v1`). Pin assignments and hardware constraints are centralized in `src/Constants.h`.

**Motor Driver Pinout:**

* **Front Left (FL):** PWM: GPIO 15 | DIR: GPIO 2
* **Front Right (FR):** PWM: GPIO 0 | DIR: GPIO 4
* **Back Left (BL):** PWM: GPIO 16 | DIR: GPIO 17
* **Back Right (BR):** PWM: GPIO 5 | DIR: GPIO 18

**PWM Configuration:**

* Base Frequency: 20 kHz
* Resolution: 8-bit (0-255)

## Development and Build Environment

This project requires [PlatformIO](https://platformio.org/) for dependency management and compilation.

**Toolchain Details:**

* **Platform:** Espressif 32
* **Framework:** Arduino (Core v3.0.7 / IDF Release v5.1)
* **C++ Standard:** C++17 (`-std=gnu++17`)

### Build Instructions

1. Clone the repository.
2. Open the project folder in VSCode with the PlatformIO extension installed, or utilize the PlatformIO Core CLI.
3. Build the firmware:
```bash
pio run

```


4. Upload to your target ESP32 board:
```bash
pio run --target upload

```



## Repository Structure

* `include/` - Contains header files (`.h`) declaring the system's APIs, abstract classes, and configuration parameters.
* `src/` - Contains the implementation files (`.cpp`) for the robot's subsystems and the main application entry point.
* `lib/` - Reserved for isolated, project-specific private libraries.
* `test/` - Reserved for unit tests utilizing the PlatformIO Test Runner.
* `platformio.ini` - Build configuration, toolchain overrides, and platform constraints.
