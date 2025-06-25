# STM32F407-Based-Robot-Control-via-Bluetooth
Bare Metal for controlling a robot via Bluetooth (HC-05/HC-06) using an STM32F407 microcontroller. Built without HAL or libraries, it features UART command parsing, hardware PWM motor control, and direct GPIO manipulation for optimal performance.

<!-- Uploading "STM robot.mp4"... -->

📌 Overview
This project implements a Bluetooth-controlled robot with analog sensor feedback using the STM32F407 microcontroller with bare-metal programming (no HAL/LL libraries). The firmware is developed in Keil MDK and features:

HC-05 Bluetooth remote control (UART commands)
PWM motor control (TIM3 with 4 channels)
Multi-channel ADC sensor reading (PB4-PB6)
External interrupt (EXTI) for manual start
Timer-based ADC sampling (TIM6)

🛠 Hardware Setup
Required Components
STM32F407 Discovery Board 
HC-05/HC-06 Bluetooth Module (UART2: PA2-TX, PA3-RX)
L298N Motor Driver (or similar)
DC Motors ×2
Analog Sensors (e.g., potentiometers, IR sensors on PB4-PB6)
Push Button (PA0 for EXTI0 interrupt)

⚙️ Setup Instructions
Flash the code using Keil MDK (ARMCC compiler).
Wire the hardware as per the pinout table.
Pair HC-05 (default PIN: 1234) and send commands via a serial terminal (e.g., Mobile app).
Press PA0 button to enable motor control and ADC sampling.

🔗 Resources
STM32F407 Reference Manual
HC-05 Datasheet
Keil MDK Setup Guide




