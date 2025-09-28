# Car-Black-Box
A Car Black Box is a system inspired by the black boxes used in airplanes. It records and stores important vehicle data and events that can be analyzed later, especially in case of accidents. This project demonstrates how to design a black box system in embedded systems / C programming.
Features

Record critical vehicle parameters like:

Vehicle speed

Engine temperature

Fuel level

Brake status

Airbag deployment

Store accident-related data (time, date, crash impact).

Save data in a file (e.g., car_blackbox.dat) or EEPROM/SD card in embedded systems.

Retrieve and display stored records for analysis.

Basic simulation in C for understanding file handling and data logging.

Example Workflow

The system continuously records vehicle parameters.

On detecting an unusual event (like sudden deceleration), it marks and stores the event.

Data is saved for later retrieval by authorized users.

The user can choose from menu options to:

View logs
Example Output:
--- Car Black Box ---
1. Record Data
2. View Records
3. Clear Records
4. Exit
----------------------
Enter choice: 1
Speed  : 80 km/h
Temp   : 72 °C
Fuel   : 60 %
Status : Normal
Record saved successfully!
Use Cases

Accident investigation and analysis.

Driver behavior monitoring.

Vehicle performance tracking.

Educational project for learning data logging and file handling in C/Embedded C.

Clear logs

Exit
