## Adjustable Runway Arduino Firmware

**Table of Contents**

<<<<<<< HEAD
1. [Important Reminders](#important-reminders)
2. [Module Overview](#module-overview)
  - [ControlRC](#controlrc)
  - [PIDCommand](#pidcommand)
  - [SlewRateLimiter](#slewratelimiter)
3. [Runtime Flow](#runtime-flow)
=======
* [Important Reminders](#important-reminders)
* [Module Overview](#module-overview)
  *  [ControlRC](#controlrc)
  * [PIDCommand](#pidcommand)
  * [SlewRateLimiter](#slewratelimiter)
* [Runtime Flow](#runtime-flow)
>>>>>>> 183a0fb (Added a README for the overall repository)

---

## Important Reminders

### Downloading Code
  - Connect to the arduino uno with the USB port on the assembly
  - Turn the switch controlling the UART to the off state
  - Press the PlatformIO: Upload arrow *(->)* to upload code
  - Turn the UART switch back to the on state
  - Disconnect the cable going from your computer to the USB port

**Remeber:** When running the serial monitor for output, run a baudrate of 115200

---

## Module Overview

The following modules can be found in the `lib/` folder 

### ControlRC

The `ControlRC` module is used to receive the values from a FlySky FS-i6X RC transmitter. The module also contains an enum, `ChannelRC`, which is used for accessing control channels

The channels and their enum counterparts, by default, are as follows:

| Channel | ChannelRC Enum | Input |
|---------|----------------|-------|
| `01` | `RIGHT_X` | Right joystick x-axis |
| `02` | `RIGHT_Y` | Right joystick y-axis |
| `03` | `LEFT_Y` | Left joystick y-axis |
| `04` | `LEFT_X` | Left joystick x-axis |
| `05` | `SWA` | Outer left switch |
| `06` | `SWD` | Outer right switch |
| `07` | `SWB` | Inner left switch | 
| `08` | `SWC` | Inner right switch |
| `09` | `VRA` | Left variable knob |
| `10` | `VRB` | Right variable knob |

---

### PIDCommand

The `PIDCommand` module is used to create and control commands to use PID to control outputs to things like motors. 

Important methods:
* `calculate()` - Updates the value of the output using the input and setpoint data
* `atSetpoint()` - Stops the PID command if the error and error rates are within a certain limit
* `eStop()` - In case of emergency, stops the PID command until the Arduino is reset

---

### SlewRateLimiter 

The `SlewRateLimiter` module is used to create a limiter for how much a variable can change in a given amount of time.

Important Methods:
* `calulate(double target)` - Calculates the allowed amount the value can change
* `setRate(double pos, double neg)` - Sets the amount the value can change in either the positive direction or the negative direction 

---

## Runtime Flow

1. **setup()**
  - Begin Serial monitor
  - Initialize motors
  - Set RC channel mapping values
  - Record start time
2. **loop()**
  - Update current time
  - Receive and update RC channels at a set sample rate
  - Update and set motor enable, speed, and direction values 

---