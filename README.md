# Door Locker Security System

## Overview
This project implements a **Door Locker Security System** using two ATmega32 microcontrollers operating at 8 MHz, designed with a **layered architecture** approach. The system secures access through a password mechanism, integrates EEPROM storage, a DC motor for door control, and a buzzer for alarms.  
A Proteus simulation is included to demonstrate the complete functionality.

## System Architecture
The system consists of **two ECUs**:

1. **HMI_ECU (Human Machine Interface)**
   - Components: 2x16 LCD, 4x4 Keypad
   - Function: Handles user interaction by taking password inputs and displaying system messages.

2. **Control_ECU**
   - Components: EEPROM, Buzzer, DC Motor
   - Function: Processes passwords, controls door locking/unlocking, manages alarms, and stores data in EEPROM.

---

## Features
- **Password Creation & Storage**:  
  - User sets a 5-digit password and confirms it before it is saved in EEPROM.
- **Door Control**:  
  - Unlocks door (motor CW 15s), waits 3s, then locks door (motor CCW 15s).
- **Password Change**:  
  - Requires existing password validation.
- **Security Lockout**:  
  - After 3 failed password attempts, buzzer activates for 1 minute, and inputs are locked.
- **UART Communication**:  
  - Secure data exchange between the two ECUs.
- **EEPROM Storage**:  
  - Retains password even after power loss.
- **Proteus Simulation**:  
  - Demonstrates hardware and firmware functionality.

---

## Drivers Implemented
- **GPIO Driver**: Shared by both ECUs.
- **LCD Driver**: 2x16 LCD in 4-bit or 8-bit mode (HMI_ECU).
- **Keypad Driver**: 4x4 Keypad (HMI_ECU).
- **DC Motor Driver**: PWM (Control_ECU).
- **EEPROM Driver**: I2C-based (Control_ECU).
- **I2C Driver**: Configurable with `TWI_ConfigType`.
- **UART Driver**: Configurable with `UART_ConfigType`.
- **Timer1 Driver**: Interrupt-based, supports Normal & Compare modes.
- **Buzzer Driver**: Simple ON/OFF control (Control_ECU).

---

## System Workflow
1. **Set Password** → Store in EEPROM.
2. **Main Menu**:
   - `+` Open Door
   - `-` Change Password
3. **Password Validation**:
   - 1st attempt → retry if wrong.
   - 2nd attempt → retry if wrong.
   - 3rd attempt → lockout with buzzer.
4. **Door Operation**:
   - Unlock → Pause → Lock.

---

## Project Files
- `HMI_ECU/` → Source code for the Human Machine Interface ECU.
- `Control_ECU/` → Source code for the Control ECU.
- `Proteus/` → Proteus simulation files.

---

## Tools & Technologies
- **MCU**: ATmega32 (8 MHz)
- **Language**: C
- **Simulation**: Proteus
- **Communication**: UART, I2C
- **Peripheral Control**: Timers, PWM

---

## How to Run
1. Open the **Proteus** project file from the `Proteus/` folder.
2. Load the compiled HEX files for both MCUs.
3. Run the simulation and interact with the keypad & LCD.

---

## Author
**George Essam**  
Computer Engineering Graduate, Ain Shams University (2024)

---
