# STM32 Mini Projects (STM32F103C8T6 / Blue Pill)

This repository contains a collection of **STM32 mini-projects** (primarily for **STM32F103C8T6 – Blue Pill**) created with **STM32CubeIDE**.

The goal is to progressively practice the basics (GPIO, interrupts, timers, PWM, UART, ADC, I2C…) with examples **with HAL** and **without HAL** (register).

## Contents

Each folder corresponds to an independent mini-project:

- `00_turn_on_pc13_with_hal`: Turn on the PC13 LED using HAL
- `01_turn_on_pc13_without_hal`: Turn on the PC13 LED without HAL (register)
- `02_turn_on_ledbutton_with_hal`: LED + button with HAL
- `03_turn_on_ledbutton_without_hal`: LED + button without HAL
- `04_blink_led_with_timer_with_hal`: Blink via timer with HAL
- `05_blink_led_with_timer_without_hal`: Blink via timer without HAL
- `06_brightness_control_timer_with_hal`: Brightness control (PWM/timer) with HAL
- `07_brightness_control_timer_without_hal`: Brightness control (PWM/timer) without HAL
- `08_measure_PWM_signal_with_hal`: Measure a PWM signal with HAL
- `09_measure_PWM_signal_without_hal`: Measure a PWM signal without HAL
- `10_UART_sending_with_hal`: Send UART with HAL
- `11_UART_sending_without_hal`: Send UART without HAL
- `12_UART_PC_communication_without_hal`: UART communication with PC without HAL
- `13_ADC_with_HAL_with_UART`: Read ADC with HAL + send UART
- `14_ADC_without_HAL_with_UART`: Read ADC without HAL + send UART
- `15_I2C_with_HAL`: I2C with HAL
- `16_I2C_without_HAL`: I2C without HAL
- `17_TIMER_INPUT_CAPTURE_MODE`: Timer Input Capture (frequency/duty cycle measurement)

## Prerequisites

- **STM32CubeIDE** (tested with 1.19.0)
- Board: **STM32F103C8T6 (Blue Pill)** or equivalent
- Programmer: **ST-LINK** (or other compatible method)
- Cables and optionally:

- External LED + resistor

- Push button

- Potentiometer (ADC)

- An I2C device (depending on your project)

## Opening a project in STM32CubeIDE

1. Open STM32CubeIDE

2. `File > Import...`
3. `General > Existing Projects into Workspace`
4. Select the mini-project folder (e.g., `10_UART_sending_with_hal`)
5. Build + Flash on the board

## Notes

- The build directories (`Debug/`, `Release/`, etc.) must be ignored via `.gitignore`.

- Projects "without HAL" use registry access to better understand the STM32 hardware.
