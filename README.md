# Soldering Station

DIY electronic soldering station project containing the hardware design, firmware, simulation files, and mechanical/laser-cut CAD files.

## Project Overview

This repository contains multiple development versions of a temperature-controlled soldering station:

- **LCD / digital soldering station** — PIC-based controller with temperature sensing, set-temperature input, PWM heater control, PI/PID-style control, UART, I²C, and display support.
- **Analog version** — earlier/full analog control design and PCB.
- **CAD / laser-cut parts** — enclosure and mechanical parts used for the station.
- **Simulation files** — Proteus project files for development and testing.

## Repository Structure

```text
soldering_station/
├── Full_anlog_uc38/
│   ├── circut/        # KiCad PCB/schematic
│   └── layout/        # Proteus/simulation and related files
├── lcd_ST/
│   ├── circut/        # KiCad schematic and PCB
│   └── firmware/
│       └── st_16f15223_irsegmet.X/
│           ├── main.c
│           ├── adc.c / adc.h
│           ├── display.c / display.h
│           ├── i2c1.c / i2c1.h
│           ├── pid.c / pid.h
│           ├── pwm.c / pwm.h
│           ├── timer_1ms.c / timer_1ms.h
│           ├── uart.c / uart.h
│           └── pin_deffiine.h
└── laser_cut_cad/
    └── *.lbrn2       # LightBurn CAD files
```

## LCD Soldering Station Firmware

The current firmware is written in C for a Microchip PIC16F1-series MCU and is developed with **MPLAB X / XC8**.

### Main firmware features

- 32 MHz internal oscillator
- Temperature ADC measurement
- Set-temperature ADC input
- 50-sample moving average
- 200 ms temperature-control period
- PWM heater control
- PI/PID-style temperature control
- Integral anti-windup
- 1 ms timer scheduling
- UART at 9600 baud
- I²C interface
- Numeric display
- Automatic display of the set temperature after a significant setpoint change

The main control loop collects ADC temperature samples into a 50-element buffer and calculates the average before updating the heater duty cycle.

### Current control parameters

```c
#define AVG_SAMPLES       50
#define PID_PERIOD_MS     200

#define S  20
#define KI 1
#define KP 5
```

PWM duty is limited to:

```text
0 ... 1000
```

## Pin Assignment

The current firmware pin map is documented in `pin_deffiine.h`.

| MCU Pin | Function |
|---|---|
| RA5 | Temperature ADC |
| RA4 | Temperature-set ADC |
| RC5 | Current sensing |
| RC4 | Heater PWM gate |
| RC2 | I²C SCL |
| RC1 | I²C SDA |
| RC0 | UART TX |
| RA2 | UART RX |
| RA1 | ICSPDAT |
| RA0 | ICSPCLK |

> Verify the exact MCU part number against the schematic and datasheet before building. Some existing firmware comments/configuration reference different PIC16F1522x/15225 variants.

## Firmware Build

Open:

```text
lcd_ST/firmware/st_16f15223_irsegmet.X
```

Development environment:

- MPLAB X IDE
- XC8 compiler
- PIC16F1522x device support package
- PICkit/programmer compatible with the selected MCU

The project contains the MPLAB X `nbproject` configuration and source files.

## Hardware Design

### LCD version

Located under:

```text
lcd_ST/circut/lcd_soldring_staion/
```

Main files:

- `lcd_soldring_staion.kicad_sch`
- `lcd_soldring_staion.kicad_pcb`

### Analog version

Located under:

```text
Full_anlog_uc38/circut/solder_staion/
```

Main files:

- `solder_staion.kicad_sch`
- `solder_staion.kicad_pcb`

## Simulation

Proteus simulation/development files are located in:

```text
Full_anlog_uc38/layout/
```

## Mechanical / Laser-Cut Design

Laser-cut enclosure/mechanical files are located in:

```text
laser_cut_cad/
```

The `.lbrn2` files can be opened with **LightBurn**.

## Development Status

This is an active DIY electronics project. Hardware, firmware, control parameters, and mechanical designs are still being developed and revised.

Current development includes:

- Temperature-control stability
- PI/PID tuning
- Temperature filtering
- Current sensing
- LCD/display interface
- PCB and enclosure development

## Safety

A soldering station contains a high-temperature heating element and may contain hazardous mains voltages depending on the power-supply design.

Use appropriate electrical isolation, fusing, grounding, thermal protection, over-temperature protection, enclosure, and strain relief.

Do not operate the heater unattended.

## License

No license has currently been specified for this repository. Unless a license is added, normal copyright restrictions apply to the repository contents.
