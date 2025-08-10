"""
3º work of the Real Time Systems class

Work completed by:
Duarte Miguel Corraleira Pereira - 62446 - P4 - LEEC
Diogo Batista Ferreira - 62501 - P4 - LEEC
Tiago Capelo Monteiro - 63368 - P4 - LEEC

This code simulates a box sorting and counting system. It uses GPIO pins to interact with sensors and actuators
and provides a user interface for monitoring and controlling the system. The program is divided into the following sections:

1. Global Variables:
   This section initializes all the global variables used throughout the program. These variables store the state
   of the system, including box counts, actuator states, and emergency stop status.

2. System Initialization:
   Sets up the GPIO pins required for the system. Input pins are used for reading sensor data, and output pins
   are used for controlling actuators such as cylinders and conveyors.

3. Sensor Detection Functions:
   Defines functions to read sensor inputs and determine the presence of fruits (apples, pears, lemons) or the 
   position of stations. These functions help in identifying when an action needs to be taken.

4. Actuation Functions:
   Contains functions to control the movements of cylinders and conveyors. These functions activate the GPIO
   pins to move items through the sorting system and update the system's state.

5. Emergency and Resume Functions:
   Includes functions to stop all movements in case of an emergency and to resume operations based on the previous
   states. These functions ensure the safety and continuity of the system during interruptions.

6. User Input Handling:
   Provides a text-based user interface for interacting with the system. Users can view box counts, trigger
   emergency stops, resume the system, or exit the program.

7. Main Section:
   Initializes the system, starts the web server for simulation, and runs the user interface in a separate thread.
   This is the entry point of the program and ensures all components work together.

"""
import threading
import os
import time
import sys
sys.path.append("simulators")
import simulators.GPIOsim as GPIO
import simulators.iSTRwebServer as webServer

# ========================
# Global Variables Section
# ========================

keyboardKey = ''

box_counts = {"Box Station 1": 0, "Box Station 2": 0, "Box Station 3": 0}
box_counts_buffer = {"Box Station 1": False, "Box Station 2": False, "Box Station 3": False}

total_boxes = {"Apple Box": 0, "Pear Box": 0, "Lemon Box": 0}
total_boxes_buffer = {"Apple Box": False, "Pear Box": False, "Lemon Box": False}

cylinder_state = {
    "cylinder1_forward": False,
    "cylinder1_backward": False,
    "cylinder2_forward": False,
    "cylinder2_backward": False,
    "cylinder3_forward": False,
    "cylinder3_backward": False,
    "conveyor": False
}

# ==============================
# System Initialization Section
# ==============================

def initialize_system():
    """
    This function initializes the system by setting up all required GPIO pins.
    It configures input pins for sensors and output pins for actuators,
    ensuring that all pins are ready for use in the sorting system.
    """
    GPIO.setup(1, GPIO.INPUT)
    GPIO.setup(2, GPIO.INPUT)
    GPIO.setup(3, GPIO.INPUT)
    GPIO.setup(4, GPIO.INPUT)
    GPIO.setup(5, GPIO.INPUT)
    GPIO.setup(6, GPIO.INPUT)
    GPIO.setup(7, GPIO.INPUT)
    GPIO.setup(8, GPIO.INPUT)
    GPIO.setup(9, GPIO.INPUT)
    GPIO.setup(10, GPIO.INPUT)
    GPIO.setup(11, GPIO.INPUT)
    GPIO.setup(12, GPIO.INPUT)

    GPIO.setup(13, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(14, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(15, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(16, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(17, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(18, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(19, GPIO.OUTPUT, GPIO.LOW)
    
    GPIO.setup(20, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(21, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(22, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(23, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(24, GPIO.OUTPUT, GPIO.LOW)
    GPIO.setup(25, GPIO.OUTPUT, GPIO.LOW)

# ==============================
# Sensor Detection Functions
# ==============================
def detect_apple(tokensCount=0): 
    """
    This function detects if an apple is present by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(3) == GPIO.HIGH

def detect_pear(tokensCount=0): 
    """
    This function detects if a pear is present by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(4) == GPIO.HIGH

def detect_lemon(tokensCount=0): 
    """
    This function detects if a lemon is present by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(5) == GPIO.HIGH

def dedect_station_2_fruit(tokensCount=0): 
    """
    This function checks if a fruit is detected at Station 2 by reading the appropriate sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(8) == GPIO.HIGH

def dedect_station_3_fruit(tokensCount=0): 
    """
    This function checks if a fruit is detected at Station 3 by reading the appropriate sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(11) == GPIO.HIGH

def dedect_station_4_fruit(tokensCount=0): 
    """
    This function checks if a fruit is detected at Station 4 by reading the appropriate sensor pin.
    If detected, it updates the box counts and buffer states for Station 3 and Lemon Box.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    if(GPIO.input(12) == GPIO.HIGH):
        box_counts_buffer["Box Station 3"] = True
        total_boxes_buffer["Lemon Box"] = True
    return GPIO.input(12) == GPIO.HIGH

def is_Station_1_work_position(tokensCount=0): 
    """
    This function checks if Station 1 is in the work position by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(1) == GPIO.HIGH

def is_Station_1_rest_position(tokensCount=0): 
    """
    This function checks if Station 1 is in the rest position by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(2) == GPIO.HIGH

def is_Station_2_work_position(tokensCount=0): 
    """
    This function checks if Station 2 is in the work position by reading the state of the corresponding sensor pin.
    If so, it updates the box counts and buffer states for Station 1 and Apple Box.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    if(GPIO.input(6) == GPIO.HIGH):
        box_counts_buffer["Box Station 1"] = True
        total_boxes_buffer["Apple Box"] = True
    return GPIO.input(6) == GPIO.HIGH

def is_Station_2_rest_position(tokensCount=0): 
    """
    This function checks if Station 2 is in the rest position by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(7) == GPIO.HIGH

def is_Station_3_work_position(tokensCount=0):
    """
    This function checks if Station 3 is in the work position by reading the state of the corresponding sensor pin.
    If so, it updates the box counts and buffer states for Station 2 and Pear Box.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    if(GPIO.input(9) == GPIO.HIGH):
        box_counts_buffer["Box Station 2"] = True
        total_boxes_buffer["Pear Box"] = True
    return GPIO.input(9) == GPIO.HIGH

def is_Station_3_rest_position(tokensCount=0): 
    """
    This function checks if Station 3 is in the rest position by reading the state of the corresponding sensor pin.
    It returns True if the sensor is activated (HIGH) and False otherwise.
    """
    return GPIO.input(10) == GPIO.HIGH

# ==============================
# Actuation Functions Section
# ==============================
def move_cylinder1_forward(tokensBefore=0, tokensNow=0, tokensCount=0): 
    """
    This function moves Cylinder 1 forward by activating the forward GPIO pin
    and deactivating the backward GPIO pin. It updates the `cylinder_state`
    dictionary to reflect the cylinder's new state.
    """
    GPIO.output(14, GPIO.LOW)#Para de andar para tras
    GPIO.output(13, GPIO.HIGH)
    cylinder_state["cylinder1_forward"] = True
    cylinder_state["cylinder1_backward"] = False
    
def move_cylinder1_backward(tokensBefore=0, tokensNow=0, tokensCount=0): 
    """
    This function moves Cylinder 1 backward by activating the backward GPIO pin
    and deactivating the forward GPIO pin. It updates the `cylinder_state`
    dictionary to reflect the cylinder's new state.
    """
    GPIO.output(13, GPIO.LOW)#Para de andar para a frente
    GPIO.output(14, GPIO.HIGH)
    cylinder_state["cylinder1_forward"] = False
    cylinder_state["cylinder1_backward"] = True
    
def move_cylinder2_forward(tokensBefore=0, tokensNow=0, tokensCount=0):
    """
    This function moves Cylinder 2 forward by activating the forward GPIO pin
    and deactivating the backward GPIO pin. It updates the `cylinder_state`
    dictionary to reflect the cylinder's new state.
    """
    GPIO.output(16, GPIO.LOW)#Para de andar para tras
    GPIO.output(15, GPIO.HIGH)
    cylinder_state["cylinder2_forward"] = True
    cylinder_state["cylinder2_backward"] = False
    
def move_cylinder2_backward(tokensBefore=0, tokensNow=0, tokensCount=0):
    """
    This function moves Cylinder 2 backward by activating the backward GPIO pin
    and deactivating the forward GPIO pin. It updates the `cylinder_state`
    dictionary to reflect the cylinder's new state.
    """
    GPIO.output(15, GPIO.LOW)#Para de andar para a frente 
    GPIO.output(16, GPIO.HIGH)
    cylinder_state["cylinder2_forward"] = False
    cylinder_state["cylinder2_backward"] = True
    
def move_cylinder3_forward(tokensBefore=0, tokensNow=0, tokensCount=0): 
    """
    This function moves Cylinder 3 forward by activating the forward GPIO pin
    and deactivating the backward GPIO pin. It updates the `cylinder_state`
    dictionary to reflect the cylinder's new state.
    """
    GPIO.output(18, GPIO.LOW)#Para de andar para tras
    GPIO.output(17, GPIO.HIGH)
    cylinder_state["cylinder3_forward"] = True
    cylinder_state["cylinder3_backward"] = False
    
def move_cylinder3_backward(tokensBefore=0, tokensNow=0, tokensCount=0): 
    """
    This function moves Cylinder 3 backward by activating the backward GPIO pin
    and deactivating the forward GPIO pin. It updates the `cylinder_state`
    dictionary to reflect the cylinder's new state.
    """
    GPIO.output(17, GPIO.LOW)#Para de andar para a frente 
    GPIO.output(18, GPIO.HIGH)
    cylinder_state["cylinder3_forward"] = False
    cylinder_state["cylinder3_backward"] = True

def move_conveyor(tokensBefore=0, tokensNow=0, tokensCount=0):
    """
    This function activates the conveyor by enabling the corresponding GPIO pin.
    It also updates box counts and buffers for stations and fruit types based on
    the current buffer states.
    """
    GPIO.output(19, GPIO.HIGH)
    cylinder_state["conveyor"] = True
    
    if (box_counts_buffer["Box Station 1"] == True):
        box_counts_buffer["Box Station 1"] = False
        box_counts["Box Station 1"] += 1
    
    if (box_counts_buffer["Box Station 2"] == True):
        box_counts_buffer["Box Station 2"] = False
        box_counts["Box Station 2"] += 1

    if (box_counts_buffer["Box Station 3"] == True):
        box_counts_buffer["Box Station 3"] = False
        box_counts["Box Station 3"] += 1

    if (total_boxes_buffer["Apple Box"] == True):
        total_boxes_buffer["Apple Box"] = False
        total_boxes["Apple Box"] += 1
    
    if (total_boxes_buffer["Pear Box"] == True):
        total_boxes_buffer["Pear Box"] = False
        total_boxes["Pear Box"] += 1
    
    if (total_boxes_buffer["Lemon Box"] == True):
        total_boxes_buffer["Lemon Box"] = False
        total_boxes["Lemon Box"] += 1
    
def stop_conveyor(tokensBefore=0, tokensNow=0, tokensCount=0): 
    """
    This function stops the conveyor by deactivating the corresponding GPIO pin.
    It updates the `cylinder_state` dictionary to indicate that the conveyor is stopped.
    """
    GPIO.output(19, GPIO.LOW)
    cylinder_state["conveyor"] = False



# =====================================
# Emergency and Resume Functions Section
# =====================================
def stop_all_cylinders(tokensBefore=0, tokensNow=0, tokensCount=0):
    """
    This function stops all cylinder and conveyor movements by deactivating all relevant GPIO pins.
    It ensures the system is completely halted.
    """
    #stop_cylinder1
    GPIO.output(13, GPIO.LOW)#Para de andar para a frente
    GPIO.output(14, GPIO.LOW)
    
    #stop_cylinder2
    GPIO.output(15, GPIO.LOW)#Para de andar para a frente
    GPIO.output(16, GPIO.LOW)
    
    #stop_cylinder3
    GPIO.output(17, GPIO.LOW)#Para de andar para a frente
    GPIO.output(18, GPIO.LOW)
    
    #stop_conveyor
    GPIO.output(19, GPIO.LOW)

def resume_all_cylinders(tokensBefore=0, tokensNow=0, tokensCount=0):
    """
    This function resumes all cylinder and conveyor movements based on their previous states.
    It activates the appropriate GPIO pins according to the saved states in `cylinder_state`.
    """
    if cylinder_state["cylinder1_forward"]:
        GPIO.output(13, GPIO.HIGH)
    if cylinder_state["cylinder1_backward"]:
        GPIO.output(14, GPIO.HIGH)
    if cylinder_state["cylinder2_forward"]:
        GPIO.output(15, GPIO.HIGH)
    if cylinder_state["cylinder2_backward"]:
        GPIO.output(16, GPIO.HIGH)
    if cylinder_state["cylinder3_forward"]:
        GPIO.output(17, GPIO.HIGH)
    if cylinder_state["cylinder3_backward"]:
        GPIO.output(18, GPIO.HIGH)
    if cylinder_state["conveyor"]:
        GPIO.output(19, GPIO.HIGH)    

def is_pressed_e_key(tokensCount=0):
    """
    This function checks if the emergency stop key ('e') has been pressed.
    It returns True if the key is pressed, allowing emergency stop logic to be triggered.
    """
    global keyboardKey
    return keyboardKey == 'e'

def is_pressed_r_key(tokensCount=0):
    """
    This function checks if the resume key ('r') has been pressed.
    It returns True if the key is pressed, allowing the system to resume operations.
    """
    global keyboardKey
    return keyboardKey == 'r'

# ==============================
# User Input Handling Section
# ==============================
def user_input_handler():
    """
    This function handles user input for interacting with the system. It provides options to view box counts,
    trigger an emergency stop, resume the system, or exit the program. The input is processed in a loop to
    continuously accept user commands.
    """
    global keyboardKey
    print("Welcome to the Box Counting System!")
    print("====================================")
    print("Commands:")
    print("  [x] - View current box counts by station")
    print("  [y] - View total boxes sorted by type")
    print("  [e] - Activate emergency stop")
    print("  [r] - Resume system")
    print("  [q] - Quit the system")
    print("====================================")
    
    while True:
        keyboardKey = input("\nEnter your command: ").strip().lower()
        
        if keyboardKey == "x":
            print("\n=== Current Box Counts by Station ===")
            for station, count in box_counts.items():
                print(f"{station}: {count} boxes")
            print("======================================")
        elif keyboardKey == "y":
            print("\n=== Total Boxes by Type ===")
            for box_type, count in total_boxes.items():
                print(f"{box_type}: {count} ")
            print("============================")
        elif keyboardKey == "e":
            print("Emergency Activated")
        elif keyboardKey == "r":
            print("Resume system")
        elif keyboardKey == "q":
            print("\nThank you for using the Box Counting System. Goodbye!")
            os._exit(0)
        else:
            print("\nInvalid command. Please try again.")
            print("Use 'x' to view box counts, 'y' to view totals, 'e' for emergency (disabled), 'r' to resume (disabled), or 'q' to quit.")


# ================
# Main Section
# ================
if __name__ == "__main__":
    """
    This block is the entry point of the program. It initializes the system by setting up GPIO pins
    and starts the necessary threads to run the system. The web server for the simulator is launched
    in a separate thread to handle simulation functionality, and another thread is started for the 
    user input interface, allowing concurrent operations.
    """
    initialize_system()

    # Start the web server for the simulator
    server_thread = threading.Thread(target=webServer.run_server, args=('localhost', 8089, GPIO))
    server_thread.start()

    # Start the UI thread
    ui_thread = threading.Thread(target=user_input_handler)
    ui_thread.start()