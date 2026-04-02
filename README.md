# Line Follower Bot (Tuk-Tuk Bot)
**Arduino Nano (ATmega328P) | PlatformIO**

---

## Overview:
This project implements an autonomous multi-stage line follower robot capable of navigating a complex track with curves, junctions, edges, and geometric patterns. 

The system utilizes **4 IR sensors** and a **Finite State Machine (FSM) using millis() func** to dynamically adapt its behavior across different sections of the track, moving beyond simple line following into intelligent path navigation.

### Features:
* **Dual-mode Tracking:** Support for both white and black line following.
* **Intelligent Logic:** Junction detection and automatic section transitions.
* **Edge Following:** Specialized algorithms for both left and right edge tracking.
* **Shape Navigation:** Pre-programmed logic for square and triangle paths.
* **Precision Control:** Smooth motion using a **PD-based** (Proportional-Derivative) control system.
* **Optimized Performance:** Section-specific logic tuning for maximum efficiency.

---

## Hardware:
* **Microcontroller:** Arduino Nano (ATmega328P)
* **Sensors:** 4 × IR Sensors (Far Left, Mid Left, Mid Right, Far Right)
* **Driver:** Motor Driver (L298N)
* **Actuators:** 2 × DC Motors
* **Power:** Robot chassis with dedicated power supply
* **Miscellaneous:** 1 caster wheel for tuk-tuk aesthetic and 2 soldered IR proximity sensors for obstacle detection, not connected to arduino

### Pin Configuration:
|    **Component**     |      Pin       |
|    **---------**     |     -----      |
| **IR Sensors**       | D2, D3, D4, D5 |
| **Motor Driver ENA** | D6             |
| **Motor IN1, IN2**   | D7, D8         |
| **Motor IN3, IN4**   | D9, D10        |
| **Motor Driver ENB** | D11            |

---

## Working Principle:
The robot continuously polls data from the sensor array to determine its position relative to the line.

* **White Surface:** Detected as `LOW`
* **Black Line:** Detected as `HIGH`

### Control Logic:
Based on sensor input, a **positional error** is calculated. This error is fed into a PD control loop to adjust motor PWM values, ensuring the robot doesn't "jitter" and maintains a smooth trajectory. Junctions are identified when multiple sensors trigger simultaneously, signaling the FSM to transition to the next logic block.

---

## State Machine & Track Sections:
The robot navigates via a sequential State Machine. Each state is tuned for the specific geometry or input type (black/white/edge line) of that section.

---

## Getting Started
1.  **Environment:** Open the project in **PlatformIO**.
2.  **Board and Port:** Select `Arduino Nano (ATmega328P)` as board and the corressponding port.
3.  **Deploy:** Build and upload the firmware.
4.  **Setup:** Place the robot at the starting line and power on.

> [!IMPORTANT]
> Proper sensor calibration is required. Performance may vary based on ambient lighting and the contrast ratio of the track surface.

---

## Authors:
**Bhagabanta Giri** *B.Tech, Biomedical Engineering* **NIT Rourkela**
**Ashutosh Kar** *B.Tech, Metallurgy and Materials Engineering* **NIT Rourkela**
**Sourav Sharma** *B.Tech, Biomedical Engineering* **NIT Rourkela**
**Swagat Panda** *B.Tech, Food-process Engineering* **NIT Rourkela**
**Ronit Kumar Patra** *B.Tech, Ceramic Engineering* **NIT Rourkela**
