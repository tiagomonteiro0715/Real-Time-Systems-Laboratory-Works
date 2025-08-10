# STR - Real-Time Systems Laboratory Works

This repository holds the three final projects of the class Real Time Systems

The first project got a grade of 14.3 out of 20.

The second project got a grade of 16.5 out of 20.

The third project got a grade of 18.3 out of 20.

**The average of all projects was 16.4 out of 20**

---

## Project Overview

The project implements various real-time systems concepts including:
- **Lab 1 (C/C++)**: Parking system simulation using FreeRTOS
- **Lab 2 (Java + C)**: Splitter conveyor system with hardware integration
- **Lab 3 (Python)**: Fruit sorting system using Petri nets and GPIO simulation

## Project Structure

```
├── 1_work_C/                    # C/C++ FreeRTOS parking system
│   ├── labwork1.cpp             # Main parking system implementation
│   ├── my_interaction_functions.h/.cpp  # Hardware interaction functions
│   └── labwork1.sln/.vcxproj   # Visual Studio project files
├── 2_work_java/                 # Java-based splitter conveyor system
│   ├── C/                       # C++ hardware interface (JNI)
│   │   ├── SplitterConveyor.cpp/.h
│   │   └── splitterconveyor_jni.c
│   └── JAVA/                    # Java application layer
│       ├── Main.java            # Main application entry point
│       ├── SplitterConveyor.java # Hardware interface wrapper
│       ├── Cylinder*.java       # Cylinder control threads
│       ├── Switch*.java         # Switch monitoring threads
│       └── EmergencyThread.java # Emergency handling
└── 3_work_python/               # Python fruit sorting system
    ├── labwork3.py              # Main sorting system implementation
    ├── fruit_splitter_full_petri_net.hps  # Petri net model
    └── labwork3_simple_petri_nets.hps     # Simple Petri net examples
```

## Lab 1: Parking System (C/C++ + FreeRTOS)

### Description
A real-time parking management system that simulates car parking operations using FreeRTOS for task management and real-time scheduling.

### Features
- Multi-threaded parking operations
- Real-time task scheduling with FreeRTOS
- Emergency stop functionality
- Automatic parking coordination
- Cost calculation and historical tracking
- Hardware port management for sensors and actuators

### Requirements
- Visual Studio (C++ development tools)
- FreeRTOS library
- Windows.h for system functions

---

## Lab 2: Splitter Conveyor System (Java + C++)

### Description
A hardware-integrated conveyor system that sorts and routes boxes using multiple cylinders and sensors, with Java providing the high-level control and C++ handling hardware interactions.

### Features
- Multi-cylinder control (3 cylinders)
- Sensor-based box detection
- Conveyor belt management
- Emergency stop and resume functionality
- Real-time hardware monitoring
- JNI integration for hardware access

### Hardware Integration
The system interfaces with physical hardware through:
- Cylinder actuators (forward/backward movement)
- Position sensors
- Box detection sensors
- Conveyor belt control
- LED indicators

---

## Lab 3: Fruit Sorting System (Python)

### Description
A fruit sorting and counting system that uses Petri nets for process modeling and GPIO simulation for hardware interaction.

### Features
- Apple, pear, and lemon sorting
- Petri net-based process modeling
- GPIO pin simulation
- Web server integration for monitoring
- Real-time sensor and actuator control
- Emergency stop and resume functionality

### Requirements
- Python 3.x
- Required packages (see requirements below)
- Web browser for monitoring interface

## Key Concepts Demonstrated

### Real-Time Systems
- Task scheduling and prioritization
- Interrupt handling
- Real-time constraints management
- Resource synchronization

### Multi-threading
- Concurrent task execution
- Thread communication and synchronization
- Race condition prevention
- Deadlock avoidance

### Hardware Integration
- GPIO pin management
- Sensor reading and actuator control
- Hardware abstraction layers
- JNI for cross-language integration

### Process Modeling
- Petri net implementation
- State machine design
- Process flow control
- Event-driven programming

## Emergency and Safety Features

All three labs include emergency stop functionality:
- **Lab 1**: Emergency switch handling with system pause
- **Lab 2**: Emergency thread with hardware shutdown
- **Lab 3**: Emergency stop with system state preservation

## Contributors

- [Duarte Pereira](https://www.linkedin.com/in/duarte-pereira-349b4b319/)
- [Diogo Ferreira]()
- [Tiago Monteiro](https://www.linkedin.com/in/tiago-monteiro-/)
