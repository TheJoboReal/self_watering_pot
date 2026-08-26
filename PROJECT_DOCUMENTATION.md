# Self-Watering Pot Project

## Overview

This is a firmware project for an IoT-enabled self-watering plant system built on the Raspberry Pi Pico 2 (RP2350). The system uses fuzzy logic to determine watering needs based on soil moisture sensor readings and controls a pump motor accordingly.

## Hardware Platform

- **Microcontroller**: Raspberry Pi Pico 2 (RP2350)
- **SDK**: Pico SDK (v2.x)
- **Programming Language**: C/C++

## Hardware Components

### Core Components
- **Soil Moisture Sensor**: Connected to GPIO 26 (ADC input)
- **Water Pump**: DC motor controlled via L298N motor driver
  - PWM Control: GPIO 15
  - Direction AIN1: GPIO 13
  - Direction AIN2: GPIO 14
  - Standby/Enable: GPIO 12
- ** LED Indicator**: Built-in LED (GPIO 25)
- **Display**: WaveShare 2.9" B/V3 e-Ink display (4-color)
- **Input**: Encoder with button (events system)

## Project Structure

```
self_watering_pot/
├── main.c                      # Main application entry point
├── CMakeLists.txt              # Build configuration
├── pico_sdk_import.cmake       # Pico SDK import script
├── .gitignore
├── build/                      # Build output directory (ignored)
├── inc/                        # Header files
│   ├── defines.h               # Pin definitions and constants
│   ├── soil.h                  # Soil sensor data structures
│   ├── soil_sensor.h           # Soil moisture measurement functions
│   ├── pump.h                  # Motor/pump control functions
│   ├── controller.h            # Fuzzy logic controller
│   ├── scheduler.h             # Task scheduler implementation
│   └── events.h                # Event handling system
├── lib/                        # External libraries
│   ├── Config/                 # Hardware interface layer
│   │   ├── DEV_Config.h/c      # GPIO, SPI, delay functions
│   │   └── Debug.h
│   ├── e-Paper/                # e-Ink display drivers
│   │   ├── EPD_2in9b_V3.c/h    # 2.9" 3-color display
│   │   ├── EPD_2in9_V2.c/h     # 2.9" 2-color display
│   │   ├── EPD_2in13_V4.c/h    # 2.13" display
│   │   └── EPD_2in9d.c/h       # 2.9" display
│   ├── GUI/                    # Graphics primitives
│   │   ├── GUI_Paint.h/c
│   │   └── CMakeLists.txt
│   └── Fonts/                  # Font files and images
│       ├── font8.c, font12.c, font16.c
│       ├── font20.c, font24.c, font82.c, font94.c
│       ├── battery16.c, battery32.c
│       └── CMakeLists.txt
├── examples/
│   └── ntp_system_time/        # NTP time synchronization example
│       ├── main.c
│       ├── CMakeLists.txt
│       ├── lwipopts.h
│       └── README.md
└── no-OS-FatFS-SD-SPI-RPi-Pico/ # SD card filesystem (excluded from git)
```

## Build System

### Requirements
- CMake ≥ 3.13
- Pico SDK (fetched automatically)
- picotool (for flashing)

### Build Process

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DPICO_BOARD=pico2

# Build the project
make -j$(nproc)

# Flash to device
cp main.uf2 /path/to/rp2040
```

### Build Artifacts
- `main.elf` - ELF executable
- `main.bin` - Raw binary
- `main.hex` - Intel HEX format
- `main.uf2` - UF2 format for drag-and-drop flashing

## Core Modules

### 1. Scheduler (`scheduler.h`)
Simple run-to-completion round-robin scheduler for time-based tasks.

**Task Structure:**
```c
typedef struct {
    void (*func)(void);      // Task function
    char *name;              // Task name
    uint64_t frequency_us;   // Execution frequency (microseconds)
    uint64_t timer_us;       // Current timer value
} Task;
```

**Usage:**
- Create tasks with `CREATE_TASK(func, frequency_ms)` macro
- Initialize scheduler with `scheduler_init()`
- Poll with `scheduler_poll()` in main loop

### 2. Soil Sensor (`soil_sensor.h`)
Measures soil moisture via ADC input (GPIO 26).

**Measurement:**
- ADC range: 0-3.3V
- Conversion: 12-bit (0-4095)
- Raw voltage: `result * (3.3 / 4096)`

**Soil Moisture Interpretation:**
- 3.3V = Dry soil
- 2.35V = Wet soil
- Linear interpolation between values

### 3. Fuzzy Logic Controller (`controller.h`)
Implements fuzzy control system for watering decisions.

**Fuzzy Membership Functions:**
- **Moisture Levels:**
  - LOW: 0-40% (dry)
  - MEDIUM: 40-60% (transition)
  - HIGH: 60-80% (transition)
  - FULL: 80-100% (wet)

**Control Rules:**
- LOW moisture → HIGH water
- MEDIUM moisture → MEDIUM water
- HIGH moisture → NO water

**Output:**
- Water duration: 0-10,000ms (linear mapping)
- PWM duty: 0-1249 (100kHz PWM frequency)

### 4. Pump/Motor Control (`pump.h`)
Controls DC water pump via H-bridge motor driver.

**GPIO Configuration:**
```c
MOTOR_PWM_PIN  = GPIO 15  // PWM control
MOTOR_AIN1     = GPIO 13  // Direction control 1
MOTOR_AIN2     = GPIO 14  // Direction control 2
MOTOR_STBY     = GPIO 12  // Enable/standby
```

**PWM Configuration:**
- Frequency: 100kHz
- Wrap value: 1249
- Clock divider: 1.0

### 5. Events System (`events.h`)
Circular buffer for handling input events.

**Event Types:**
- Button press/release
- Encoder rotation
- Barcode scan

**Usage Pattern:**
- Single producer writes events to buffer
- Multiple consumers can read independently
- Each reader maintains its own read position

## Main Application Flow

```
1. Initialize ADC (GPIO 26)
2. Initialize LED (GPIO 25)
3. Initialize motor driver
4. Initialize USB serial
5. Initialize tinyusb
6. Create task schedule with:
   - LED blink (200ms)
   - Soil measurement (500ms)
   - Serial output (500ms)
   - Pump control (500ms)
7. Run scheduler loop
```

## Key Dependencies

### Pico SDK Components
- `pico_stdlib` - Standard utilities
- `hardware_pio` - PIO peripherals
- `hardware_adc` - Analog-to-digital converter
- `hardware_pwm` - Pulse width modulation
- `hardware_irq` - Interrupt handling
- `pico_time` - Time functions
- `tusb` - TinyUSB stack

### External Libraries
- WaveShare e-Paper drivers
- GUI primitives
- Font library

## Configuration

### Pin Definitions (`inc/defines.h`)
```c
ADC_PIN       = 26  // Soil moisture sensor
MOTOR_PWM_PIN = 15  // Pump PWM control
```

### Soil Calibration
Adjust calibration values in `controller.h`:
- Dry point voltage: 3.3V
- Wet point voltage: 2.35V

### Scheduler Frequencies
Tasks run at 500ms intervals (configurable in `main.c`).

## Example: NTP Time System
Located in `examples/ntp_system_time/`:

- Connects to WiFi
- Synchronizes time via NTP
- Maintains always-on timer
- Supports POSIX timezone conversion

## Development Notes

### Coding Style
- Header guards: `#ifndef NAME_H / #define NAME_H / #endif`
- No external dependencies (except SDK and included libraries)
- Simple, portable C code

### Building Custom Examples
```bash
mkdir build/examples
cd build/examples
cmake ../../examples/ntp_system_time -DPICO_BOARD=pico2
make
```

### Flashing
```bash
#.uf2 format for drag-and-drop
cp build/main.uf2 /media/$USER/RPI-RP2/

# Or use picotool
picotool load build/main.uf2
```

## TODO/Dynamic Features (from git branches)
- SD card support
- Dormant/low-power mode
- Motor testing utilities

## References
- [Pico SDK Documentation](https://raspberrypi.com/documentation/pico-sdk/)
- [WaveShare e-Paper Docs](https://www.waveshare.com/wiki/2.9inch_e-Paper_Panel)
- [RP2350 Datasheet](https://raspberrypi.com/documentation/microcontrollers/rp2350.html)
