/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See Readme.md
*
*******************************************************************************
* (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h" // include Peripheral Driver Library
#include "cyhal.h" // include Hardware Abstraction Layer library
#include "cycfg.h" // include device configurator library

/*******************************************************************************
* Function Prototypes
********************************************************************************/
static void button_intr_handler(void *handler_arg, cyhal_gpio_event_t event);

/*******************************************************************************
* Global Variables
********************************************************************************/
volatile bool button_intr_flag = false;

int main(void)
{
    init_cycfg_all(); // configure pins as done in Device Configurator
    __enable_irq(); // enable interrupt
    cyhal_gpio_register_callback(button_HAL_PORT_PIN,
                                 button_intr_handler, NULL); // assigning isr handler for button
    cyhal_gpio_enable_event(button_HAL_PORT_PIN, CYHAL_GPIO_IRQ_RISE,
                                 1u, true); // interrupt on rising edge (button pressed)

    for (;;)
    {
    	if (true == button_intr_flag) // if interrupt happens (button pressed)
    	{
        	button_intr_flag = false; // clear interrupt flag for further interrupt
        	if(Cy_GPIO_ReadOut(water_pump_PORT, water_pump_NUM)==0UL)
        	{
        		Cy_GPIO_Write(green_PORT, green_NUM, 1UL); // green LED on
        		Cy_GPIO_Write(water_pump_PORT, water_pump_NUM, 1UL);	// on water pump
        	}
        	else
        	{
        		Cy_GPIO_Write(green_PORT, green_NUM, 0UL); // green LED off
        		Cy_GPIO_Write(water_pump_PORT, water_pump_NUM, 0UL);	// off water pump
        	}
    	}


    }
}

/*******************************************************************************
* Function Name: button_intr_handler
********************************************************************************
* Summary:
*   GPIO interrupt handler.
*
* Parameters:
*  void *handler_arg (unused)
*  cyhal_gpio_irq_event_t (unused)
*
*******************************************************************************/
static void button_intr_handler(void *handler_arg, cyhal_gpio_irq_event_t event)
{
    button_intr_flag = true;
}

///* [] END OF FILE */
