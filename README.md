# Multicomponent-Distillation

Program that models a given multicomponent distillation system using the Fenske-Underwood-Gilliland-Kirkbride method.

## Directories and Files

1. [**Data_Files**](./Data_Files) Contains dictionary text files of chemical parameters
   - [**antoine_data.txt**](./Data_Files/antoine_data.txt) Dictionary text file of antoine coefficients for thousands of chemicals
   - [**azeotrope_data.txt**](./Data_Files/azeotrope_data.txt) Dictionary text file of common azeotropes and their distillation parameters
2. [**Source_Header_Files**](./Source_Header_Files) Contains the header and source files for the program
   - [**main.h**](./Source_Header_Files/main.h) This header file contains the data structures used across the program and their variables
   - [**main.c**](./Source_Header_Files/main.c) Interacts with the user and calls the appropriate functions
   - [**user.h**](./Source_Header_Files/user.h) Header file contains functions used by user.c
   - [**user.c**](./Source_Header_Files/user.c) evaluates user inputs and ensures they are correct and returns the confirmation values
   - [**fractional.h**](./Source_Header_Files/fractional.h) Header file contains functions and variables used by the fractional.c file
   - [**fractional.c**](./Source_Header_Files/fractional.c) Does a multicomponent fractional distillation calculation using the FUGK method
   - [**antoinedata.c**](./Source_Header_Files/antoinedata.c) Reads through the .txt file "antoine_data.txt" and extracts the correct antoine coefficients for the problem
   - [**azeotropedata.c**](./Source_Header_Files/azeotropedata.c) Reads through the .txt file "azeotrope_data.text" and warns the user if the mixture has an azeotrope
   - [**batch.c**](./Source_Header_Files/batch.c) Would have done a batch calculation for binary systems. It is not yet ready.
   - [**feed.c**](./Source_Header_Files/feed.c) Gets feed conditions from the user
   - [**flash.c**](./Source_Header_Files/flash.c) Does the flash calculation on the provided feed stream. It also gets user input about the system
   - [**print.c**](./Source_Header_Files/print.c) Prints the calculated results contained in the data structures to the screen
   - [**structure.c**](./Source_Header_Files/structure.c) places data from files and calculated results into the correct data stucture. This file also allocates memory for the data structures dynamically
3. [**Results Validation**](./Results%20Validation) Contains hand calculations done by Dr. Masse and William Veith to confirm the program's output results
   - [**Validation By Eric Maase.pdf**](./Results%20Validation/Validation%20By%20Eric%20Maase.pdf) By hand re-calculations of the program's model by Eric Maase
   - [**Validation By William Veith.pdf**](./Results%20Validation/Validation%20By%20William%20Veith.pdf) By hand re-calculations of the program's model by William Veith
4. [**Project Memo.pdf**](Project%20Memo.pdf) An overview of the program, the program results, and future issues that need to be address
5. [**User Interface.pdf**](User%20Interface.pdf) An overview of the program interface displayed by the command line
