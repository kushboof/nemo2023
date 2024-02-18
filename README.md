# Autonomous Rover Code

## Overview
This repository contains the code for controlling our custom-designed rover. The rover is equipped with various sensors and a camera to control its navigation and commands related to object detection.

## Features
- Utilizes three infrared sensors positioned at the base to aid in steering the vehicle.
- Incorporates a Pixy2 camera for detecting objects based on specific color, width, and height criteria.
- Implements functionality for the rover to autonomously follow and grab objects using its gripper claw.

## Requirements
- Arduino IDE (or compatible development environment)
- Libraries for infrared sensors and Pixy2 camera (if not already included)

## Usage
1. Connect the rover to the development environment.
2. Upload the code to the rover's microcontroller.
3. Ensure all sensors and the camera are properly calibrated and connected.
4. Power on the rover and initiate the control program.
5. Monitor the rover's behavior and adjust parameters as necessary.

## License
This project is licensed under the [MIT License](LICENSE).
