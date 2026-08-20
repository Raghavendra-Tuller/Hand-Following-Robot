# 🤖 Hand Following Robot

This project is an Arduino-based autonomous hand-following robot.

The robot uses an ultrasonic sensor mounted on a servo motor to detect and track a person's hand. The servo scans different directions to locate the hand, while four DC motors drive the robot.

## How It Works

- The ultrasonic sensor measures the distance between the robot and the hand.
- The servo rotates the ultrasonic sensor to scan different directions.
- When the hand is detected within the following range, the robot moves toward it.
- When the hand is approximately 8–12 cm away, the robot stops.
- If the hand comes closer than 5 cm, the robot moves backward.
- If the hand moves away or is not detected in front, the servo scans left and right to find it.
- The robot turns toward the detected hand and continues following it.
- A buzzer provides an indication when the hand is too close.

## Components Used

- Arduino
- L293D/L298-based motor driver
- 4 DC geared motors
- 4 wheels
- HC-SR04 ultrasonic sensor
- Servo motor
- Buzzer
- Robot chassis
- Battery/power supply

## Working Distance

| Distance | Robot Action |
|---|---|
| 0–5 cm | Move backward |
| 5–8 cm | Move backward/adjust |
| 8–12 cm | Stop |
| 12–20 cm | Follow hand |
| >20 cm | Search for hand |

## Proof of Working

The following photographs show the physical robot, wiring, and testing of the hand-following system.

![Robot](images/robot.jpg)

![Testing](images/testing.jpg)
