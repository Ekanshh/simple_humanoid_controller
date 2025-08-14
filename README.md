# Simple Humanoid Controller

This project implements a simple (FSM-based) controller for the **JVRC-1 humanoid robot** using the [mc\_rtc](https://jrl-umi3218.github.io/mc_rtc/) framework.

---

## Overview

The `SimpleHumanoidController` controller continuously cycles through the following actions:

1. Move the **left hand** to a target pose, then return it to its initial pose.
2. Move the **right hand** to a target pose, then return it to its initial pose.
3. Move **both hands** to their target poses, then return them to their initial poses.

**Gaze behavior:**

* When a single hand is moving, the robot looks at that hand.
* When both hands are moving together, the robot looks straight ahead.

In addition, the controller enforces **left and right foot contacts**, **self-collision avoidance**, **joint constraints**, and the **equation of motion**.

---

## Installation

### 1. Prepare the `mc_rtc` development environment

Follow the [mc-rtc-superbuild devcontainer setup guide](https://github.com/mc-rtc/mc-rtc-superbuild/blob/main/doc/devcontainer.md) to setup the development environment.

### 2. Clone this repository as an `mc-rtc-superbuild` extension

Follow the instructions below to clone this repository as an `mc-rtc-superbuild` extension. Be sure to use the correct path to your superbuild directory.

```bash
cd <path>/mc-rtc-superbuild/extensions/
git clone git@github.com:Ekanshh/simple_humanoid_controller_superbuild.git
```

### 3. Build `mc-rtc-superbuild` with the new extension

Refer to the superbuild instructions to build the `mc-rtc-superbuild`. A successful build will make `SimpleHumanoidController` available in your `mc_rtc` installation.

### 4. Enable the controller in `mc_rtc` configuration

Create the `mc_rtc` configuration file at `$HOME/.config/mc_rtc/mc_rtc.yaml` to enable the controller:

```yaml
MainRobot: JVRC1
Enabled:
  - SimpleHumanoidController
```

---

## Running the Controller

Open **two terminals** in your development container environment.

**Terminal 1 – Launch the display:**

```bash
ros2 launch mc_rtc_ticker display.launch
```

**Terminal 2 – Run the controller:**

```bash
mc_rtc_ticker
```

---

## Demo Video

<sub><em>The demo showcases the robot executing the controller sequence at 2× speed. Click to download and watch!</em></sub>

[![Demo Video](media/demo_2x.png)](media/demo_2x.webm)

---

## Requirements

* [mc-rtc-superbuild](https://github.com/mc-rtc/mc-rtc-superbuild) (and dependencies)
* Tested on **Ubuntu 22.04 (Jammy)**

---

## License

This project is licensed under the MIT License.

---
