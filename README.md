# User-Defined-Clock-Generation-with-MMCM

This project implements a user-defined clock generator using an MMCM (Mixed-Mode Clock Manager) on a Xilinx FPGA. The design uses a UARTLite interface to receive a frequency value (in MHz) from the user and then calculates the clock parameters (such as divider and multiplier values) needed to configure the MMCM. The calculated parameters are then written to memory-mapped registers to set the clock frequency. The project also demonstrates reading back the lock status of the MMCM.

Table of Contents
Overview
Features
Folder Structure
Build and Run
Source Files Description
License
Overview
The project allows the user to input a desired clock frequency via UART. The software then:

Reads the frequency value from UART.
Computes the required MMCM parameters (dividers and multipliers) using custom calculations.
Writes the calculated parameters to memory-mapped registers (simulated or actual MMCM registers).
Verifies and displays the achieved frequency and lock status via UART.
This solution is built on top of Xilinx drivers (such as xparameters.h, xuartlite_l.h, and xil_io.h) and can be integrated into an FPGA design using Xilinx Vivado along with SDK/Vitis for software development.

Features
UART-Based Control: A simple menu-driven interface allows the user to select clock configuration options.
Dynamic Frequency Setting: Accepts a new clock frequency input (up to 3 digits) and recalculates MMCM parameters accordingly.
MMCM Parameter Calculation: Uses a custom rounding function and arithmetic to determine the divider and multiplier values.
Status Feedback: Displays register values and lock status to help verify successful configuration.
Memory-Mapped I/O: Interacts directly with the MMCM registers via Xil_Out32() and Xil_In32() functions.


![image](https://github.com/user-attachments/assets/ab5fbf9f-e5be-4130-ae8c-5e9c6a235253)

![WhatsApp Görsel 2024-08-30 saat 12 17 34_c1b9eb58](https://github.com/user-attachments/assets/85a541f7-bcd2-4e16-94f8-8e3a096ad0ac)

![WhatsApp Görsel 2024-08-30 saat 12 16 00_0b45835d](https://github.com/user-attachments/assets/e1aad15c-f255-4151-9e0c-40b9f66e5b0d)

UART Adresses: 

![WhatsApp Görsel 2024-08-30 saat 12 20 21_c1502ef6](https://github.com/user-attachments/assets/f82f48cd-0815-43aa-80a5-f5f1142a357a)

UART Frequecny Control Menu:
![WhatsApp Görsel 2024-10-08 saat 23 25 35_7cc2dddc](https://github.com/user-attachments/assets/ea5cb6e0-e6ee-4e41-82cf-058d10388599)

Waveform Result With İLA: 

![WhatsApp Görsel 2024-10-08 saat 20 14 20_455acace](https://github.com/user-attachments/assets/77d80e85-e56b-4a26-81f5-9fc1da07160a)

