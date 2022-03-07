/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F15344
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set Doorlock aliases
#define Doorlock_TRIS                 TRISAbits.TRISA2
#define Doorlock_LAT                  LATAbits.LATA2
#define Doorlock_PORT                 PORTAbits.RA2
#define Doorlock_WPU                  WPUAbits.WPUA2
#define Doorlock_OD                   ODCONAbits.ODCA2
#define Doorlock_ANS                  ANSELAbits.ANSA2
#define Doorlock_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define Doorlock_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define Doorlock_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define Doorlock_GetValue()           PORTAbits.RA2
#define Doorlock_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define Doorlock_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define Doorlock_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define Doorlock_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define Doorlock_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define Doorlock_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define Doorlock_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define Doorlock_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set SW_U1 aliases
#define SW_U1_TRIS                 TRISBbits.TRISB4
#define SW_U1_LAT                  LATBbits.LATB4
#define SW_U1_PORT                 PORTBbits.RB4
#define SW_U1_WPU                  WPUBbits.WPUB4
#define SW_U1_OD                   ODCONBbits.ODCB4
#define SW_U1_ANS                  ANSELBbits.ANSB4
#define SW_U1_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define SW_U1_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define SW_U1_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define SW_U1_GetValue()           PORTBbits.RB4
#define SW_U1_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define SW_U1_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define SW_U1_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define SW_U1_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define SW_U1_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define SW_U1_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define SW_U1_SetAnalogMode()      do { ANSELBbits.ANSB4 = 1; } while(0)
#define SW_U1_SetDigitalMode()     do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set SW_U2 aliases
#define SW_U2_TRIS                 TRISBbits.TRISB5
#define SW_U2_LAT                  LATBbits.LATB5
#define SW_U2_PORT                 PORTBbits.RB5
#define SW_U2_WPU                  WPUBbits.WPUB5
#define SW_U2_OD                   ODCONBbits.ODCB5
#define SW_U2_ANS                  ANSELBbits.ANSB5
#define SW_U2_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define SW_U2_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define SW_U2_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define SW_U2_GetValue()           PORTBbits.RB5
#define SW_U2_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define SW_U2_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define SW_U2_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define SW_U2_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define SW_U2_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define SW_U2_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define SW_U2_SetAnalogMode()      do { ANSELBbits.ANSB5 = 1; } while(0)
#define SW_U2_SetDigitalMode()     do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set SW_Door aliases
#define SW_Door_TRIS                 TRISBbits.TRISB6
#define SW_Door_LAT                  LATBbits.LATB6
#define SW_Door_PORT                 PORTBbits.RB6
#define SW_Door_WPU                  WPUBbits.WPUB6
#define SW_Door_OD                   ODCONBbits.ODCB6
#define SW_Door_ANS                  ANSELBbits.ANSB6
#define SW_Door_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define SW_Door_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define SW_Door_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define SW_Door_GetValue()           PORTBbits.RB6
#define SW_Door_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define SW_Door_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define SW_Door_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define SW_Door_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define SW_Door_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define SW_Door_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define SW_Door_SetAnalogMode()      do { ANSELBbits.ANSB6 = 1; } while(0)
#define SW_Door_SetDigitalMode()     do { ANSELBbits.ANSB6 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)
#define RC0_SetAnalogMode()         do { ANSELCbits.ANSC0 = 1; } while(0)
#define RC0_SetDigitalMode()        do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSC1 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/