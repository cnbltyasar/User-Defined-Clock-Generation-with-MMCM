/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xstatus.h"
#include "xuartlite_l.h"
#include "xil_printf.h"
#include <stdlib.h>  // For strtof()
#include "math.h"


#include <math.h> // Include for rounding
#include <xil_io.h>


#define XPAR_CLK_WIZ_0_BASEADDR 0x44a00000// Base address example; adjust as necessary
#define CLK_CFG_SR (XPAR_CLK_WIZ_0_BASEADDR + 0x04)
#define CLK_CFG_REG0 (XPAR_CLK_WIZ_0_BASEADDR + 0x200)
#define CLK_CFG_REG2 (XPAR_CLK_WIZ_0_BASEADDR + 0x208)
#define CLK_CFG_REG23 (XPAR_CLK_WIZ_0_BASEADDR + 0x25C)
#define CLKOUT0_DUTY (XPAR_CLK_WIZ_0_BASEADDR + 0x210)

// Simulating memory-mapped registers with local variables for demonstration
volatile uint32_t clockConfigReg0 = 0;
volatile uint32_t clockConfigReg2 = 0;
volatile uint32_t achieved_freq = 0;


/************************** Constant Definitions *****************************/

#ifndef SDT
#define UARTLITE_BASEADDR	   XPAR_UARTLITE_0_BASEADDR
#else
#define UARTLITE_BASEADDR	   XPAR_XUARTLITE_0_BASEADDR
#endif

#define BUFFER_SIZE 4  // Buffer size for receiving frequency

/************************** Function Prototypes ******************************/
void display_menu();
void set_frequency(u32 UartliteBaseAddress);
u8 get_user_choice(u32 UartliteBaseAddress);
u8 ask_set_new_frequency(u32 UartliteBaseAddress);


/************************** Variable Definitions *****************************/
u8 RecvBuffer[BUFFER_SIZE];  // Buffer for receiving data

/******************************************************************************/ 
/*
 * Main function to call the example.
 */
int main(void) {
    int Status;

    /*
     * Run the UartLite frequency example, specify the BaseAddress that is
     * generated in xparameters.h.
     */
    Status = UartLiteFrequencyExample(UARTLITE_BASEADDR);
    if (Status != XST_SUCCESS) {
        xil_printf("Uartlite frequency Example Failed\r\n");
        return XST_FAILURE;
    }

    xil_printf("Successfully ran Uartlite frequency Example\r\n");
    return XST_SUCCESS;
}

/******************************************************************************/ 
/*
 * This function handles the menu-based system for setting the frequency.
 */
int UartLiteFrequencyExample(u32 UartliteBaseAddress) {
    u8 choice;
    while (1) {
        display_menu();  // Display the options to the user

        choice = get_user_choice(UartliteBaseAddress);  // Get the user's choice

        switch (choice) {
        case '1':
            do {
                set_frequency(UartliteBaseAddress);  // Set the frequency
            } while (ask_set_new_frequency(UartliteBaseAddress) == 'y');  // Ask if the user wants to set a new frequency
            break;
        case '2':
            xil_printf("Exiting...\r\n");
            return XST_SUCCESS;  // Exit the program
        default:
            xil_printf("Invalid choice. Please try again.\r\n");
            break;
        }
    }
}


/******************************************************************************/ 
// Custom round function to round a float to the nearest integer
int custom_round(float number) {
    // Get the integer part of the number
    int integer_part = (int)number;

    // Get the fractional part by subtracting the integer part from the number
    float fractional_part = number - (float)integer_part;

    // If the fractional part is 0.5 or more, round up
    if (fractional_part >= 0.5) {
        return integer_part + 1;
    }
    // If the fractional part is less than 0.5, round down (stay at integer_part)
    else {
        return integer_part;
    }
}


/******************************************************************************/ 
// Function to calculate clock parameters
 // Function to calculate clock parameters and return a struct
void calculateClockParameters(float user_freq, uint32_t *DIVCLK_DIVIDE, uint32_t *CLKFBOUT_MULT, uint32_t *CLKFBOUT_FRAC, uint32_t *CLKOUT0_DIVIDE, uint32_t *CLKOUT0_FRAC, float reference_freq) {
    if (user_freq > 100) {
        *DIVCLK_DIVIDE = 5;

        // Calculate full_divider, CLKOUT0_DIVIDE, and CLKOUT0_FRAC
        float full_divider = 2.5;
        *CLKOUT0_DIVIDE = (uint32_t)full_divider;  // Integer part
        float frac_divider = full_divider - *CLKOUT0_DIVIDE;  // Fractional part
        *CLKOUT0_FRAC = custom_round(frac_divider * 1000 / 125) * 125;

        if (*CLKOUT0_FRAC > 875) {
            *CLKOUT0_FRAC = 875;  // Cap the fractional part
        }

        // Calculate full_multiplier and CLKFBOUT_MULT/FRAC
        float full_multiplier = (user_freq * (*DIVCLK_DIVIDE) * full_divider) / reference_freq;
        *CLKFBOUT_MULT = (uint32_t)full_multiplier;
        float frac_multiplier = full_multiplier - *CLKFBOUT_MULT;
        *CLKFBOUT_FRAC = custom_round(frac_multiplier * 1000 / 125) * 125;

        if (*CLKFBOUT_FRAC > 875) {
            *CLKFBOUT_FRAC = 875;  // Cap the fractional part
        }

    } else {
        *DIVCLK_DIVIDE = 5;
        // Calculate CLKFBOUT_MULT/FRAC
        float full_multiplier = ((user_freq * (*DIVCLK_DIVIDE)) / reference_freq) * 10;
        *CLKFBOUT_MULT = (uint32_t)full_multiplier;
        float frac_multiplier = full_multiplier - *CLKFBOUT_MULT;
        *CLKFBOUT_FRAC = custom_round(frac_multiplier * 1000 / 125) * 125;

        if (*CLKFBOUT_FRAC > 875) {
            *CLKFBOUT_FRAC = 875;
        }

        // Calculate full_divider, CLKOUT0_DIVIDE, and CLKOUT0_FRAC
        float full_divider = ((float)*CLKFBOUT_MULT + (float)(*CLKFBOUT_FRAC) / 1000) * reference_freq / (user_freq * (*DIVCLK_DIVIDE));
        *CLKOUT0_DIVIDE = (uint32_t)full_divider;
        float frac_divider = full_divider - (*CLKOUT0_DIVIDE);
        *CLKOUT0_FRAC = custom_round(frac_divider * 1000 / 125) * 125;

        if (*CLKFBOUT_FRAC > 875) {
            *CLKFBOUT_FRAC = 875;  // Cap the fractional part
        }

    }
    achieved_freq = (reference_freq * ((float)*CLKFBOUT_MULT + ((float)*CLKFBOUT_FRAC / 1000))) /
                    ((float)*DIVCLK_DIVIDE * ((float)*CLKOUT0_DIVIDE + ((float)*CLKOUT0_FRAC / 1000)));

    // Update Clock Configuration Register 0
    clockConfigReg0 = (*DIVCLK_DIVIDE & 0xFF) |
                      ((*CLKFBOUT_MULT & 0xFF) << 8) |
                      ((*CLKFBOUT_FRAC & 0x3FF) << 16);  // Fractional part of the multiplier

    // Update Clock Configuration Register 2 with CLKOUT0_DIVIDE and CLKOUT0_FRAC
    clockConfigReg2 = (*CLKOUT0_DIVIDE & 0xFF) |          // Store integer part
                      ((*CLKOUT0_FRAC & 0x3FF) << 8);     // Store fractional part
}



/******************************************************************************/ 
/*
 * Displays the menu options to the user.
 */
void display_menu() {
    xil_printf("\r\n****** Frequency Control Menu ******\r\n");
    xil_printf("1. Set Frequency\r\n");
    xil_printf("2. Exit\r\n");
    xil_printf("Enter your choice: ");
}

/******************************************************************************/ 
/*
 * Sets the frequency based on the user's input.
 */
void set_frequency(u32 UartliteBaseAddress) {
    u8 ReceivedChar;
    u8 BufferIndex = 0;
    int received_freq;
    uint32_t DIVCLK_DIVIDE, CLKFBOUT_MULT, CLKFBOUT_FRAC, CLKOUT0_DIVIDE, CLKOUT0_FRAC;
    float reference_freq = 100;  // Reference frequency in MHz
    uint32_t Lock_reg;

    xil_printf("\r\nEnter the frequency value (up to 3 characters such as 100, 050): ");

    // Initialize the receive buffer
    for (int Index = 0; Index < BUFFER_SIZE; Index++) {
        RecvBuffer[Index] = 0;
    }

    // Receive the frequency value from UART
    while (1) {
        ReceivedChar = XUartLite_RecvByte(UartliteBaseAddress);
        
        // Echo back the character to the user
        XUartLite_SendByte(UartliteBaseAddress, ReceivedChar);

        // Check for newline or buffer overflow
        if (ReceivedChar == '\n' || BufferIndex >= BUFFER_SIZE - 1) {
            break;
        }

        RecvBuffer[BufferIndex++] = ReceivedChar;
    }

    // Null-terminate the string
    RecvBuffer[BufferIndex] = '\0';

    // Print the received frequency value
    xil_printf("\r\nFrequency set to: %s MHz\r\n", RecvBuffer);


    received_freq = atoi(RecvBuffer);
    xil_printf("\r\nFrequency set to: %d MHz\r\n", (received_freq));

    calculateClockParameters(received_freq, &DIVCLK_DIVIDE, &CLKFBOUT_MULT, &CLKFBOUT_FRAC, &CLKOUT0_DIVIDE, &CLKOUT0_FRAC, reference_freq);
        // Output the set parameters
    xil_printf("Parameters Set: \n");
    xil_printf("DIVCLK_DIVIDE = %d\n", DIVCLK_DIVIDE);
    xil_printf("CLKFBOUT_MULT = %d\n", CLKFBOUT_MULT);
    xil_printf("CLKFBOUT_FRAC = %d (Fractional Part)\n", CLKFBOUT_FRAC);
    xil_printf("CLKOUT0_DIVIDE = %d\n", CLKOUT0_DIVIDE);
    xil_printf("CLKOUT0_FRAC = %d (Fractional Part)\n", CLKOUT0_FRAC);

        // Calculating the achieved frequency with both integer and fractional parts
    // Calculating the achieved frequency with both integer and fractional parts

    // Perform calculations
// Perform calculations
    if (DIVCLK_DIVIDE == 0) {
        xil_printf("Error: DIVCLK_DIVIDE is zero.\n");
    } 
    if ((CLKOUT0_DIVIDE + (float)CLKOUT0_FRAC / 1000.0f) == 0) {
        xil_printf("Error: CLKOUT0_DIVIDE + CLKOUT0_FRAC is zero.\n");
    } 
    if (DIVCLK_DIVIDE == 0 || (CLKOUT0_DIVIDE + (float)CLKOUT0_FRAC / 1000.0f) == 0) {
        xil_printf("Error: Division by zero in frequency calculation.\n");
    } else {
        xil_printf("Achieved Frequency = %d MHz\n", achieved_freq);
    }



    // For demonstration purposes, print the register values
    xil_printf("\nClock Configuration Register 0 = 0x%08X\n", clockConfigReg0);
    xil_printf("Clock Configuration Register 2 = 0x%08X\n", clockConfigReg2);  

    // C_BASEADDR + 0x04 Status Register (SR)
    Lock_reg =Xil_In32(CLK_CFG_SR);
    xil_printf("Locked %x\n", Lock_reg);
    
    Xil_Out32(CLK_CFG_REG0, clockConfigReg0);
    Xil_Out32(CLK_CFG_REG2,clockConfigReg2);
    Xil_Out32(CLK_CFG_REG23, 0x3);
    
    //Xil_Out32(CLKOUT0_DUTY, 0x13880); // yüzde seksen clock cycle
    
    for (volatile int i = 0; i < 1000000; i++);  // Simple delay loop
    Lock_reg =Xil_In32(CLK_CFG_SR);
    xil_printf("Locked %x\n", Lock_reg);
    
    xil_printf("Succesfully Clock Frequency setted %d MHz\n", achieved_freq);


}

/******************************************************************************/ 
/*
 * Ask if the user wants to set a new frequency after setting one.
 */
u8 ask_set_new_frequency(u32 UartliteBaseAddress) {
    u8 choice;
    
    xil_printf("\r\nDo you want to set a new frequency? (y/n): ");
    
    choice = XUartLite_RecvByte(UartliteBaseAddress);  // Get user input for setting new frequency
    xil_printf("%c\r\n", choice);  // Echo back the choice

    return choice;
}

/******************************************************************************/ 
/*
 * Get the user's choice from the menu.
 */
u8 get_user_choice(u32 UartliteBaseAddress) {
    u8 choice = XUartLite_RecvByte(UartliteBaseAddress);  // Get user input from UART
    xil_printf("%c\r\n", choice);  // Echo back the choice
    return choice;
}
