# OSLab_TheBarberShop

This C++ project implements a simulation of a Barber Shop using Semaphores along with Priority Scheduling.

## Requirements
- C++ compiler (GCC or Clang)
- Make utility
- 'pthread' library

## How to Build and Run
1. Download or clone the project files.
2. Open a terminal or command prompt and navigate to the project directory.
3. Run the following command to build the project:

   ```
   make
   ```
   
4. Run the following command to start the simulation:

   ```
   ./barber_shop
   ```
   
5. The simulation will output the following information:
   - Barber status (awake or sleeping)
   - Customer status (waiting, getting a haircut, or leaving)
   - Number of customers waiting
   - Number of chairs available
   
## How to Clean
1. Open a terminal or command prompt and navigate to the project directory.
2. Run the following command to clean the project:

   ```
   make clean
   ```
   
This will remove all generated files, including the executable.
