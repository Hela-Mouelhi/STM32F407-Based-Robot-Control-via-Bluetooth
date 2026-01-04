# STM32F407-Based-Robot-Control-via-Bluetooth

📌 Overview
This project implements a Bluetooth-controlled robot with analog sensor feedback using the STM32F407 microcontroller with bare-metal programming (no HAL/LL libraries). The firmware is developed in Keil MDK.

https://github.com/user-attachments/assets/3b251a83-b700-46ea-8428-853f37613961


## 🔑 Key Technologies

- **Microcontroller:** STM32F407 (ARM Cortex-M4 @ 168 MHz)
- **Communication:** Bluetooth SPP (Serial Port Profile)
- **Motor Control:** 4-channel PWM using L298N motor driver
- **Sensors:** 3-channel ADC for analog sensor acquisition
- **Control Architecture:** Interrupt-based event handling
- **Communication Protocol:** Simple text-based command system

---

## ✨ Features

### 🎮 Control Features
- **Wireless Bluetooth Control:** Real-time motor control via smartphone or PC
- **Variable Speed Control:** Incremental speed adjustment (±100 PWM units)
- **Direction Control:** Forward, backward, left, right, and stop commands
- **Smooth Acceleration:** Gradual speed changes to protect motors and improve stability

---

### 🔧 Technical Features
- **4-Channel PWM:** Independent control of two DC motors
- **3-Channel ADC:** Simultaneous monitoring of analog sensors
- **Interrupt-Driven Design:** Efficient CPU usage and fast response
- **Priority-Based Interrupts:** Configurable interrupt hierarchy
- **Real-Time Feedback:** Sensor data transmitted via Bluetooth
- **Physical Start Button:** External interrupt-based start/reset control

---

### 📊 Monitoring Features
- **Real-Time ADC Values:** Live monitoring of three analog sensor inputs
- **Command Echo:** Bluetooth command acknowledgment for reliability
- **Error Handling:** Buffer overflow protection for received commands
- **Safe Motor Control:** Boundary checks to prevent invalid PWM values

---

## 🛠 Hardware Requirements

### Essential Components

| Component                     | Quantity | Description                          |
|--------------------------------|----------|--------------------------------------|
| STM32F407 Discovery Board      | 1        | Main microcontroller                 |
| HC-05 / HC-06 Bluetooth Module | 1        | Wireless communication               |
| L298N Motor Driver             | 1        | Dual H-bridge motor controller       |
| DC Motors (12V)                | 2        | Robot drive motors                   |
| 7–12V Power Supply             | 1        | Motor power source                   |
| 3.3V / 5V Voltage Regulator    | 1        | Logic power supply                   |
| Push Button                    | 1        | Start / reset button                 |
| Analog Sensors                 | 3        | potentiometer  |

---

# 🏗️ Installation & Setup

## Prerequisites

### Software Requirements
- **Keil uVision**  
- **STM32CubeMX** for pin configuration  
- **ST-Link Utility** for programming  
- **Serial Terminal**: Putty
- **ARM GCC Toolchain** (if using a custom build system)  

### Bluetooth Apps for Smartphone Control
- **Android**: Serial Bluetooth Terminal, Blueterm  
- **Windows**: Tera Term, Putty with Bluetooth SPP




