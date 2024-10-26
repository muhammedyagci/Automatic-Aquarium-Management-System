
# Automatic Aquarium Management System

This project aims to automate water change and lighting control for an aquarium. The system performs tasks such as water pumping at specific times, monitoring water flow, controlling light on/off cycles, and feeding. Additionally, it issues alerts if the expected water flow is not met. The project utilizes components including the Virtuabotix RTC module, water flow sensor, relays, LED, buzzer, and a step motor.

## Features

- **Water Change Management**: Operates the water pump at designated times to ensure fresh water supply in the aquarium.
- **Lighting Management**: Controls the aquarium lighting on a schedule suitable for plant growth.
- **Automatic Feeding**: Automates fish feeding at specific times.
- **Water Flow Monitoring and Alert System**: Issues an alert if the water flow falls below a certain threshold, ensuring proper water circulation.
- **Manual Control Button**: Includes a button for manual control as needed.

## Required Components

- Arduino Uno
- Virtuabotix RTC module (DS1302)
- Water flow sensor
- Relay modules
- LED and buzzer
- Step motor
- Various resistors and connecting wires

## Installation

1. **Hardware Setup**:
   - Connect the Virtuabotix RTC module to pins 6, 7, and 8.
   - Set up the water flow sensor on digital pin 2.
   - Attach the relays to control lighting and the water pump.
   - Connect the step motor to pins 9, 10, 11, and 12.
   - Connect the LED and buzzer for alert signals.

2. **Software Setup**:
   - Install the Arduino IDE.
   - Include the `virtuabotixRTC` library in the Arduino IDE for RTC support.
   - Upload the code to the Arduino board.

## Code Overview

The code manages the following functions:

- **Real-Time Clock (RTC) Management**: Keeps track of current time to automate tasks based on specified hours and minutes.
- **Water Flow Monitoring**: Measures water flow rate using a sensor and calculates the amount of water pumped. If flow is below 10 liters within 10 minutes, an alert is triggered.
- **Lighting Schedule**: Turns the aquarium light on or off at preset times throughout the day, accommodating plant needs.
- **Feeding Schedule**: Operates a step motor to release food at specific times during the day.
- **Alert System**: Activates a buzzer and LED alert if water flow is below the threshold during a pumping session.

## Usage

- **Automatic Mode**: The system will automatically operate based on the preset schedules coded into the program.
- **Manual Control**: Pressing the manual control button will trigger immediate motor operation for feeding or other purposes as coded.

## Code Details

The main sections in the code include:

1. **Setup**:
   - Sets up initial time on the RTC module.
   - Configures all pins and initializes sensor values.
   - Sets up interrupts for pulse counting in water flow measurement.

2. **Loop**:
   - Continuously monitors the current time.
   - Triggers actions such as feeding, water pumping, and lighting based on specific times.
   - Checks for water flow and raises an alert if flow is insufficient.

3. **Functions**:
   - `pulseCounter()`: Handles water flow counting using the sensor.
   - `rotateClockwise()`: Controls the step motor rotation for feeding purposes.

## Customization

To modify the schedule for any task:
- Adjust the hour and minute values in the loop section for each action (lighting, feeding, pumping).

## License

This project is open-source and licensed under the MIT License. Feel free to modify and distribute.
