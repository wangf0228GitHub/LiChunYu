#ifndef	_HTC_H_
#warning Header file pic18f24k22.h included directly. Use #include <htc.h> instead.
#endif

 /* header file for the MICROCHIP PIC microcontroller
    18F24K22
 */

#ifndef __PIC18F24K22_H
#define __PIC18F24K22_H

#ifdef _PLIB
#error Peripheral Library not supported
#endif


// Config Register: CONFIG1H
// Oscillator Selection bits
// EC oscillator (low power, <500 kHz)
#define FOSC_ECLPIO6         0xF7FF 
// EC oscillator, CLKOUT function on OSC2 (low power, <500 kHz)
#define FOSC_ECLP            0xF6FF 
// EC oscillator (medium power, 500 kHz-16 MHz)
#define FOSC_ECMPIO6         0xF1FF 
// EC oscillator, CLKOUT function on OSC2 (medium power, 500 kHz-16 MHz)
#define FOSC_ECMP            0xF0FF 
// Internal oscillator block, CLKOUT function on OSC2
#define FOSC_INTIO7          0xF3FF 
// Internal oscillator block
#define FOSC_INTIO67         0xF2FF 
// External RC oscillator
#define FOSC_RCIO6           0xFDFF 
// External RC oscillator, CLKOUT function on OSC2
#define FOSC_RC              0xFCFF 
// EC oscillator (high power, >16 MHz)
#define FOSC_ECHPIO6         0xFFFF 
// EC oscillator, CLKOUT function on OSC2 (high power, >16 MHz)
#define FOSC_ECHP            0xFEFF 
// HS oscillator (medium power 4-16 MHz)
#define FOSC_HSMP            0xF9FF 
// HS oscillator (high power > 16 MHz)
#define FOSC_HSHP            0xF8FF 
// XT oscillator
#define FOSC_XT              0xFBFF 
// LP oscillator
#define FOSC_LP              0xFAFF 
// 4X PLL Enable
// Oscillator used directly
#define PLLCFG_OFF           0xFFFF 
// Oscillator multiplied by 4
#define PLLCFG_ON            0xEFFF 
// Primary clock enable bit
// Primary clock can be disabled by software
#define PRICLKEN_OFF         0xDFFF 
// Primary clock enabled
#define PRICLKEN_ON          0xFFFF 
// Fail-Safe Clock Monitor Enable bit
// Fail-Safe Clock Monitor disabled
#define FCMEN_OFF            0xFFFF 
// Fail-Safe Clock Monitor enabled
#define FCMEN_ON             0xBFFF 
// Internal/External Oscillator Switchover bit
// Oscillator Switchover mode disabled
#define IESO_OFF             0xFFFF 
// Oscillator Switchover mode enabled
#define IESO_ON              0x7FFF 


// Config Register: CONFIG2L
// Power-up Timer Enable bit
// Power up timer disabled
#define PWRTEN_OFF           0xFFFF 
// Power up timer enabled
#define PWRTEN_ON            0xFFFE 
// Brown-out Reset Enable bits
// Brown-out Reset enabled in hardware only (SBOREN is disabled)
#define BOREN_SBORDIS        0xFFFF 
// Brown-out Reset enabled in hardware only and disabled in Sleep mode (SBOREN is disabled)
#define BOREN_NOSLP          0xFFFD 
// Brown-out Reset enabled and controlled by software (SBOREN is enabled)
#define BOREN_ON             0xFFFB 
// Brown-out Reset disabled in hardware and software
#define BOREN_OFF            0xFFF9 
// Brown Out Reset Voltage bits
// VBOR set to 1.90 V nominal
#define BORV_190             0xFFFF 
// VBOR set to 2.20 V nominal
#define BORV_220             0xFFF7 
// VBOR set to 2.50 V nominal
#define BORV_250             0xFFEF 
// VBOR set to 2.85 V nominal
#define BORV_285             0xFFE7 


// Config Register: CONFIG2H
// Watchdog Timer Enable bits
// WDT is always enabled. SWDTEN bit has no effect
#define WDTEN_ON             0xFFFF 
// WDT is controlled by SWDTEN bit of the WDTCON register
#define WDTEN_SWON           0xFEFF 
// WDT is disabled in sleep, otherwise enabled. SWDTEN bit has no effect
#define WDTEN_NOSLP          0xFDFF 
// Watch dog timer is always disabled. SWDTEN has no effect.
#define WDTEN_OFF            0xFCFF 
// Watchdog Timer Postscale Select bits
// 1:32768
#define WDTPS_32768          0xFFFF 
// 1:16384
#define WDTPS_16384          0xFBFF 
// 1:8192
#define WDTPS_8192           0xF7FF 
// 1:4096
#define WDTPS_4096           0xF3FF 
// 1:2048
#define WDTPS_2048           0xEFFF 
// 1:1024
#define WDTPS_1024           0xEBFF 
// 1:512
#define WDTPS_512            0xE7FF 
// 1:256
#define WDTPS_256            0xE3FF 
// 1:128
#define WDTPS_128            0xDFFF 
// 1:64
#define WDTPS_64             0xDBFF 
// 1:32
#define WDTPS_32             0xD7FF 
// 1:16
#define WDTPS_16             0xD3FF 
// 1:8
#define WDTPS_8              0xCFFF 
// 1:4
#define WDTPS_4              0xCBFF 
// 1:2
#define WDTPS_2              0xC7FF 
// 1:1
#define WDTPS_1              0xC3FF 


// Config Register: CONFIG3H
// CCP2 MUX bit
// CCP2 input/output is multiplexed with RC1
#define CCP2MX_PORTC1        0xFFFF 
// CCP2 input/output is multiplexed with RB3
#define CCP2MX_PORTB3        0xFEFF 
// PORTB A/D Enable bit
// PORTB<5:0> pins are configured as analog input channels on Reset
#define PBADEN_ON            0xFFFF 
// PORTB<5:0> pins are configured as digital I/O on Reset
#define PBADEN_OFF           0xFDFF 
// P3A/CCP3 Mux bit
// P3A/CCP3 input/output is multiplexed with RB5
#define CCP3MX_PORTB5        0xFFFF 
// P3A/CCP3 input/output is mulitplexed with RC6
#define CCP3MX_PORTC6        0xFBFF 
// HFINTOSC Fast Start-up
// HFINTOSC output and ready status are not delayed by the oscillator stable status
#define HFOFST_ON            0xFFFF 
// HFINTOSC output and ready status are delayed by the oscillator stable status
#define HFOFST_OFF           0xF7FF 
// Timer3 Clock input mux bit
// T3CKI is on RC0
#define T3CMX_PORTC0         0xFFFF 
// T3CKI is on RB5
#define T3CMX_PORTB5         0xEFFF 
// ECCP2 B output mux bit
// P2B is on RB5
#define P2BMX_PORTB5         0xFFFF 
// P2B is on RC0
#define P2BMX_PORTC0         0xDFFF 
// MCLR Pin Enable bit
// MCLR pin enabled, RE3 input pin disabled
#define MCLRE_EXTMCLR        0xFFFF 
// RE3 input pin enabled; MCLR disabled
#define MCLRE_INTMCLR        0x7FFF 


// Config Register: CONFIG4L
// Stack Full/Underflow Reset Enable bit
// Stack full/underflow will cause Reset
#define STVREN_ON            0xFFFF 
// Stack full/underflow will not cause Reset
#define STVREN_OFF           0xFFFE 
// Single-Supply ICSP Enable bit
// Single-Supply ICSP enabled if MCLRE is also 1
#define LVP_ON               0xFFFF 
// Single-Supply ICSP disabled
#define LVP_OFF              0xFFFB 
// Extended Instruction Set Enable bit
// Instruction set extension and Indexed Addressing mode enabled
#define XINST_ON             0xFFBF 
// Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#define XINST_OFF            0xFFFF 
// Background Debug
// Disabled
#define DEBUG_OFF            0xFFFF 
// Enabled
#define DEBUG_ON             0xFF7F 


// Config Register: CONFIG5L
// Code Protection Block 0
// Block 0 (000800-001FFFh) not code-protected
#define CP0_OFF              0xFFFF 
// Block 0 (000800-001FFFh) code-protected
#define CP0_ON               0xFFFE 
// Code Protection Block 1
// Block 1 (002000-003FFFh) not code-protected
#define CP1_OFF              0xFFFF 
// Block 1 (002000-003FFFh) code-protected
#define CP1_ON               0xFFFD 


// Config Register: CONFIG5H
// Boot Block Code Protection bit
// Boot block (000000-0007FFh) not code-protected
#define CPB_OFF              0xFFFF 
// Boot block (000000-0007FFh) code-protected
#define CPB_ON               0xBFFF 
// Data EEPROM Code Protection bit
// Data EEPROM not code-protected
#define CPD_OFF              0xFFFF 
// Data EEPROM code-protected
#define CPD_ON               0x7FFF 


// Config Register: CONFIG6L
// Write Protection Block 0
// Block 0 (000800-001FFFh) not write-protected
#define WRT0_OFF             0xFFFF 
// Block 0 (000800-001FFFh) write-protected
#define WRT0_ON              0xFFFE 
// Write Protection Block 1
// Block 1 (002000-003FFFh) not write-protected
#define WRT1_OFF             0xFFFF 
// Block 1 (002000-003FFFh) write-protected
#define WRT1_ON              0xFFFD 


// Config Register: CONFIG6H
// Configuration Register Write Protection bit
// Configuration registers (300000-3000FFh) not write-protected
#define WRTC_OFF             0xFFFF 
// Configuration registers (300000-3000FFh) write-protected
#define WRTC_ON              0xDFFF 
// Boot Block Write Protection bit
// Boot Block (000000-0007FFh) not write-protected
#define WRTB_OFF             0xFFFF 
// Boot Block (000000-0007FFh) write-protected
#define WRTB_ON              0xBFFF 
// Data EEPROM Write Protection bit
// Data EEPROM not write-protected
#define WRTD_OFF             0xFFFF 
// Data EEPROM write-protected
#define WRTD_ON              0x7FFF 


// Config Register: CONFIG7L
// Table Read Protection Block 0
// Block 0 (000800-001FFFh) not protected from table reads executed in other blocks
#define EBTR0_OFF            0xFFFF 
// Block 0 (000800-001FFFh) protected from table reads executed in other blocks
#define EBTR0_ON             0xFFFE 
// Table Read Protection Block 1
// Block 1 (002000-003FFFh) not protected from table reads executed in other blocks
#define EBTR1_OFF            0xFFFF 
// Block 1 (002000-003FFFh) protected from table reads executed in other blocks
#define EBTR1_ON             0xFFFD 


// Config Register: CONFIG7H
// Boot Block Table Read Protection bit
// Boot Block (000000-0007FFh) not protected from table reads executed in other blocks
#define EBTRB_OFF            0xFFFF 
// Boot Block (000000-0007FFh) protected from table reads executed in other blocks
#define EBTRB_ON             0xBFFF 


//
// Special function register definitions: Bank 0xf
//

// Register: ANSELA
// PORTA analog select bits
extern volatile unsigned char           ANSELA              @ 0xF38;
// bit and bitfield definitions
// RA0 analog select
extern volatile bit ANSA0               @ ((unsigned)&ANSELA*8)+0;
// RA1 analog select
extern volatile bit ANSA1               @ ((unsigned)&ANSELA*8)+1;
// RA2 analog select
extern volatile bit ANSA2               @ ((unsigned)&ANSELA*8)+2;
// RA3 analog select
extern volatile bit ANSA3               @ ((unsigned)&ANSELA*8)+3;
// RA5 analog select
extern volatile bit ANSA5               @ ((unsigned)&ANSELA*8)+5;
extern union {
    struct {
        volatile unsigned ANSA0               : 1;
        volatile unsigned ANSA1               : 1;
        volatile unsigned ANSA2               : 1;
        volatile unsigned ANSA3               : 1;
        volatile unsigned                     : 1;
        volatile unsigned ANSA5               : 1;
    };
} ANSELAbits @ 0xF38;

// Register: ANSELB
// PORTB analog select bits
extern volatile unsigned char           ANSELB              @ 0xF39;
// bit and bitfield definitions
// RB0 analog select
extern volatile bit ANSB0               @ ((unsigned)&ANSELB*8)+0;
// RB1 analog select
extern volatile bit ANSB1               @ ((unsigned)&ANSELB*8)+1;
// RB2 analog select
extern volatile bit ANSB2               @ ((unsigned)&ANSELB*8)+2;
// RB3 analog select
extern volatile bit ANSB3               @ ((unsigned)&ANSELB*8)+3;
// RB4 analog select
extern volatile bit ANSB4               @ ((unsigned)&ANSELB*8)+4;
// RB5 analog select
extern volatile bit ANSB5               @ ((unsigned)&ANSELB*8)+5;
extern union {
    struct {
        volatile unsigned ANSB0               : 1;
        volatile unsigned ANSB1               : 1;
        volatile unsigned ANSB2               : 1;
        volatile unsigned ANSB3               : 1;
        volatile unsigned ANSB4               : 1;
        volatile unsigned ANSB5               : 1;
    };
} ANSELBbits @ 0xF39;

// Register: ANSELC
// PORTC analog select bits
extern volatile unsigned char           ANSELC              @ 0xF3A;
// bit and bitfield definitions
// RC2 analog select
extern volatile bit ANSC2               @ ((unsigned)&ANSELC*8)+2;
// RC3 analog select
extern volatile bit ANSC3               @ ((unsigned)&ANSELC*8)+3;
// RC4 analog select
extern volatile bit ANSC4               @ ((unsigned)&ANSELC*8)+4;
// RC5 analog select
extern volatile bit ANSC5               @ ((unsigned)&ANSELC*8)+5;
// RC6 analog select
extern volatile bit ANSC6               @ ((unsigned)&ANSELC*8)+6;
// RC7 analog select
extern volatile bit ANSC7               @ ((unsigned)&ANSELC*8)+7;
extern union {
    struct {
        volatile unsigned                     : 2;
        volatile unsigned ANSC2               : 1;
        volatile unsigned ANSC3               : 1;
        volatile unsigned ANSC4               : 1;
        volatile unsigned ANSC5               : 1;
        volatile unsigned ANSC6               : 1;
        volatile unsigned ANSC7               : 1;
    };
} ANSELCbits @ 0xF3A;

// Register: PMD2
// Power mode control bits
extern volatile unsigned char           PMD2                @ 0xF3D;
// bit and bitfield definitions
// ADC power mode control
extern volatile bit ADCMD               @ ((unsigned)&PMD2*8)+0;
// Comparator 1 power mode control
extern volatile bit CMP1MD              @ ((unsigned)&PMD2*8)+1;
// Comparator 2 power mode control
extern volatile bit CMP2MD              @ ((unsigned)&PMD2*8)+2;
// CTMU power mode control
extern volatile bit CTMUMD              @ ((unsigned)&PMD2*8)+3;
extern union {
    struct {
        volatile unsigned ADCMD               : 1;
        volatile unsigned CMP1MD              : 1;
        volatile unsigned CMP2MD              : 1;
        volatile unsigned CTMUMD              : 1;
    };
} PMD2bits @ 0xF3D;

// Register: PMD1
// Power mode control bits
extern volatile unsigned char           PMD1                @ 0xF3E;
// bit and bitfield definitions
// CCP1 power mode control
extern volatile bit CCP1MD              @ ((unsigned)&PMD1*8)+0;
// CCP2 power mode control
extern volatile bit CCP2MD              @ ((unsigned)&PMD1*8)+1;
// CCP3 power mode control
extern volatile bit CCP3MD              @ ((unsigned)&PMD1*8)+2;
// CCP4 power mode control
extern volatile bit CCP4MD              @ ((unsigned)&PMD1*8)+3;
// CCP5 power mode control
extern volatile bit CCP5MD              @ ((unsigned)&PMD1*8)+4;
// MSSP1 power mode control
extern volatile bit MSSP1MD             @ ((unsigned)&PMD1*8)+6;
// MSSP2 power mode control
extern volatile bit MSSP2MD             @ ((unsigned)&PMD1*8)+7;
extern union {
    struct {
        volatile unsigned CCP1MD              : 1;
        volatile unsigned CCP2MD              : 1;
        volatile unsigned CCP3MD              : 1;
        volatile unsigned CCP4MD              : 1;
        volatile unsigned CCP5MD              : 1;
        volatile unsigned                     : 1;
        volatile unsigned MSSP1MD             : 1;
        volatile unsigned MSSP2MD             : 1;
    };
} PMD1bits @ 0xF3E;

// Register: PMD0
// Power mode control bits
extern volatile unsigned char           PMD0                @ 0xF3F;
// bit and bitfield definitions
// Timer1 power mode control
extern volatile bit TMR1MD              @ ((unsigned)&PMD0*8)+0;
// Timer2 power mode control
extern volatile bit TMR2MD              @ ((unsigned)&PMD0*8)+1;
// Timer3 power mode control
extern volatile bit TMR3MD              @ ((unsigned)&PMD0*8)+2;
// Timer4 power mode control
extern volatile bit TMR4MD              @ ((unsigned)&PMD0*8)+3;
// Timer5 power mode control
extern volatile bit TMR5MD              @ ((unsigned)&PMD0*8)+4;
// Timer6 power mode control
extern volatile bit TMR6MD              @ ((unsigned)&PMD0*8)+5;
// UART1 power mode control
extern volatile bit UART1MD             @ ((unsigned)&PMD0*8)+6;
// UART2 power mode control
extern volatile bit UART2MD             @ ((unsigned)&PMD0*8)+7;
extern union {
    struct {
        volatile unsigned TMR1MD              : 1;
        volatile unsigned TMR2MD              : 1;
        volatile unsigned TMR3MD              : 1;
        volatile unsigned TMR4MD              : 1;
        volatile unsigned TMR5MD              : 1;
        volatile unsigned TMR6MD              : 1;
        volatile unsigned UART1MD             : 1;
        volatile unsigned UART2MD             : 1;
    };
} PMD0bits @ 0xF3F;

// Register: VREFCON2
// DAC voltage output select
extern volatile unsigned char           VREFCON2            @ 0xF40;
extern volatile unsigned char           DACCON1             @ 0xF40;
// bit and bitfield definitions
extern volatile bit DACR0               @ ((unsigned)&VREFCON2*8)+0;
extern volatile bit DACR1               @ ((unsigned)&VREFCON2*8)+1;
extern volatile bit DACR2               @ ((unsigned)&VREFCON2*8)+2;
extern volatile bit DACR3               @ ((unsigned)&VREFCON2*8)+3;
extern volatile bit DACR4               @ ((unsigned)&VREFCON2*8)+4;
extern union {
    struct {
        volatile unsigned DACR                : 5;
    };
    struct {
        volatile unsigned DACR0               : 1;
        volatile unsigned DACR1               : 1;
        volatile unsigned DACR2               : 1;
        volatile unsigned DACR3               : 1;
        volatile unsigned DACR4               : 1;
    };
} VREFCON2bits @ 0xF40;

// Register: VREFCON1
extern volatile unsigned char           VREFCON1            @ 0xF41;
extern volatile unsigned char           DACCON0             @ 0xF41;
// bit and bitfield definitions
// DAC1 negative source select
extern volatile bit DACNSS              @ ((unsigned)&VREFCON1*8)+0;
// DAC1 Voltage output enable
extern volatile bit DACOE               @ ((unsigned)&VREFCON1*8)+5;
// DAC1 low power voltage state select
extern volatile bit DACLPS              @ ((unsigned)&VREFCON1*8)+6;
// DAC1 Enable
extern volatile bit DACEN               @ ((unsigned)&VREFCON1*8)+7;
extern volatile bit DACPSS0             @ ((unsigned)&VREFCON1*8)+2;
extern volatile bit DACPSS1             @ ((unsigned)&VREFCON1*8)+3;
extern union {
    struct {
        volatile unsigned DACNSS              : 1;
        volatile unsigned                     : 1;
        volatile unsigned DACPSS              : 2;
        volatile unsigned : 1;
        volatile unsigned DACOE               : 1;
        volatile unsigned DACLPS              : 1;
        volatile unsigned DACEN               : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned DACPSS0             : 1;
        volatile unsigned DACPSS1             : 1;
    };
} VREFCON1bits @ 0xF41;

// Register: VREFCON0
extern volatile unsigned char           VREFCON0            @ 0xF42;
extern volatile unsigned char           FVRCON              @ 0xF42;
// bit and bitfield definitions
extern volatile bit FVRST               @ ((unsigned)&VREFCON0*8)+6;
extern volatile bit FVREN               @ ((unsigned)&VREFCON0*8)+7;
extern volatile bit FVRS0               @ ((unsigned)&VREFCON0*8)+4;
extern volatile bit FVRS1               @ ((unsigned)&VREFCON0*8)+5;
extern union {
    struct {
        volatile unsigned                     : 4;
        volatile unsigned FVRS                : 2;
        volatile unsigned FVRST               : 1;
        volatile unsigned FVREN               : 1;
    };
    struct {
        volatile unsigned : 4;
        volatile unsigned FVRS0               : 1;
        volatile unsigned FVRS1               : 1;
    };
} VREFCON0bits @ 0xF42;

// Register: CTMUICON
extern volatile unsigned char           CTMUICON            @ 0xF43;
extern volatile unsigned char           CTMUICONH           @ 0xF43;
// bit and bitfield definitions
extern volatile bit IRNG0               @ ((unsigned)&CTMUICON*8)+0;
extern volatile bit IRNG1               @ ((unsigned)&CTMUICON*8)+1;
extern volatile bit ITRIM0              @ ((unsigned)&CTMUICON*8)+2;
extern volatile bit ITRIM1              @ ((unsigned)&CTMUICON*8)+3;
extern volatile bit ITRIM2              @ ((unsigned)&CTMUICON*8)+4;
extern volatile bit ITRIM3              @ ((unsigned)&CTMUICON*8)+5;
extern volatile bit ITRIM4              @ ((unsigned)&CTMUICON*8)+6;
extern volatile bit ITRIM5              @ ((unsigned)&CTMUICON*8)+7;
extern union {
    struct {
        volatile unsigned IRNG                : 2;
        volatile unsigned ITRIM               : 6;
    };
    struct {
        volatile unsigned IRNG0               : 1;
        volatile unsigned IRNG1               : 1;
        volatile unsigned ITRIM0              : 1;
        volatile unsigned ITRIM1              : 1;
        volatile unsigned ITRIM2              : 1;
        volatile unsigned ITRIM3              : 1;
        volatile unsigned ITRIM4              : 1;
        volatile unsigned ITRIM5              : 1;
    };
} CTMUICONbits @ 0xF43;

// Register: CTMUCONL
extern volatile unsigned char           CTMUCONL            @ 0xF44;
extern volatile unsigned char           CTMUCON1            @ 0xF44;
// bit and bitfield definitions
extern volatile bit EDG1STAT            @ ((unsigned)&CTMUCONL*8)+0;
extern volatile bit EDG2STAT            @ ((unsigned)&CTMUCONL*8)+1;
extern volatile bit EDG1POL             @ ((unsigned)&CTMUCONL*8)+4;
extern volatile bit EDG2POL             @ ((unsigned)&CTMUCONL*8)+7;
extern volatile bit EDG1SEL0            @ ((unsigned)&CTMUCONL*8)+2;
extern volatile bit EDG1SEL1            @ ((unsigned)&CTMUCONL*8)+3;
extern volatile bit EDG2SEL0            @ ((unsigned)&CTMUCONL*8)+5;
extern volatile bit EDG2SEL1            @ ((unsigned)&CTMUCONL*8)+6;
extern union {
    struct {
        volatile unsigned EDG1STAT            : 1;
        volatile unsigned EDG2STAT            : 1;
        volatile unsigned EDG1SEL             : 2;
        volatile unsigned EDG1POL             : 1;
        volatile unsigned EDG2SEL             : 2;
        volatile unsigned EDG2POL             : 1;
    };
    struct {
        volatile unsigned                     : 2;
        volatile unsigned EDG1SEL0            : 1;
        volatile unsigned EDG1SEL1            : 1;
        volatile unsigned : 1;
        volatile unsigned EDG2SEL0            : 1;
        volatile unsigned EDG2SEL1            : 1;
    };
} CTMUCONLbits @ 0xF44;

// Register: CTMUCONH
extern volatile unsigned char           CTMUCONH            @ 0xF45;
extern volatile unsigned char           CTMUCON0            @ 0xF45;
// bit and bitfield definitions
extern volatile bit CTTRIG              @ ((unsigned)&CTMUCONH*8)+0;
extern volatile bit IDISSEN             @ ((unsigned)&CTMUCONH*8)+1;
extern volatile bit EDGSEQEN            @ ((unsigned)&CTMUCONH*8)+2;
extern volatile bit EDGEN               @ ((unsigned)&CTMUCONH*8)+3;
extern volatile bit TGEN                @ ((unsigned)&CTMUCONH*8)+4;
extern volatile bit CTMUSIDL            @ ((unsigned)&CTMUCONH*8)+5;
extern volatile bit CTMUEN              @ ((unsigned)&CTMUCONH*8)+7;
extern union {
    struct {
        volatile unsigned CTTRIG              : 1;
        volatile unsigned IDISSEN             : 1;
        volatile unsigned EDGSEQEN            : 1;
        volatile unsigned EDGEN               : 1;
        volatile unsigned TGEN                : 1;
        volatile unsigned CTMUSIDL            : 1;
        volatile unsigned                     : 1;
        volatile unsigned CTMUEN              : 1;
    };
} CTMUCONHbits @ 0xF45;

// Register: SRCON1
extern volatile unsigned char           SRCON1              @ 0xF46;
// bit and bitfield definitions
extern volatile bit SRRC1E              @ ((unsigned)&SRCON1*8)+0;
extern volatile bit SRRC2E              @ ((unsigned)&SRCON1*8)+1;
extern volatile bit SRRCKE              @ ((unsigned)&SRCON1*8)+2;
extern volatile bit SRRPE               @ ((unsigned)&SRCON1*8)+3;
extern volatile bit SRSC1E              @ ((unsigned)&SRCON1*8)+4;
extern volatile bit SRSC2E              @ ((unsigned)&SRCON1*8)+5;
extern volatile bit SRSCKE              @ ((unsigned)&SRCON1*8)+6;
extern volatile bit SRSPE               @ ((unsigned)&SRCON1*8)+7;
extern union {
    struct {
        volatile unsigned SRRC1E              : 1;
        volatile unsigned SRRC2E              : 1;
        volatile unsigned SRRCKE              : 1;
        volatile unsigned SRRPE               : 1;
        volatile unsigned SRSC1E              : 1;
        volatile unsigned SRSC2E              : 1;
        volatile unsigned SRSCKE              : 1;
        volatile unsigned SRSPE               : 1;
    };
} SRCON1bits @ 0xF46;

// Register: SRCON0
extern volatile unsigned char           SRCON0              @ 0xF47;
// bit and bitfield definitions
extern volatile bit SRPR                @ ((unsigned)&SRCON0*8)+0;
extern volatile bit SRPS                @ ((unsigned)&SRCON0*8)+1;
extern volatile bit SRNQEN              @ ((unsigned)&SRCON0*8)+2;
extern volatile bit SRQEN               @ ((unsigned)&SRCON0*8)+3;
extern volatile bit SRLEN               @ ((unsigned)&SRCON0*8)+7;
extern volatile bit SRCLK0              @ ((unsigned)&SRCON0*8)+4;
extern volatile bit SRCLK1              @ ((unsigned)&SRCON0*8)+5;
extern volatile bit SRCLK2              @ ((unsigned)&SRCON0*8)+6;
extern union {
    struct {
        volatile unsigned SRPR                : 1;
        volatile unsigned SRPS                : 1;
        volatile unsigned SRNQEN              : 1;
        volatile unsigned SRQEN               : 1;
        volatile unsigned SRCLK               : 3;
        volatile unsigned SRLEN               : 1;
    };
    struct {
        volatile unsigned                     : 4;
        volatile unsigned SRCLK0              : 1;
        volatile unsigned SRCLK1              : 1;
        volatile unsigned SRCLK2              : 1;
    };
} SRCON0bits @ 0xF47;

// Register: CCPTMRS1
extern volatile unsigned char           CCPTMRS1            @ 0xF48;
// bit and bitfield definitions
extern volatile bit C4TSEL0             @ ((unsigned)&CCPTMRS1*8)+0;
extern volatile bit C4TSEL1             @ ((unsigned)&CCPTMRS1*8)+1;
extern volatile bit C5TSEL0             @ ((unsigned)&CCPTMRS1*8)+2;
extern volatile bit C5TSEL1             @ ((unsigned)&CCPTMRS1*8)+3;
extern union {
    struct {
        volatile unsigned C4TSEL              : 2;
        volatile unsigned C5TSEL              : 2;
    };
    struct {
        volatile unsigned C4TSEL0             : 1;
        volatile unsigned C4TSEL1             : 1;
        volatile unsigned C5TSEL0             : 1;
        volatile unsigned C5TSEL1             : 1;
    };
} CCPTMRS1bits @ 0xF48;

// Register: CCPTMRS0
extern volatile unsigned char           CCPTMRS0            @ 0xF49;
// bit and bitfield definitions
extern volatile bit C1TSEL0             @ ((unsigned)&CCPTMRS0*8)+0;
extern volatile bit C1TSEL1             @ ((unsigned)&CCPTMRS0*8)+1;
extern volatile bit C2TSEL0             @ ((unsigned)&CCPTMRS0*8)+3;
extern volatile bit C2TSEL1             @ ((unsigned)&CCPTMRS0*8)+4;
extern volatile bit C3TSEL0             @ ((unsigned)&CCPTMRS0*8)+6;
extern volatile bit C3TSEL1             @ ((unsigned)&CCPTMRS0*8)+7;
extern union {
    struct {
        volatile unsigned C1TSEL              : 2;
        volatile unsigned                     : 1;
        volatile unsigned C2TSEL              : 2;
        volatile unsigned : 1;
        volatile unsigned C3TSEL              : 2;
    };
    struct {
        volatile unsigned C1TSEL0             : 1;
        volatile unsigned C1TSEL1             : 1;
        volatile unsigned : 1;
        volatile unsigned C2TSEL0             : 1;
        volatile unsigned C2TSEL1             : 1;
        volatile unsigned : 1;
        volatile unsigned C3TSEL0             : 1;
        volatile unsigned C3TSEL1             : 1;
    };
} CCPTMRS0bits @ 0xF49;

// Register: T6CON
extern volatile unsigned char           T6CON               @ 0xF4A;
// bit and bitfield definitions
extern volatile bit TMR6ON              @ ((unsigned)&T6CON*8)+2;
extern volatile bit T6CKPS0             @ ((unsigned)&T6CON*8)+0;
extern volatile bit T6CKPS1             @ ((unsigned)&T6CON*8)+1;
extern volatile bit T6OUTPS0            @ ((unsigned)&T6CON*8)+3;
extern volatile bit T6OUTPS1            @ ((unsigned)&T6CON*8)+4;
extern volatile bit T6OUTPS2            @ ((unsigned)&T6CON*8)+5;
extern volatile bit T6OUTPS3            @ ((unsigned)&T6CON*8)+6;
extern union {
    struct {
        volatile unsigned T6CKPS              : 2;
        volatile unsigned TMR6ON              : 1;
        volatile unsigned T6OUTPS             : 4;
    };
    struct {
        volatile unsigned T6CKPS0             : 1;
        volatile unsigned T6CKPS1             : 1;
        volatile unsigned                     : 1;
        volatile unsigned T6OUTPS0            : 1;
        volatile unsigned T6OUTPS1            : 1;
        volatile unsigned T6OUTPS2            : 1;
        volatile unsigned T6OUTPS3            : 1;
    };
} T6CONbits @ 0xF4A;

// Register: PR6
extern volatile unsigned char           PR6                 @ 0xF4B;
// bit and bitfield definitions

// Register: TMR6
extern volatile unsigned char           TMR6                @ 0xF4C;
// bit and bitfield definitions

// Register: T5GCON
extern volatile unsigned char           T5GCON              @ 0xF4D;
// bit and bitfield definitions
extern volatile bit T5GVAL              @ ((unsigned)&T5GCON*8)+2;
extern volatile bit T5GGO_nDONE         @ ((unsigned)&T5GCON*8)+3;
extern volatile bit T5GSPM              @ ((unsigned)&T5GCON*8)+4;
extern volatile bit T5GTM               @ ((unsigned)&T5GCON*8)+5;
extern volatile bit T5GPOL              @ ((unsigned)&T5GCON*8)+6;
extern volatile bit TMR5GE              @ ((unsigned)&T5GCON*8)+7;
extern volatile bit T5GSS0              @ ((unsigned)&T5GCON*8)+0;
extern volatile bit T5GSS1              @ ((unsigned)&T5GCON*8)+1;
extern volatile bit T5GGO               @ ((unsigned)&T5GCON*8)+3;
extern volatile bit T5G_DONE            @ ((unsigned)&T5GCON*8)+3;
extern union {
    struct {
        volatile unsigned T5GSS               : 2;
        volatile unsigned T5GVAL              : 1;
        volatile unsigned T5GGO_nDONE         : 1;
        volatile unsigned T5GSPM              : 1;
        volatile unsigned T5GTM               : 1;
        volatile unsigned T5GPOL              : 1;
        volatile unsigned TMR5GE              : 1;
    };
    struct {
        volatile unsigned T5GSS0              : 1;
        volatile unsigned T5GSS1              : 1;
        volatile unsigned                     : 1;
        volatile unsigned T5GGO               : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
    };
    struct {
        volatile unsigned : 3;
        volatile unsigned T5G_DONE            : 1;
    };
} T5GCONbits @ 0xF4D;

// Register: T5CON
extern volatile unsigned char           T5CON               @ 0xF4E;
// bit and bitfield definitions
extern volatile bit TMR5ON              @ ((unsigned)&T5CON*8)+0;
extern volatile bit T5RD16              @ ((unsigned)&T5CON*8)+1;
extern volatile bit nT5SYNC             @ ((unsigned)&T5CON*8)+2;
extern volatile bit T5SOSCEN            @ ((unsigned)&T5CON*8)+3;
extern volatile bit T5SYNC              @ ((unsigned)&T5CON*8)+2;
extern volatile bit T5CKPS0             @ ((unsigned)&T5CON*8)+4;
extern volatile bit T5CKPS1             @ ((unsigned)&T5CON*8)+5;
extern volatile bit TMR5CS0             @ ((unsigned)&T5CON*8)+6;
extern volatile bit TMR5CS1             @ ((unsigned)&T5CON*8)+7;
extern union {
    struct {
        volatile unsigned TMR5ON              : 1;
        volatile unsigned T5RD16              : 1;
        volatile unsigned nT5SYNC             : 1;
        volatile unsigned T5SOSCEN            : 1;
        volatile unsigned T5CKPS              : 2;
        volatile unsigned TMR5CS              : 2;
    };
    struct {
        volatile unsigned                     : 1;
        volatile unsigned : 1;
        volatile unsigned T5SYNC              : 1;
        volatile unsigned : 1;
        volatile unsigned T5CKPS0             : 1;
        volatile unsigned T5CKPS1             : 1;
        volatile unsigned TMR5CS0             : 1;
        volatile unsigned TMR5CS1             : 1;
    };
} T5CONbits @ 0xF4E;
// bit and bitfield definitions

// Register: TMR5L
extern volatile unsigned char           TMR5L               @ 0xF4F;
// bit and bitfield definitions

// Register: TMR5H
extern volatile unsigned char           TMR5H               @ 0xF50;
// bit and bitfield definitions

// Register: TMR5
extern volatile unsigned int            TMR5                @ 0xF4F;

// Register: T4CON
extern volatile unsigned char           T4CON               @ 0xF51;
// bit and bitfield definitions
extern volatile bit TMR4ON              @ ((unsigned)&T4CON*8)+2;
extern volatile bit T4CKPS0             @ ((unsigned)&T4CON*8)+0;
extern volatile bit T4CKPS1             @ ((unsigned)&T4CON*8)+1;
extern volatile bit T4OUTPS0            @ ((unsigned)&T4CON*8)+3;
extern volatile bit T4OUTPS1            @ ((unsigned)&T4CON*8)+4;
extern volatile bit T4OUTPS2            @ ((unsigned)&T4CON*8)+5;
extern volatile bit T4OUTPS3            @ ((unsigned)&T4CON*8)+6;
extern union {
    struct {
        volatile unsigned T4CKPS              : 2;
        volatile unsigned TMR4ON              : 1;
        volatile unsigned T4OUTPS             : 4;
    };
    struct {
        volatile unsigned T4CKPS0             : 1;
        volatile unsigned T4CKPS1             : 1;
        volatile unsigned                     : 1;
        volatile unsigned T4OUTPS0            : 1;
        volatile unsigned T4OUTPS1            : 1;
        volatile unsigned T4OUTPS2            : 1;
        volatile unsigned T4OUTPS3            : 1;
    };
} T4CONbits @ 0xF51;

// Register: PR4
extern volatile unsigned char           PR4                 @ 0xF52;
// bit and bitfield definitions

// Register: TMR4
extern volatile unsigned char           TMR4                @ 0xF53;
// bit and bitfield definitions

// Register: CCP5CON
extern volatile unsigned char           CCP5CON             @ 0xF54;
// bit and bitfield definitions
extern volatile bit CCP5M0              @ ((unsigned)&CCP5CON*8)+0;
extern volatile bit CCP5M1              @ ((unsigned)&CCP5CON*8)+1;
extern volatile bit CCP5M2              @ ((unsigned)&CCP5CON*8)+2;
extern volatile bit CCP5M3              @ ((unsigned)&CCP5CON*8)+3;
extern volatile bit DC5B0               @ ((unsigned)&CCP5CON*8)+4;
extern volatile bit DC5B1               @ ((unsigned)&CCP5CON*8)+5;
extern union {
    struct {
        volatile unsigned CCP5M               : 4;
        volatile unsigned DC5B                : 2;
    };
    struct {
        volatile unsigned CCP5M0              : 1;
        volatile unsigned CCP5M1              : 1;
        volatile unsigned CCP5M2              : 1;
        volatile unsigned CCP5M3              : 1;
        volatile unsigned DC5B0               : 1;
        volatile unsigned DC5B1               : 1;
    };
} CCP5CONbits @ 0xF54;
// bit and bitfield definitions

// Register: CCPR5L
extern volatile unsigned char           CCPR5L              @ 0xF55;
// bit and bitfield definitions

// Register: CCPR5H
extern volatile unsigned char           CCPR5H              @ 0xF56;
// bit and bitfield definitions

// Register: CCPR5
extern volatile unsigned int            CCPR5               @ 0xF55;

// Register: CCP4CON
extern volatile unsigned char           CCP4CON             @ 0xF57;
// bit and bitfield definitions
extern volatile bit CCP4M0              @ ((unsigned)&CCP4CON*8)+0;
extern volatile bit CCP4M1              @ ((unsigned)&CCP4CON*8)+1;
extern volatile bit CCP4M2              @ ((unsigned)&CCP4CON*8)+2;
extern volatile bit CCP4M3              @ ((unsigned)&CCP4CON*8)+3;
extern volatile bit DC4B0               @ ((unsigned)&CCP4CON*8)+4;
extern volatile bit DC4B1               @ ((unsigned)&CCP4CON*8)+5;
extern union {
    struct {
        volatile unsigned CCP4M               : 4;
        volatile unsigned DC4B                : 2;
    };
    struct {
        volatile unsigned CCP4M0              : 1;
        volatile unsigned CCP4M1              : 1;
        volatile unsigned CCP4M2              : 1;
        volatile unsigned CCP4M3              : 1;
        volatile unsigned DC4B0               : 1;
        volatile unsigned DC4B1               : 1;
    };
} CCP4CONbits @ 0xF57;
// bit and bitfield definitions

// Register: CCPR4L
extern volatile unsigned char           CCPR4L              @ 0xF58;
// bit and bitfield definitions

// Register: CCPR4H
extern volatile unsigned char           CCPR4H              @ 0xF59;
// bit and bitfield definitions

// Register: CCPR4
extern volatile unsigned int            CCPR4               @ 0xF58;

// Register: PSTR3CON
extern volatile unsigned char           PSTR3CON            @ 0xF5A;
// bit and bitfield definitions
extern volatile bit STR3A               @ ((unsigned)&PSTR3CON*8)+0;
extern volatile bit STR3B               @ ((unsigned)&PSTR3CON*8)+1;
extern volatile bit STR3C               @ ((unsigned)&PSTR3CON*8)+2;
extern volatile bit STR3D               @ ((unsigned)&PSTR3CON*8)+3;
extern volatile bit STR3SYNC            @ ((unsigned)&PSTR3CON*8)+4;
extern union {
    struct {
        volatile unsigned STR3A               : 1;
        volatile unsigned STR3B               : 1;
        volatile unsigned STR3C               : 1;
        volatile unsigned STR3D               : 1;
        volatile unsigned STR3SYNC            : 1;
    };
} PSTR3CONbits @ 0xF5A;

// Register: ECCP3AS
extern volatile unsigned char           ECCP3AS             @ 0xF5B;
extern volatile unsigned char           CCP3AS              @ 0xF5B;
// bit and bitfield definitions
extern volatile bit CCP3ASE             @ ((unsigned)&ECCP3AS*8)+7;
extern volatile bit P3SSBD0             @ ((unsigned)&ECCP3AS*8)+0;
extern volatile bit P3SSBD1             @ ((unsigned)&ECCP3AS*8)+1;
extern volatile bit P3SSAC0             @ ((unsigned)&ECCP3AS*8)+2;
extern volatile bit P3SSAC1             @ ((unsigned)&ECCP3AS*8)+3;
extern volatile bit CCP3AS0             @ ((unsigned)&ECCP3AS*8)+4;
extern volatile bit CCP3AS1             @ ((unsigned)&ECCP3AS*8)+5;
extern volatile bit CCP3AS2             @ ((unsigned)&ECCP3AS*8)+6;
extern volatile bit PSS3BD0             @ ((unsigned)&ECCP3AS*8)+0;
extern volatile bit PSS3BD1             @ ((unsigned)&ECCP3AS*8)+1;
extern volatile bit PSS3AC0             @ ((unsigned)&ECCP3AS*8)+2;
extern volatile bit PSS3AC1             @ ((unsigned)&ECCP3AS*8)+3;
extern union {
    struct {
        volatile unsigned P3SSBD              : 2;
        volatile unsigned P3SSAC              : 2;
        volatile unsigned CCP3AS              : 3;
        volatile unsigned CCP3ASE             : 1;
    };
    struct {
        volatile unsigned P3SSBD0             : 1;
        volatile unsigned P3SSBD1             : 1;
        volatile unsigned P3SSAC0             : 1;
        volatile unsigned P3SSAC1             : 1;
        volatile unsigned CCP3AS0             : 1;
        volatile unsigned CCP3AS1             : 1;
        volatile unsigned CCP3AS2             : 1;
        volatile unsigned                     : 1;
    };
    struct {
        volatile unsigned PSS3BD              : 2;
        volatile unsigned PSS3AC              : 2;
    };
    struct {
        volatile unsigned PSS3BD0             : 1;
        volatile unsigned PSS3BD1             : 1;
        volatile unsigned PSS3AC0             : 1;
        volatile unsigned PSS3AC1             : 1;
    };
} ECCP3ASbits @ 0xF5B;

// Register: PWM3CON
extern volatile unsigned char           PWM3CON             @ 0xF5C;
// bit and bitfield definitions
extern volatile bit P3RSEN              @ ((unsigned)&PWM3CON*8)+7;
extern volatile bit P3DC0               @ ((unsigned)&PWM3CON*8)+0;
extern volatile bit P3DC1               @ ((unsigned)&PWM3CON*8)+1;
extern volatile bit P3DC2               @ ((unsigned)&PWM3CON*8)+2;
extern volatile bit P3DC3               @ ((unsigned)&PWM3CON*8)+3;
extern volatile bit P3DC4               @ ((unsigned)&PWM3CON*8)+4;
extern volatile bit P3DC5               @ ((unsigned)&PWM3CON*8)+5;
extern volatile bit P3DC6               @ ((unsigned)&PWM3CON*8)+6;
extern union {
    struct {
        volatile unsigned P3DC                : 7;
        volatile unsigned P3RSEN              : 1;
    };
    struct {
        volatile unsigned P3DC0               : 1;
        volatile unsigned P3DC1               : 1;
        volatile unsigned P3DC2               : 1;
        volatile unsigned P3DC3               : 1;
        volatile unsigned P3DC4               : 1;
        volatile unsigned P3DC5               : 1;
        volatile unsigned P3DC6               : 1;
    };
} PWM3CONbits @ 0xF5C;

// Register: CCP3CON
extern volatile unsigned char           CCP3CON             @ 0xF5D;
// bit and bitfield definitions
extern volatile bit CCP3M0              @ ((unsigned)&CCP3CON*8)+0;
extern volatile bit CCP3M1              @ ((unsigned)&CCP3CON*8)+1;
extern volatile bit CCP3M2              @ ((unsigned)&CCP3CON*8)+2;
extern volatile bit CCP3M3              @ ((unsigned)&CCP3CON*8)+3;
extern volatile bit DC3B0               @ ((unsigned)&CCP3CON*8)+4;
extern volatile bit DC3B1               @ ((unsigned)&CCP3CON*8)+5;
extern volatile bit P3M0                @ ((unsigned)&CCP3CON*8)+6;
extern volatile bit P3M1                @ ((unsigned)&CCP3CON*8)+7;
extern union {
    struct {
        volatile unsigned CCP3M               : 4;
        volatile unsigned DC3B                : 2;
        volatile unsigned P3M                 : 2;
    };
    struct {
        volatile unsigned CCP3M0              : 1;
        volatile unsigned CCP3M1              : 1;
        volatile unsigned CCP3M2              : 1;
        volatile unsigned CCP3M3              : 1;
        volatile unsigned DC3B0               : 1;
        volatile unsigned DC3B1               : 1;
        volatile unsigned P3M0                : 1;
        volatile unsigned P3M1                : 1;
    };
} CCP3CONbits @ 0xF5D;
// bit and bitfield definitions

// Register: CCPR3L
extern volatile unsigned char           CCPR3L              @ 0xF5E;
// bit and bitfield definitions

// Register: CCPR3H
extern volatile unsigned char           CCPR3H              @ 0xF5F;
// bit and bitfield definitions

// Register: CCPR3
extern volatile unsigned int            CCPR3               @ 0xF5E;

//
// Special function register definitions: Bank 0xf
//

// Register: SLRCON
extern volatile unsigned char           SLRCON              @ 0xF60;
// bit and bitfield definitions
extern volatile bit SLRA                @ ((unsigned)&SLRCON*8)+0;
extern volatile bit SLRB                @ ((unsigned)&SLRCON*8)+1;
extern volatile bit SLRC                @ ((unsigned)&SLRCON*8)+2;
extern union {
    struct {
        volatile unsigned SLRA                : 1;
        volatile unsigned SLRB                : 1;
        volatile unsigned SLRC                : 1;
        volatile unsigned                     : 1;
        volatile unsigned : 1;
    };
} SLRCONbits @ 0xF60;

// Register: WPUB
extern volatile unsigned char           WPUB                @ 0xF61;
// bit and bitfield definitions
extern volatile bit WPUB0               @ ((unsigned)&WPUB*8)+0;
extern volatile bit WPUB1               @ ((unsigned)&WPUB*8)+1;
extern volatile bit WPUB2               @ ((unsigned)&WPUB*8)+2;
extern volatile bit WPUB3               @ ((unsigned)&WPUB*8)+3;
extern volatile bit WPUB4               @ ((unsigned)&WPUB*8)+4;
extern volatile bit WPUB5               @ ((unsigned)&WPUB*8)+5;
extern volatile bit WPUB6               @ ((unsigned)&WPUB*8)+6;
extern volatile bit WPUB7               @ ((unsigned)&WPUB*8)+7;
extern union {
    struct {
        volatile unsigned WPUB0               : 1;
        volatile unsigned WPUB1               : 1;
        volatile unsigned WPUB2               : 1;
        volatile unsigned WPUB3               : 1;
        volatile unsigned WPUB4               : 1;
        volatile unsigned WPUB5               : 1;
        volatile unsigned WPUB6               : 1;
        volatile unsigned WPUB7               : 1;
    };
} WPUBbits @ 0xF61;

// Register: IOCB
extern volatile unsigned char           IOCB                @ 0xF62;
// bit and bitfield definitions
extern volatile bit IOCB4               @ ((unsigned)&IOCB*8)+4;
extern volatile bit IOCB5               @ ((unsigned)&IOCB*8)+5;
extern volatile bit IOCB6               @ ((unsigned)&IOCB*8)+6;
extern volatile bit IOCB7               @ ((unsigned)&IOCB*8)+7;
extern union {
    struct {
        volatile unsigned                     : 4;
        volatile unsigned IOCB4               : 1;
        volatile unsigned IOCB5               : 1;
        volatile unsigned IOCB6               : 1;
        volatile unsigned IOCB7               : 1;
    };
} IOCBbits @ 0xF62;

// Register: PSTR2CON
extern volatile unsigned char           PSTR2CON            @ 0xF63;
// bit and bitfield definitions
extern volatile bit STR2A               @ ((unsigned)&PSTR2CON*8)+0;
extern volatile bit STR2B               @ ((unsigned)&PSTR2CON*8)+1;
extern volatile bit STR2C               @ ((unsigned)&PSTR2CON*8)+2;
extern volatile bit STR2D               @ ((unsigned)&PSTR2CON*8)+3;
extern volatile bit STR2SYNC            @ ((unsigned)&PSTR2CON*8)+4;
extern union {
    struct {
        volatile unsigned STR2A               : 1;
        volatile unsigned STR2B               : 1;
        volatile unsigned STR2C               : 1;
        volatile unsigned STR2D               : 1;
        volatile unsigned STR2SYNC            : 1;
    };
} PSTR2CONbits @ 0xF63;

// Register: ECCP2AS
extern volatile unsigned char           ECCP2AS             @ 0xF64;
extern volatile unsigned char           CCP2AS              @ 0xF64;
// bit and bitfield definitions
extern volatile bit CCP2ASE             @ ((unsigned)&ECCP2AS*8)+7;
extern volatile bit P2SSBD0             @ ((unsigned)&ECCP2AS*8)+0;
extern volatile bit P2SSBD1             @ ((unsigned)&ECCP2AS*8)+1;
extern volatile bit P2SSAC0             @ ((unsigned)&ECCP2AS*8)+2;
extern volatile bit P2SSAC1             @ ((unsigned)&ECCP2AS*8)+3;
extern volatile bit CCP2AS0             @ ((unsigned)&ECCP2AS*8)+4;
extern volatile bit CCP2AS1             @ ((unsigned)&ECCP2AS*8)+5;
extern volatile bit CCP2AS2             @ ((unsigned)&ECCP2AS*8)+6;
extern volatile bit PSS2BD0             @ ((unsigned)&ECCP2AS*8)+0;
extern volatile bit PSS2BD1             @ ((unsigned)&ECCP2AS*8)+1;
extern volatile bit PSS2AC0             @ ((unsigned)&ECCP2AS*8)+2;
extern volatile bit PSS2AC1             @ ((unsigned)&ECCP2AS*8)+3;
extern union {
    struct {
        volatile unsigned P2SSBD              : 2;
        volatile unsigned P2SSAC              : 2;
        volatile unsigned CCP2AS              : 3;
        volatile unsigned CCP2ASE             : 1;
    };
    struct {
        volatile unsigned P2SSBD0             : 1;
        volatile unsigned P2SSBD1             : 1;
        volatile unsigned P2SSAC0             : 1;
        volatile unsigned P2SSAC1             : 1;
        volatile unsigned CCP2AS0             : 1;
        volatile unsigned CCP2AS1             : 1;
        volatile unsigned CCP2AS2             : 1;
    };
    struct {
        volatile unsigned PSS2BD              : 2;
        volatile unsigned PSS2AC              : 2;
    };
    struct {
        volatile unsigned PSS2BD0             : 1;
        volatile unsigned PSS2BD1             : 1;
        volatile unsigned PSS2AC0             : 1;
        volatile unsigned PSS2AC1             : 1;
    };
} ECCP2ASbits @ 0xF64;

// Register: PWM2CON
extern volatile unsigned char           PWM2CON             @ 0xF65;
// bit and bitfield definitions
extern volatile bit P2RSEN              @ ((unsigned)&PWM2CON*8)+7;
extern volatile bit P2DC0               @ ((unsigned)&PWM2CON*8)+0;
extern volatile bit P2DC1               @ ((unsigned)&PWM2CON*8)+1;
extern volatile bit P2DC2               @ ((unsigned)&PWM2CON*8)+2;
extern volatile bit P2DC3               @ ((unsigned)&PWM2CON*8)+3;
extern volatile bit P2DC4               @ ((unsigned)&PWM2CON*8)+4;
extern volatile bit P2DC5               @ ((unsigned)&PWM2CON*8)+5;
extern volatile bit P2DC6               @ ((unsigned)&PWM2CON*8)+6;
extern union {
    struct {
        volatile unsigned P2DC                : 7;
        volatile unsigned P2RSEN              : 1;
    };
    struct {
        volatile unsigned P2DC0               : 1;
        volatile unsigned P2DC1               : 1;
        volatile unsigned P2DC2               : 1;
        volatile unsigned P2DC3               : 1;
        volatile unsigned P2DC4               : 1;
        volatile unsigned P2DC5               : 1;
        volatile unsigned P2DC6               : 1;
    };
} PWM2CONbits @ 0xF65;

// Register: CCP2CON
extern volatile unsigned char           CCP2CON             @ 0xF66;
// bit and bitfield definitions
extern volatile bit P2M0                @ ((unsigned)&CCP2CON*8)+6;
extern volatile bit P2M1                @ ((unsigned)&CCP2CON*8)+7;
extern volatile bit CCP2M0              @ ((unsigned)&CCP2CON*8)+0;
extern volatile bit CCP2M1              @ ((unsigned)&CCP2CON*8)+1;
extern volatile bit CCP2M2              @ ((unsigned)&CCP2CON*8)+2;
extern volatile bit CCP2M3              @ ((unsigned)&CCP2CON*8)+3;
extern volatile bit DC2B0               @ ((unsigned)&CCP2CON*8)+4;
extern volatile bit DC2B1               @ ((unsigned)&CCP2CON*8)+5;
extern union {
    struct {
        volatile unsigned CCP2M               : 4;
        volatile unsigned DC2B                : 2;
        volatile unsigned P2M0                : 1;
        volatile unsigned P2M1                : 1;
    };
    struct {
        volatile unsigned CCP2M0              : 1;
        volatile unsigned CCP2M1              : 1;
        volatile unsigned CCP2M2              : 1;
        volatile unsigned CCP2M3              : 1;
        volatile unsigned DC2B0               : 1;
        volatile unsigned DC2B1               : 1;
    };
} CCP2CONbits @ 0xF66;
// bit and bitfield definitions

// Register: CCPR2L
extern volatile unsigned char           CCPR2L              @ 0xF67;
// bit and bitfield definitions

// Register: CCPR2H
extern volatile unsigned char           CCPR2H              @ 0xF68;
// bit and bitfield definitions

// Register: CCPR2
extern volatile unsigned int            CCPR2               @ 0xF67;

// Register: SSP2CON3
extern volatile unsigned char           SSP2CON3            @ 0xF69;
// bit and bitfield definitions
//extern volatile bit DHEN               @ ((unsigned)&SSP2CON3*8)+0;
//extern volatile bit AHEN               @ ((unsigned)&SSP2CON3*8)+1;
//extern volatile bit SBCDE              @ ((unsigned)&SSP2CON3*8)+2;
//extern volatile bit SDAHT              @ ((unsigned)&SSP2CON3*8)+3;
//extern volatile bit BOEN               @ ((unsigned)&SSP2CON3*8)+4;
//extern volatile bit SCIE               @ ((unsigned)&SSP2CON3*8)+5;
//extern volatile bit PCIE               @ ((unsigned)&SSP2CON3*8)+6;
//extern volatile bit ACKTIM             @ ((unsigned)&SSP2CON3*8)+7;
extern union {
    struct {
        volatile unsigned DHEN                : 1;
        volatile unsigned AHEN                : 1;
        volatile unsigned SBCDE               : 1;
        volatile unsigned SDAHT               : 1;
        volatile unsigned BOEN                : 1;
        volatile unsigned SCIE                : 1;
        volatile unsigned PCIE                : 1;
        volatile unsigned ACKTIM              : 1;
    };
} SSP2CON3bits @ 0xF69;

// Register: SSP2MSK
extern volatile unsigned char           SSP2MSK             @ 0xF6A;
// bit and bitfield definitions
//extern volatile bit MSK0               @ ((unsigned)&SSP2MSK*8)+0;
//extern volatile bit MSK1               @ ((unsigned)&SSP2MSK*8)+1;
//extern volatile bit MSK2               @ ((unsigned)&SSP2MSK*8)+2;
//extern volatile bit MSK3               @ ((unsigned)&SSP2MSK*8)+3;
//extern volatile bit MSK4               @ ((unsigned)&SSP2MSK*8)+4;
//extern volatile bit MSK5               @ ((unsigned)&SSP2MSK*8)+5;
//extern volatile bit MSK6               @ ((unsigned)&SSP2MSK*8)+6;
//extern volatile bit MSK7               @ ((unsigned)&SSP2MSK*8)+7;
extern union {
    struct {
        volatile unsigned MSK0                : 1;
        volatile unsigned MSK1                : 1;
        volatile unsigned MSK2                : 1;
        volatile unsigned MSK3                : 1;
        volatile unsigned MSK4                : 1;
        volatile unsigned MSK5                : 1;
        volatile unsigned MSK6                : 1;
        volatile unsigned MSK7                : 1;
    };
} SSP2MSKbits @ 0xF6A;

// Register: SSP2CON2
extern volatile unsigned char           SSP2CON2            @ 0xF6B;
// bit and bitfield definitions
//extern volatile bit SEN                @ ((unsigned)&SSP2CON2*8)+0;
//extern volatile bit RSEN               @ ((unsigned)&SSP2CON2*8)+1;
//extern volatile bit PEN                @ ((unsigned)&SSP2CON2*8)+2;
//extern volatile bit RCEN               @ ((unsigned)&SSP2CON2*8)+3;
//extern volatile bit ACKEN              @ ((unsigned)&SSP2CON2*8)+4;
//extern volatile bit ACKDT              @ ((unsigned)&SSP2CON2*8)+5;
//extern volatile bit ACKSTAT            @ ((unsigned)&SSP2CON2*8)+6;
//extern volatile bit GCEN               @ ((unsigned)&SSP2CON2*8)+7;
extern union {
    struct {
        volatile unsigned SEN                 : 1;
        volatile unsigned RSEN                : 1;
        volatile unsigned PEN                 : 1;
        volatile unsigned RCEN                : 1;
        volatile unsigned ACKEN               : 1;
        volatile unsigned ACKDT               : 1;
        volatile unsigned ACKSTAT             : 1;
        volatile unsigned GCEN                : 1;
    };
} SSP2CON2bits @ 0xF6B;

// Register: SSP2CON1
extern volatile unsigned char           SSP2CON1            @ 0xF6C;
// bit and bitfield definitions
//extern volatile bit CKP                @ ((unsigned)&SSP2CON1*8)+4;
//extern volatile bit SSPEN              @ ((unsigned)&SSP2CON1*8)+5;
//extern volatile bit SSPOV              @ ((unsigned)&SSP2CON1*8)+6;
//extern volatile bit WCOL               @ ((unsigned)&SSP2CON1*8)+7;
//extern volatile bit SSPM0              @ ((unsigned)&SSP2CON1*8)+0;
//extern volatile bit SSPM1              @ ((unsigned)&SSP2CON1*8)+1;
//extern volatile bit SSPM2              @ ((unsigned)&SSP2CON1*8)+2;
//extern volatile bit SSPM3              @ ((unsigned)&SSP2CON1*8)+3;
extern union {
    struct {
        volatile unsigned SSPM                : 4;
        volatile unsigned CKP                 : 1;
        volatile unsigned SSPEN               : 1;
        volatile unsigned SSPOV               : 1;
        volatile unsigned WCOL                : 1;
    };
    struct {
        volatile unsigned SSPM0               : 1;
        volatile unsigned SSPM1               : 1;
        volatile unsigned SSPM2               : 1;
        volatile unsigned SSPM3               : 1;
    };
} SSP2CON1bits @ 0xF6C;

// Register: SSP2STAT
extern volatile unsigned char           SSP2STAT            @ 0xF6D;
// bit and bitfield definitions
//extern volatile bit BF                 @ ((unsigned)&SSP2STAT*8)+0;
//extern volatile bit UA                 @ ((unsigned)&SSP2STAT*8)+1;
//extern volatile bit R_nW               @ ((unsigned)&SSP2STAT*8)+2;
//extern volatile bit S                  @ ((unsigned)&SSP2STAT*8)+3;
//extern volatile bit P                  @ ((unsigned)&SSP2STAT*8)+4;
//extern volatile bit D_nA               @ ((unsigned)&SSP2STAT*8)+5;
//extern volatile bit CKE                @ ((unsigned)&SSP2STAT*8)+6;
//extern volatile bit SMP                @ ((unsigned)&SSP2STAT*8)+7;
//extern volatile bit R                  @ ((unsigned)&SSP2STAT*8)+2;
//extern volatile bit D                  @ ((unsigned)&SSP2STAT*8)+5;
//extern volatile bit W                  @ ((unsigned)&SSP2STAT*8)+2;
//extern volatile bit A                  @ ((unsigned)&SSP2STAT*8)+5;
//extern volatile bit nW                 @ ((unsigned)&SSP2STAT*8)+2;
//extern volatile bit nA                 @ ((unsigned)&SSP2STAT*8)+5;
//extern volatile bit R_W                @ ((unsigned)&SSP2STAT*8)+2;
//extern volatile bit D_A                @ ((unsigned)&SSP2STAT*8)+5;
//extern volatile bit nWRITE             @ ((unsigned)&SSP2STAT*8)+2;
//extern volatile bit nADDRESS           @ ((unsigned)&SSP2STAT*8)+5;
extern union {
    struct {
        volatile unsigned BF                  : 1;
        volatile unsigned UA                  : 1;
        volatile unsigned R_nW                : 1;
        volatile unsigned S                   : 1;
        volatile unsigned P                   : 1;
        volatile unsigned D_nA                : 1;
        volatile unsigned CKE                 : 1;
        volatile unsigned SMP                 : 1;
    };
    struct {
        volatile unsigned                     : 2;
        volatile unsigned R                   : 1;
        volatile unsigned : 2;
        volatile unsigned D                   : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned W                   : 1;
        volatile unsigned : 2;
        volatile unsigned A                   : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned nW                  : 1;
        volatile unsigned : 2;
        volatile unsigned nA                  : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned R_W                 : 1;
        volatile unsigned : 2;
        volatile unsigned D_A                 : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned nWRITE              : 1;
        volatile unsigned : 2;
        volatile unsigned nADDRESS            : 1;
    };
} SSP2STATbits @ 0xF6D;

// Register: SSP2ADD
extern volatile unsigned char           SSP2ADD             @ 0xF6E;
// bit and bitfield definitions

// Register: SSP2BUF
extern volatile unsigned char           SSP2BUF             @ 0xF6F;
// bit and bitfield definitions

// Register: BAUDCON2
extern volatile unsigned char           BAUDCON2            @ 0xF70;
extern volatile unsigned char           BAUD2CON            @ 0xF70;
// bit and bitfield definitions
//extern volatile bit ABDEN              @ ((unsigned)&BAUDCON2*8)+0;
//extern volatile bit WUE                @ ((unsigned)&BAUDCON2*8)+1;
//extern volatile bit BRG16              @ ((unsigned)&BAUDCON2*8)+3;
//extern volatile bit CKTXP              @ ((unsigned)&BAUDCON2*8)+4;
//extern volatile bit DTRXP              @ ((unsigned)&BAUDCON2*8)+5;
//extern volatile bit RCIDL              @ ((unsigned)&BAUDCON2*8)+6;
//extern volatile bit ABDOVF             @ ((unsigned)&BAUDCON2*8)+7;
//extern volatile bit SCKP               @ ((unsigned)&BAUDCON2*8)+4;
extern union {
    struct {
        volatile unsigned ABDEN               : 1;
        volatile unsigned WUE                 : 1;
        volatile unsigned                     : 1;
        volatile unsigned BRG16               : 1;
        volatile unsigned CKTXP               : 1;
        volatile unsigned DTRXP               : 1;
        volatile unsigned RCIDL               : 1;
        volatile unsigned ABDOVF              : 1;
    };
    struct {
        volatile unsigned : 4;
        volatile unsigned SCKP                : 1;
    };
} BAUDCON2bits @ 0xF70;

// Register: RCSTA2
extern volatile unsigned char           RCSTA2              @ 0xF71;
extern volatile unsigned char           RC2STA              @ 0xF71;
// bit and bitfield definitions
//extern volatile bit RX9D               @ ((unsigned)&RCSTA2*8)+0;
//extern volatile bit OERR               @ ((unsigned)&RCSTA2*8)+1;
//extern volatile bit FERR               @ ((unsigned)&RCSTA2*8)+2;
//extern volatile bit ADDEN              @ ((unsigned)&RCSTA2*8)+3;
//extern volatile bit CREN               @ ((unsigned)&RCSTA2*8)+4;
//extern volatile bit SREN               @ ((unsigned)&RCSTA2*8)+5;
//extern volatile bit RX9                @ ((unsigned)&RCSTA2*8)+6;
//extern volatile bit SPEN               @ ((unsigned)&RCSTA2*8)+7;
//extern volatile bit ADEN               @ ((unsigned)&RCSTA2*8)+3;
extern volatile bit RX9D2               @ ((unsigned)&RCSTA2*8)+0;
extern volatile bit OERR2               @ ((unsigned)&RCSTA2*8)+1;
extern volatile bit FERR2               @ ((unsigned)&RCSTA2*8)+2;
extern volatile bit ADDEN2              @ ((unsigned)&RCSTA2*8)+3;
extern volatile bit CREN2               @ ((unsigned)&RCSTA2*8)+4;
extern volatile bit SREN2               @ ((unsigned)&RCSTA2*8)+5;
extern volatile bit RX92                @ ((unsigned)&RCSTA2*8)+6;
extern volatile bit SPEN2               @ ((unsigned)&RCSTA2*8)+7;
extern union {
    struct {
        volatile unsigned RX9D                : 1;
        volatile unsigned OERR                : 1;
        volatile unsigned FERR                : 1;
        volatile unsigned ADDEN               : 1;
        volatile unsigned CREN                : 1;
        volatile unsigned SREN                : 1;
        volatile unsigned RX9                 : 1;
        volatile unsigned SPEN                : 1;
    };
    struct {
        volatile unsigned                     : 3;
        volatile unsigned ADEN                : 1;
    };
    struct {
        volatile unsigned RX9D2               : 1;
        volatile unsigned OERR2               : 1;
        volatile unsigned FERR2               : 1;
        volatile unsigned ADDEN2              : 1;
        volatile unsigned CREN2               : 1;
        volatile unsigned SREN2               : 1;
        volatile unsigned RX92                : 1;
        volatile unsigned SPEN2               : 1;
    };
} RCSTA2bits @ 0xF71;

// Register: TXSTA2
extern volatile unsigned char           TXSTA2              @ 0xF72;
extern volatile unsigned char           TX2STA              @ 0xF72;
// bit and bitfield definitions
//extern volatile bit TX9D               @ ((unsigned)&TXSTA2*8)+0;
//extern volatile bit TRMT               @ ((unsigned)&TXSTA2*8)+1;
//extern volatile bit BRGH               @ ((unsigned)&TXSTA2*8)+2;
//extern volatile bit SENDB              @ ((unsigned)&TXSTA2*8)+3;
//extern volatile bit SYNC               @ ((unsigned)&TXSTA2*8)+4;
//extern volatile bit TXEN               @ ((unsigned)&TXSTA2*8)+5;
//extern volatile bit TX9                @ ((unsigned)&TXSTA2*8)+6;
//extern volatile bit CSRC               @ ((unsigned)&TXSTA2*8)+7;
extern volatile bit TX9D2               @ ((unsigned)&TXSTA2*8)+0;
extern volatile bit TRMT2               @ ((unsigned)&TXSTA2*8)+1;
extern volatile bit BRGH2               @ ((unsigned)&TXSTA2*8)+2;
extern volatile bit SENDB2              @ ((unsigned)&TXSTA2*8)+3;
extern volatile bit SYNC2               @ ((unsigned)&TXSTA2*8)+4;
extern volatile bit TXEN2               @ ((unsigned)&TXSTA2*8)+5;
extern volatile bit TX92                @ ((unsigned)&TXSTA2*8)+6;
extern volatile bit CSRC2               @ ((unsigned)&TXSTA2*8)+7;
extern union {
    struct {
        volatile unsigned TX9D                : 1;
        volatile unsigned TRMT                : 1;
        volatile unsigned BRGH                : 1;
        volatile unsigned SENDB               : 1;
        volatile unsigned SYNC                : 1;
        volatile unsigned TXEN                : 1;
        volatile unsigned TX9                 : 1;
        volatile unsigned CSRC                : 1;
    };
    struct {
        volatile unsigned TX9D2               : 1;
        volatile unsigned TRMT2               : 1;
        volatile unsigned BRGH2               : 1;
        volatile unsigned SENDB2              : 1;
        volatile unsigned SYNC2               : 1;
        volatile unsigned TXEN2               : 1;
        volatile unsigned TX92                : 1;
        volatile unsigned CSRC2               : 1;
    };
} TXSTA2bits @ 0xF72;

// Register: TXREG2
extern volatile unsigned char           TXREG2              @ 0xF73;
extern volatile unsigned char           TX2REG              @ 0xF73;
// bit and bitfield definitions

// Register: RCREG2
extern volatile unsigned char           RCREG2              @ 0xF74;
extern volatile unsigned char           RC2REG              @ 0xF74;
// bit and bitfield definitions

// Register: SPBRG2
extern volatile unsigned char           SPBRG2              @ 0xF75;
extern volatile unsigned char           SP2BRG              @ 0xF75;
// bit and bitfield definitions

// Register: SPBRGH2
extern volatile unsigned char           SPBRGH2             @ 0xF76;
extern volatile unsigned char           SP2BRGH             @ 0xF76;
// bit and bitfield definitions

// Register: CM2CON1
extern volatile unsigned char           CM2CON1             @ 0xF77;
extern volatile unsigned char           CM12CON             @ 0xF77;
// bit and bitfield definitions
extern volatile bit C2SYNC              @ ((unsigned)&CM2CON1*8)+0;
extern volatile bit C1SYNC              @ ((unsigned)&CM2CON1*8)+1;
extern volatile bit C2HYS               @ ((unsigned)&CM2CON1*8)+2;
extern volatile bit C1HYS               @ ((unsigned)&CM2CON1*8)+3;
extern volatile bit C2RSEL              @ ((unsigned)&CM2CON1*8)+4;
extern volatile bit C1RSEL              @ ((unsigned)&CM2CON1*8)+5;
extern volatile bit MC2OUT              @ ((unsigned)&CM2CON1*8)+6;
extern volatile bit MC1OUT              @ ((unsigned)&CM2CON1*8)+7;
extern union {
    struct {
        volatile unsigned C2SYNC              : 1;
        volatile unsigned C1SYNC              : 1;
        volatile unsigned C2HYS               : 1;
        volatile unsigned C1HYS               : 1;
        volatile unsigned C2RSEL              : 1;
        volatile unsigned C1RSEL              : 1;
        volatile unsigned MC2OUT              : 1;
        volatile unsigned MC1OUT              : 1;
    };
} CM2CON1bits @ 0xF77;

// Register: CM2CON0
extern volatile unsigned char           CM2CON0             @ 0xF78;
extern volatile unsigned char           CM2CON              @ 0xF78;
// bit and bitfield definitions
extern volatile bit C2R                 @ ((unsigned)&CM2CON0*8)+2;
extern volatile bit C2SP                @ ((unsigned)&CM2CON0*8)+3;
extern volatile bit C2POL               @ ((unsigned)&CM2CON0*8)+4;
extern volatile bit C2OE                @ ((unsigned)&CM2CON0*8)+5;
//extern volatile bit C2OUT              @ ((unsigned)&CM2CON0*8)+6;
extern volatile bit C2ON                @ ((unsigned)&CM2CON0*8)+7;
extern volatile bit C2CH0               @ ((unsigned)&CM2CON0*8)+0;
extern volatile bit C2CH1               @ ((unsigned)&CM2CON0*8)+1;
extern union {
    struct {
        volatile unsigned C2CH                : 2;
        volatile unsigned C2R                 : 1;
        volatile unsigned C2SP                : 1;
        volatile unsigned C2POL               : 1;
        volatile unsigned C2OE                : 1;
        volatile unsigned C2OUT               : 1;
        volatile unsigned C2ON                : 1;
    };
    struct {
        volatile unsigned C2CH0               : 1;
        volatile unsigned C2CH1               : 1;
    };
} CM2CON0bits @ 0xF78;

// Register: CM1CON0
extern volatile unsigned char           CM1CON0             @ 0xF79;
extern volatile unsigned char           CM1CON              @ 0xF79;
// bit and bitfield definitions
extern volatile bit C1R                 @ ((unsigned)&CM1CON0*8)+2;
extern volatile bit C1SP                @ ((unsigned)&CM1CON0*8)+3;
extern volatile bit C1POL               @ ((unsigned)&CM1CON0*8)+4;
extern volatile bit C1OE                @ ((unsigned)&CM1CON0*8)+5;
//extern volatile bit C1OUT              @ ((unsigned)&CM1CON0*8)+6;
extern volatile bit C1ON                @ ((unsigned)&CM1CON0*8)+7;
extern volatile bit C1CH0               @ ((unsigned)&CM1CON0*8)+0;
extern volatile bit C1CH1               @ ((unsigned)&CM1CON0*8)+1;
extern union {
    struct {
        volatile unsigned C1CH                : 2;
        volatile unsigned C1R                 : 1;
        volatile unsigned C1SP                : 1;
        volatile unsigned C1POL               : 1;
        volatile unsigned C1OE                : 1;
        volatile unsigned C1OUT               : 1;
        volatile unsigned C1ON                : 1;
    };
    struct {
        volatile unsigned C1CH0               : 1;
        volatile unsigned C1CH1               : 1;
    };
} CM1CON0bits @ 0xF79;

// Register: PIE4
extern volatile unsigned char           PIE4                @ 0xF7A;
// bit and bitfield definitions
extern volatile bit CCP3IE              @ ((unsigned)&PIE4*8)+0;
extern volatile bit CCP4IE              @ ((unsigned)&PIE4*8)+1;
extern volatile bit CCP5IE              @ ((unsigned)&PIE4*8)+2;
extern union {
    struct {
        volatile unsigned CCP3IE              : 1;
        volatile unsigned CCP4IE              : 1;
        volatile unsigned CCP5IE              : 1;
    };
} PIE4bits @ 0xF7A;

// Register: PIR4
extern volatile unsigned char           PIR4                @ 0xF7B;
// bit and bitfield definitions
extern volatile bit CCP3IF              @ ((unsigned)&PIR4*8)+0;
extern volatile bit CCP4IF              @ ((unsigned)&PIR4*8)+1;
extern volatile bit CCP5IF              @ ((unsigned)&PIR4*8)+2;
extern union {
    struct {
        volatile unsigned CCP3IF              : 1;
        volatile unsigned CCP4IF              : 1;
        volatile unsigned CCP5IF              : 1;
    };
} PIR4bits @ 0xF7B;

// Register: IPR4
extern volatile unsigned char           IPR4                @ 0xF7C;
// bit and bitfield definitions
extern volatile bit CCP3IP              @ ((unsigned)&IPR4*8)+0;
extern volatile bit CCP4IP              @ ((unsigned)&IPR4*8)+1;
extern volatile bit CCP5IP              @ ((unsigned)&IPR4*8)+2;
extern union {
    struct {
        volatile unsigned CCP3IP              : 1;
        volatile unsigned CCP4IP              : 1;
        volatile unsigned CCP5IP              : 1;
    };
} IPR4bits @ 0xF7C;

// Register: PIE5
extern volatile unsigned char           PIE5                @ 0xF7D;
// bit and bitfield definitions
extern volatile bit TMR4IE              @ ((unsigned)&PIE5*8)+0;
extern volatile bit TMR5IE              @ ((unsigned)&PIE5*8)+1;
extern volatile bit TMR6IE              @ ((unsigned)&PIE5*8)+2;
extern union {
    struct {
        volatile unsigned TMR4IE              : 1;
        volatile unsigned TMR5IE              : 1;
        volatile unsigned TMR6IE              : 1;
    };
} PIE5bits @ 0xF7D;

// Register: PIR5
extern volatile unsigned char           PIR5                @ 0xF7E;
// bit and bitfield definitions
extern volatile bit TMR4IF              @ ((unsigned)&PIR5*8)+0;
extern volatile bit TMR5IF              @ ((unsigned)&PIR5*8)+1;
extern volatile bit TMR6IF              @ ((unsigned)&PIR5*8)+2;
extern union {
    struct {
        volatile unsigned TMR4IF              : 1;
        volatile unsigned TMR5IF              : 1;
        volatile unsigned TMR6IF              : 1;
        volatile unsigned                     : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
    };
} PIR5bits @ 0xF7E;

// Register: IPR5
extern volatile unsigned char           IPR5                @ 0xF7F;
// bit and bitfield definitions
extern volatile bit TMR4IP              @ ((unsigned)&IPR5*8)+0;
extern volatile bit TMR5IP              @ ((unsigned)&IPR5*8)+1;
extern volatile bit TMR6IP              @ ((unsigned)&IPR5*8)+2;
extern union {
    struct {
        volatile unsigned TMR4IP              : 1;
        volatile unsigned TMR5IP              : 1;
        volatile unsigned TMR6IP              : 1;
        volatile unsigned                     : 1;
        volatile unsigned : 1;
    };
} IPR5bits @ 0xF7F;

// Register: PORTA
extern volatile unsigned char           PORTA               @ 0xF80;
// bit and bitfield definitions
//extern volatile bit RA0                @ ((unsigned)&PORTA*8)+0;
//extern volatile bit RA1                @ ((unsigned)&PORTA*8)+1;
//extern volatile bit RA2                @ ((unsigned)&PORTA*8)+2;
//extern volatile bit RA3                @ ((unsigned)&PORTA*8)+3;
//extern volatile bit RA4                @ ((unsigned)&PORTA*8)+4;
//extern volatile bit RA5                @ ((unsigned)&PORTA*8)+5;
//extern volatile bit RA6                @ ((unsigned)&PORTA*8)+6;
//extern volatile bit RA7                @ ((unsigned)&PORTA*8)+7;
extern volatile bit AN0                 @ ((unsigned)&PORTA*8)+0;
extern volatile bit AN1                 @ ((unsigned)&PORTA*8)+1;
extern volatile bit AN2                 @ ((unsigned)&PORTA*8)+2;
extern volatile bit AN3                 @ ((unsigned)&PORTA*8)+3;
extern volatile bit AN4                 @ ((unsigned)&PORTA*8)+5;
extern volatile bit C12IN0M             @ ((unsigned)&PORTA*8)+0;
extern volatile bit C12IN1M             @ ((unsigned)&PORTA*8)+1;
extern volatile bit C2INP               @ ((unsigned)&PORTA*8)+2;
extern volatile bit C1INP               @ ((unsigned)&PORTA*8)+3;
//extern volatile bit C1OUT              @ ((unsigned)&PORTA*8)+4;
//extern volatile bit C2OUT              @ ((unsigned)&PORTA*8)+5;
extern volatile bit C12IN0N             @ ((unsigned)&PORTA*8)+0;
extern volatile bit C12IN1N             @ ((unsigned)&PORTA*8)+1;
extern volatile bit VREFM               @ ((unsigned)&PORTA*8)+2;
extern volatile bit VREFP               @ ((unsigned)&PORTA*8)+3;
extern volatile bit T0CKI               @ ((unsigned)&PORTA*8)+4;
extern volatile bit SS                  @ ((unsigned)&PORTA*8)+5;
extern volatile bit VREFN               @ ((unsigned)&PORTA*8)+2;
extern volatile bit SRQ                 @ ((unsigned)&PORTA*8)+4;
extern volatile bit nSS                 @ ((unsigned)&PORTA*8)+5;
extern volatile bit CVREF               @ ((unsigned)&PORTA*8)+2;
extern volatile bit CCP5                @ ((unsigned)&PORTA*8)+4;
extern volatile bit LVDIN               @ ((unsigned)&PORTA*8)+5;
extern volatile bit DACOUT              @ ((unsigned)&PORTA*8)+2;
extern volatile bit HLVDIN              @ ((unsigned)&PORTA*8)+5;
extern volatile bit SS1                 @ ((unsigned)&PORTA*8)+5;
extern volatile bit nSS1                @ ((unsigned)&PORTA*8)+5;
extern volatile bit SRNQ                @ ((unsigned)&PORTA*8)+5;
extern union {
    struct {
        volatile unsigned RA0                 : 1;
        volatile unsigned RA1                 : 1;
        volatile unsigned RA2                 : 1;
        volatile unsigned RA3                 : 1;
        volatile unsigned RA4                 : 1;
        volatile unsigned RA5                 : 1;
        volatile unsigned RA6                 : 1;
        volatile unsigned RA7                 : 1;
    };
    struct {
        volatile unsigned AN0                 : 1;
        volatile unsigned AN1                 : 1;
        volatile unsigned AN2                 : 1;
        volatile unsigned AN3                 : 1;
        volatile unsigned                     : 1;
        volatile unsigned AN4                 : 1;
    };
    struct {
        volatile unsigned C12IN0M             : 1;
        volatile unsigned C12IN1M             : 1;
        volatile unsigned C2INP               : 1;
        volatile unsigned C1INP               : 1;
        volatile unsigned C1OUT               : 1;
        volatile unsigned C2OUT               : 1;
    };
    struct {
        volatile unsigned C12IN0N             : 1;
        volatile unsigned C12IN1N             : 1;
        volatile unsigned VREFM               : 1;
        volatile unsigned VREFP               : 1;
        volatile unsigned T0CKI               : 1;
        volatile unsigned SS                  : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned VREFN               : 1;
        volatile unsigned : 1;
        volatile unsigned SRQ                 : 1;
        volatile unsigned nSS                 : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned CVREF               : 1;
        volatile unsigned : 1;
        volatile unsigned CCP5                : 1;
        volatile unsigned LVDIN               : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned DACOUT              : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned HLVDIN              : 1;
    };
    struct {
        volatile unsigned : 5;
        volatile unsigned SS1                 : 1;
    };
    struct {
        volatile unsigned : 5;
        volatile unsigned nSS1                : 1;
    };
    struct {
        volatile unsigned : 5;
        volatile unsigned SRNQ                : 1;
    };
} PORTAbits @ 0xF80;

// Register: PORTB
extern volatile unsigned char           PORTB               @ 0xF81;
// bit and bitfield definitions
//extern volatile bit RB0                @ ((unsigned)&PORTB*8)+0;
//extern volatile bit RB1                @ ((unsigned)&PORTB*8)+1;
//extern volatile bit RB2                @ ((unsigned)&PORTB*8)+2;
//extern volatile bit RB3                @ ((unsigned)&PORTB*8)+3;
//extern volatile bit RB4                @ ((unsigned)&PORTB*8)+4;
//extern volatile bit RB5                @ ((unsigned)&PORTB*8)+5;
//extern volatile bit RB6                @ ((unsigned)&PORTB*8)+6;
//extern volatile bit RB7                @ ((unsigned)&PORTB*8)+7;
extern volatile bit INT0                @ ((unsigned)&PORTB*8)+0;
extern volatile bit INT1                @ ((unsigned)&PORTB*8)+1;
extern volatile bit INT2                @ ((unsigned)&PORTB*8)+2;
//extern volatile bit CCP2               @ ((unsigned)&PORTB*8)+3;
extern volatile bit KBI0                @ ((unsigned)&PORTB*8)+4;
extern volatile bit KBI1                @ ((unsigned)&PORTB*8)+5;
extern volatile bit KBI2                @ ((unsigned)&PORTB*8)+6;
extern volatile bit KBI3                @ ((unsigned)&PORTB*8)+7;
extern volatile bit AN12                @ ((unsigned)&PORTB*8)+0;
extern volatile bit AN10                @ ((unsigned)&PORTB*8)+1;
extern volatile bit AN8                 @ ((unsigned)&PORTB*8)+2;
extern volatile bit AN9                 @ ((unsigned)&PORTB*8)+3;
extern volatile bit AN11                @ ((unsigned)&PORTB*8)+4;
extern volatile bit AN13                @ ((unsigned)&PORTB*8)+5;
extern volatile bit TX2                 @ ((unsigned)&PORTB*8)+6;
extern volatile bit RX2                 @ ((unsigned)&PORTB*8)+7;
extern volatile bit FLT0                @ ((unsigned)&PORTB*8)+0;
extern volatile bit C12IN3M             @ ((unsigned)&PORTB*8)+1;
extern volatile bit P1B                 @ ((unsigned)&PORTB*8)+2;
extern volatile bit C12IN2M             @ ((unsigned)&PORTB*8)+3;
extern volatile bit T5G                 @ ((unsigned)&PORTB*8)+4;
extern volatile bit T1G                 @ ((unsigned)&PORTB*8)+5;
extern volatile bit CK2                 @ ((unsigned)&PORTB*8)+6;
extern volatile bit DT2                 @ ((unsigned)&PORTB*8)+7;
extern volatile bit SRI                 @ ((unsigned)&PORTB*8)+0;
extern volatile bit C12IN3N             @ ((unsigned)&PORTB*8)+1;
extern volatile bit CTED1               @ ((unsigned)&PORTB*8)+2;
extern volatile bit C12IN2N             @ ((unsigned)&PORTB*8)+3;
extern volatile bit P1D                 @ ((unsigned)&PORTB*8)+4;
//extern volatile bit CCP3               @ ((unsigned)&PORTB*8)+5;
extern volatile bit PGC                 @ ((unsigned)&PORTB*8)+6;
extern volatile bit PGD                 @ ((unsigned)&PORTB*8)+7;
extern volatile bit CCP4                @ ((unsigned)&PORTB*8)+0;
extern volatile bit P1C                 @ ((unsigned)&PORTB*8)+1;
extern volatile bit SDA2                @ ((unsigned)&PORTB*8)+2;
extern volatile bit CTED2               @ ((unsigned)&PORTB*8)+3;
//extern volatile bit T3CKI              @ ((unsigned)&PORTB*8)+5;
extern volatile bit SS2                 @ ((unsigned)&PORTB*8)+0;
extern volatile bit SCL2                @ ((unsigned)&PORTB*8)+1;
extern volatile bit SDI2                @ ((unsigned)&PORTB*8)+2;
//extern volatile bit P2A                @ ((unsigned)&PORTB*8)+3;
//extern volatile bit P3A                @ ((unsigned)&PORTB*8)+5;
extern volatile bit nSS2                @ ((unsigned)&PORTB*8)+0;
extern volatile bit SCK2                @ ((unsigned)&PORTB*8)+1;
extern volatile bit SDO2                @ ((unsigned)&PORTB*8)+3;
//extern volatile bit P2B                @ ((unsigned)&PORTB*8)+5;
extern union {
    struct {
        volatile unsigned RB0                 : 1;
        volatile unsigned RB1                 : 1;
        volatile unsigned RB2                 : 1;
        volatile unsigned RB3                 : 1;
        volatile unsigned RB4                 : 1;
        volatile unsigned RB5                 : 1;
        volatile unsigned RB6                 : 1;
        volatile unsigned RB7                 : 1;
    };
    struct {
        volatile unsigned INT0                : 1;
        volatile unsigned INT1                : 1;
        volatile unsigned INT2                : 1;
        volatile unsigned CCP2                : 1;
        volatile unsigned KBI0                : 1;
        volatile unsigned KBI1                : 1;
        volatile unsigned KBI2                : 1;
        volatile unsigned KBI3                : 1;
    };
    struct {
        volatile unsigned AN12                : 1;
        volatile unsigned AN10                : 1;
        volatile unsigned AN8                 : 1;
        volatile unsigned AN9                 : 1;
        volatile unsigned AN11                : 1;
        volatile unsigned AN13                : 1;
        volatile unsigned TX2                 : 1;
        volatile unsigned RX2                 : 1;
    };
    struct {
        volatile unsigned FLT0                : 1;
        volatile unsigned C12IN3M             : 1;
        volatile unsigned P1B                 : 1;
        volatile unsigned C12IN2M             : 1;
        volatile unsigned T5G                 : 1;
        volatile unsigned T1G                 : 1;
        volatile unsigned CK2                 : 1;
        volatile unsigned DT2                 : 1;
    };
    struct {
        volatile unsigned SRI                 : 1;
        volatile unsigned C12IN3N             : 1;
        volatile unsigned CTED1               : 1;
        volatile unsigned C12IN2N             : 1;
        volatile unsigned P1D                 : 1;
        volatile unsigned CCP3                : 1;
        volatile unsigned PGC                 : 1;
        volatile unsigned PGD                 : 1;
    };
    struct {
        volatile unsigned CCP4                : 1;
        volatile unsigned P1C                 : 1;
        volatile unsigned SDA2                : 1;
        volatile unsigned CTED2               : 1;
        volatile unsigned                     : 1;
        volatile unsigned T3CKI               : 1;
    };
    struct {
        volatile unsigned SS2                 : 1;
        volatile unsigned SCL2                : 1;
        volatile unsigned SDI2                : 1;
        volatile unsigned P2A                 : 1;
        volatile unsigned : 1;
        volatile unsigned P3A                 : 1;
    };
    struct {
        volatile unsigned nSS2                : 1;
        volatile unsigned SCK2                : 1;
        volatile unsigned : 1;
        volatile unsigned SDO2                : 1;
        volatile unsigned : 1;
        volatile unsigned P2B                 : 1;
    };
} PORTBbits @ 0xF81;

// Register: PORTC
extern volatile unsigned char           PORTC               @ 0xF82;
// bit and bitfield definitions
//extern volatile bit RC0                @ ((unsigned)&PORTC*8)+0;
//extern volatile bit RC1                @ ((unsigned)&PORTC*8)+1;
//extern volatile bit RC2                @ ((unsigned)&PORTC*8)+2;
//extern volatile bit RC3                @ ((unsigned)&PORTC*8)+3;
//extern volatile bit RC4                @ ((unsigned)&PORTC*8)+4;
//extern volatile bit RC5                @ ((unsigned)&PORTC*8)+5;
//extern volatile bit RC6                @ ((unsigned)&PORTC*8)+6;
//extern volatile bit RC7                @ ((unsigned)&PORTC*8)+7;
extern volatile bit T1OSO               @ ((unsigned)&PORTC*8)+0;
extern volatile bit T1OSI               @ ((unsigned)&PORTC*8)+1;
extern volatile bit T5CKI               @ ((unsigned)&PORTC*8)+2;
extern volatile bit SCK                 @ ((unsigned)&PORTC*8)+3;
extern volatile bit SDI                 @ ((unsigned)&PORTC*8)+4;
extern volatile bit SDO                 @ ((unsigned)&PORTC*8)+5;
extern volatile bit TX                  @ ((unsigned)&PORTC*8)+6;
extern volatile bit RX                  @ ((unsigned)&PORTC*8)+7;
//extern volatile bit P2B                @ ((unsigned)&PORTC*8)+0;
//extern volatile bit P2A                @ ((unsigned)&PORTC*8)+1;
extern volatile bit P1A                 @ ((unsigned)&PORTC*8)+2;
extern volatile bit SCL                 @ ((unsigned)&PORTC*8)+3;
extern volatile bit SDA                 @ ((unsigned)&PORTC*8)+4;
extern volatile bit CK                  @ ((unsigned)&PORTC*8)+6;
extern volatile bit DT                  @ ((unsigned)&PORTC*8)+7;
extern volatile bit T1CKI               @ ((unsigned)&PORTC*8)+0;
//extern volatile bit CCP2               @ ((unsigned)&PORTC*8)+1;
extern volatile bit CCP1                @ ((unsigned)&PORTC*8)+2;
extern volatile bit SCK1                @ ((unsigned)&PORTC*8)+3;
extern volatile bit SDI1                @ ((unsigned)&PORTC*8)+4;
extern volatile bit SDO1                @ ((unsigned)&PORTC*8)+5;
extern volatile bit TX1                 @ ((unsigned)&PORTC*8)+6;
extern volatile bit RX1                 @ ((unsigned)&PORTC*8)+7;
//extern volatile bit T3CKI              @ ((unsigned)&PORTC*8)+0;
extern volatile bit CTPLS               @ ((unsigned)&PORTC*8)+2;
extern volatile bit SCL1                @ ((unsigned)&PORTC*8)+3;
extern volatile bit SDA1                @ ((unsigned)&PORTC*8)+4;
extern volatile bit CK1                 @ ((unsigned)&PORTC*8)+6;
extern volatile bit DT1                 @ ((unsigned)&PORTC*8)+7;
extern volatile bit T3G                 @ ((unsigned)&PORTC*8)+0;
extern volatile bit AN14                @ ((unsigned)&PORTC*8)+2;
extern volatile bit AN15                @ ((unsigned)&PORTC*8)+3;
extern volatile bit AN16                @ ((unsigned)&PORTC*8)+4;
extern volatile bit AN17                @ ((unsigned)&PORTC*8)+5;
extern volatile bit AN18                @ ((unsigned)&PORTC*8)+6;
extern volatile bit AN19                @ ((unsigned)&PORTC*8)+7;
//extern volatile bit CCP3               @ ((unsigned)&PORTC*8)+6;
extern volatile bit P3B                 @ ((unsigned)&PORTC*8)+7;
//extern volatile bit P3A                @ ((unsigned)&PORTC*8)+6;
extern union {
    struct {
        volatile unsigned RC0                 : 1;
        volatile unsigned RC1                 : 1;
        volatile unsigned RC2                 : 1;
        volatile unsigned RC3                 : 1;
        volatile unsigned RC4                 : 1;
        volatile unsigned RC5                 : 1;
        volatile unsigned RC6                 : 1;
        volatile unsigned RC7                 : 1;
    };
    struct {
        volatile unsigned T1OSO               : 1;
        volatile unsigned T1OSI               : 1;
        volatile unsigned T5CKI               : 1;
        volatile unsigned SCK                 : 1;
        volatile unsigned SDI                 : 1;
        volatile unsigned SDO                 : 1;
        volatile unsigned TX                  : 1;
        volatile unsigned RX                  : 1;
    };
    struct {
        volatile unsigned P2B                 : 1;
        volatile unsigned P2A                 : 1;
        volatile unsigned P1A                 : 1;
        volatile unsigned SCL                 : 1;
        volatile unsigned SDA                 : 1;
        volatile unsigned                     : 1;
        volatile unsigned CK                  : 1;
        volatile unsigned DT                  : 1;
    };
    struct {
        volatile unsigned T1CKI               : 1;
        volatile unsigned CCP2                : 1;
        volatile unsigned CCP1                : 1;
        volatile unsigned SCK1                : 1;
        volatile unsigned SDI1                : 1;
        volatile unsigned SDO1                : 1;
        volatile unsigned TX1                 : 1;
        volatile unsigned RX1                 : 1;
    };
    struct {
        volatile unsigned T3CKI               : 1;
        volatile unsigned : 1;
        volatile unsigned CTPLS               : 1;
        volatile unsigned SCL1                : 1;
        volatile unsigned SDA1                : 1;
        volatile unsigned : 1;
        volatile unsigned CK1                 : 1;
        volatile unsigned DT1                 : 1;
    };
    struct {
        volatile unsigned T3G                 : 1;
        volatile unsigned : 1;
        volatile unsigned AN14                : 1;
        volatile unsigned AN15                : 1;
        volatile unsigned AN16                : 1;
        volatile unsigned AN17                : 1;
        volatile unsigned AN18                : 1;
        volatile unsigned AN19                : 1;
    };
    struct {
        volatile unsigned : 6;
        volatile unsigned CCP3                : 1;
        volatile unsigned P3B                 : 1;
    };
    struct {
        volatile unsigned : 6;
        volatile unsigned P3A                 : 1;
    };
} PORTCbits @ 0xF82;

// Register: PORTE
extern volatile unsigned char           PORTE               @ 0xF84;
// bit and bitfield definitions
extern volatile bit RE3                 @ ((unsigned)&PORTE*8)+3;
extern volatile bit MCLR                @ ((unsigned)&PORTE*8)+3;
extern volatile bit nMCLR               @ ((unsigned)&PORTE*8)+3;
extern volatile bit VPP                 @ ((unsigned)&PORTE*8)+3;
extern union {
    struct {
        volatile unsigned                     : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned RE3                 : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned MCLR                : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned nMCLR               : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 2;
        volatile unsigned VPP                 : 1;
    };
} PORTEbits @ 0xF84;

// Register: LATA
extern volatile unsigned char           LATA                @ 0xF89;
// bit and bitfield definitions
extern volatile bit LATA0               @ ((unsigned)&LATA*8)+0;
extern volatile bit LATA1               @ ((unsigned)&LATA*8)+1;
extern volatile bit LATA2               @ ((unsigned)&LATA*8)+2;
extern volatile bit LATA3               @ ((unsigned)&LATA*8)+3;
extern volatile bit LATA4               @ ((unsigned)&LATA*8)+4;
extern volatile bit LATA5               @ ((unsigned)&LATA*8)+5;
extern volatile bit LATA6               @ ((unsigned)&LATA*8)+6;
extern volatile bit LATA7               @ ((unsigned)&LATA*8)+7;
extern union {
    struct {
        volatile unsigned LATA0               : 1;
        volatile unsigned LATA1               : 1;
        volatile unsigned LATA2               : 1;
        volatile unsigned LATA3               : 1;
        volatile unsigned LATA4               : 1;
        volatile unsigned LATA5               : 1;
        volatile unsigned LATA6               : 1;
        volatile unsigned LATA7               : 1;
    };
} LATAbits @ 0xF89;

// Register: LATB
extern volatile unsigned char           LATB                @ 0xF8A;
// bit and bitfield definitions
extern volatile bit LATB0               @ ((unsigned)&LATB*8)+0;
extern volatile bit LATB1               @ ((unsigned)&LATB*8)+1;
extern volatile bit LATB2               @ ((unsigned)&LATB*8)+2;
extern volatile bit LATB3               @ ((unsigned)&LATB*8)+3;
extern volatile bit LATB4               @ ((unsigned)&LATB*8)+4;
extern volatile bit LATB5               @ ((unsigned)&LATB*8)+5;
extern volatile bit LATB6               @ ((unsigned)&LATB*8)+6;
extern volatile bit LATB7               @ ((unsigned)&LATB*8)+7;
extern union {
    struct {
        volatile unsigned LATB0               : 1;
        volatile unsigned LATB1               : 1;
        volatile unsigned LATB2               : 1;
        volatile unsigned LATB3               : 1;
        volatile unsigned LATB4               : 1;
        volatile unsigned LATB5               : 1;
        volatile unsigned LATB6               : 1;
        volatile unsigned LATB7               : 1;
    };
} LATBbits @ 0xF8A;

// Register: LATC
extern volatile unsigned char           LATC                @ 0xF8B;
// bit and bitfield definitions
extern volatile bit LATC0               @ ((unsigned)&LATC*8)+0;
extern volatile bit LATC1               @ ((unsigned)&LATC*8)+1;
extern volatile bit LATC2               @ ((unsigned)&LATC*8)+2;
extern volatile bit LATC3               @ ((unsigned)&LATC*8)+3;
extern volatile bit LATC4               @ ((unsigned)&LATC*8)+4;
extern volatile bit LATC5               @ ((unsigned)&LATC*8)+5;
extern volatile bit LATC6               @ ((unsigned)&LATC*8)+6;
extern volatile bit LATC7               @ ((unsigned)&LATC*8)+7;
extern union {
    struct {
        volatile unsigned LATC0               : 1;
        volatile unsigned LATC1               : 1;
        volatile unsigned LATC2               : 1;
        volatile unsigned LATC3               : 1;
        volatile unsigned LATC4               : 1;
        volatile unsigned LATC5               : 1;
        volatile unsigned LATC6               : 1;
        volatile unsigned LATC7               : 1;
    };
} LATCbits @ 0xF8B;

// Register: TRISA
extern volatile unsigned char           TRISA               @ 0xF92;
extern volatile unsigned char           DDRA                @ 0xF92;
// bit and bitfield definitions
extern volatile bit TRISA0              @ ((unsigned)&TRISA*8)+0;
extern volatile bit TRISA1              @ ((unsigned)&TRISA*8)+1;
extern volatile bit TRISA2              @ ((unsigned)&TRISA*8)+2;
extern volatile bit TRISA3              @ ((unsigned)&TRISA*8)+3;
extern volatile bit TRISA4              @ ((unsigned)&TRISA*8)+4;
extern volatile bit TRISA5              @ ((unsigned)&TRISA*8)+5;
extern volatile bit TRISA6              @ ((unsigned)&TRISA*8)+6;
extern volatile bit TRISA7              @ ((unsigned)&TRISA*8)+7;
//extern volatile bit RA0                @ ((unsigned)&TRISA*8)+0;
//extern volatile bit RA1                @ ((unsigned)&TRISA*8)+1;
//extern volatile bit RA2                @ ((unsigned)&TRISA*8)+2;
//extern volatile bit RA3                @ ((unsigned)&TRISA*8)+3;
//extern volatile bit RA4                @ ((unsigned)&TRISA*8)+4;
//extern volatile bit RA5                @ ((unsigned)&TRISA*8)+5;
//extern volatile bit RA6                @ ((unsigned)&TRISA*8)+6;
//extern volatile bit RA7                @ ((unsigned)&TRISA*8)+7;
extern union {
    struct {
        volatile unsigned TRISA0              : 1;
        volatile unsigned TRISA1              : 1;
        volatile unsigned TRISA2              : 1;
        volatile unsigned TRISA3              : 1;
        volatile unsigned TRISA4              : 1;
        volatile unsigned TRISA5              : 1;
        volatile unsigned TRISA6              : 1;
        volatile unsigned TRISA7              : 1;
    };
    struct {
        volatile unsigned RA0                 : 1;
        volatile unsigned RA1                 : 1;
        volatile unsigned RA2                 : 1;
        volatile unsigned RA3                 : 1;
        volatile unsigned RA4                 : 1;
        volatile unsigned RA5                 : 1;
        volatile unsigned RA6                 : 1;
        volatile unsigned RA7                 : 1;
    };
} TRISAbits @ 0xF92;

// Register: TRISB
extern volatile unsigned char           TRISB               @ 0xF93;
extern volatile unsigned char           DDRB                @ 0xF93;
// bit and bitfield definitions
extern volatile bit TRISB0              @ ((unsigned)&TRISB*8)+0;
extern volatile bit TRISB1              @ ((unsigned)&TRISB*8)+1;
extern volatile bit TRISB2              @ ((unsigned)&TRISB*8)+2;
extern volatile bit TRISB3              @ ((unsigned)&TRISB*8)+3;
extern volatile bit TRISB4              @ ((unsigned)&TRISB*8)+4;
extern volatile bit TRISB5              @ ((unsigned)&TRISB*8)+5;
extern volatile bit TRISB6              @ ((unsigned)&TRISB*8)+6;
extern volatile bit TRISB7              @ ((unsigned)&TRISB*8)+7;
//extern volatile bit RB0                @ ((unsigned)&TRISB*8)+0;
//extern volatile bit RB1                @ ((unsigned)&TRISB*8)+1;
//extern volatile bit RB2                @ ((unsigned)&TRISB*8)+2;
//extern volatile bit RB3                @ ((unsigned)&TRISB*8)+3;
//extern volatile bit RB4                @ ((unsigned)&TRISB*8)+4;
//extern volatile bit RB5                @ ((unsigned)&TRISB*8)+5;
//extern volatile bit RB6                @ ((unsigned)&TRISB*8)+6;
//extern volatile bit RB7                @ ((unsigned)&TRISB*8)+7;
extern union {
    struct {
        volatile unsigned TRISB0              : 1;
        volatile unsigned TRISB1              : 1;
        volatile unsigned TRISB2              : 1;
        volatile unsigned TRISB3              : 1;
        volatile unsigned TRISB4              : 1;
        volatile unsigned TRISB5              : 1;
        volatile unsigned TRISB6              : 1;
        volatile unsigned TRISB7              : 1;
    };
    struct {
        volatile unsigned RB0                 : 1;
        volatile unsigned RB1                 : 1;
        volatile unsigned RB2                 : 1;
        volatile unsigned RB3                 : 1;
        volatile unsigned RB4                 : 1;
        volatile unsigned RB5                 : 1;
        volatile unsigned RB6                 : 1;
        volatile unsigned RB7                 : 1;
    };
} TRISBbits @ 0xF93;

// Register: TRISC
extern volatile unsigned char           TRISC               @ 0xF94;
extern volatile unsigned char           DDRC                @ 0xF94;
// bit and bitfield definitions
extern volatile bit TRISC0              @ ((unsigned)&TRISC*8)+0;
extern volatile bit TRISC1              @ ((unsigned)&TRISC*8)+1;
extern volatile bit TRISC2              @ ((unsigned)&TRISC*8)+2;
extern volatile bit TRISC3              @ ((unsigned)&TRISC*8)+3;
extern volatile bit TRISC4              @ ((unsigned)&TRISC*8)+4;
extern volatile bit TRISC5              @ ((unsigned)&TRISC*8)+5;
extern volatile bit TRISC6              @ ((unsigned)&TRISC*8)+6;
extern volatile bit TRISC7              @ ((unsigned)&TRISC*8)+7;
//extern volatile bit RC0                @ ((unsigned)&TRISC*8)+0;
//extern volatile bit RC1                @ ((unsigned)&TRISC*8)+1;
//extern volatile bit RC2                @ ((unsigned)&TRISC*8)+2;
//extern volatile bit RC3                @ ((unsigned)&TRISC*8)+3;
//extern volatile bit RC4                @ ((unsigned)&TRISC*8)+4;
//extern volatile bit RC5                @ ((unsigned)&TRISC*8)+5;
//extern volatile bit RC6                @ ((unsigned)&TRISC*8)+6;
//extern volatile bit RC7                @ ((unsigned)&TRISC*8)+7;
extern union {
    struct {
        volatile unsigned TRISC0              : 1;
        volatile unsigned TRISC1              : 1;
        volatile unsigned TRISC2              : 1;
        volatile unsigned TRISC3              : 1;
        volatile unsigned TRISC4              : 1;
        volatile unsigned TRISC5              : 1;
        volatile unsigned TRISC6              : 1;
        volatile unsigned TRISC7              : 1;
    };
    struct {
        volatile unsigned RC0                 : 1;
        volatile unsigned RC1                 : 1;
        volatile unsigned RC2                 : 1;
        volatile unsigned RC3                 : 1;
        volatile unsigned RC4                 : 1;
        volatile unsigned RC5                 : 1;
        volatile unsigned RC6                 : 1;
        volatile unsigned RC7                 : 1;
    };
} TRISCbits @ 0xF94;

// Register: TRISE
extern volatile unsigned char           TRISE               @ 0xF96;
// bit and bitfield definitions
extern volatile bit WPUE3               @ ((unsigned)&TRISE*8)+7;
extern union {
    struct {
        volatile unsigned                     : 7;
        volatile unsigned WPUE3               : 1;
    };
} TRISEbits @ 0xF96;

// Register: OSCTUNE
extern volatile unsigned char           OSCTUNE             @ 0xF9B;
// bit and bitfield definitions
extern volatile bit PLLEN               @ ((unsigned)&OSCTUNE*8)+6;
extern volatile bit INTSRC              @ ((unsigned)&OSCTUNE*8)+7;
extern volatile bit TUN0                @ ((unsigned)&OSCTUNE*8)+0;
extern volatile bit TUN1                @ ((unsigned)&OSCTUNE*8)+1;
extern volatile bit TUN2                @ ((unsigned)&OSCTUNE*8)+2;
extern volatile bit TUN3                @ ((unsigned)&OSCTUNE*8)+3;
extern volatile bit TUN4                @ ((unsigned)&OSCTUNE*8)+4;
extern volatile bit TUN5                @ ((unsigned)&OSCTUNE*8)+5;
extern union {
    struct {
        volatile unsigned TUN                 : 6;
        volatile unsigned PLLEN               : 1;
        volatile unsigned INTSRC              : 1;
    };
    struct {
        volatile unsigned TUN0                : 1;
        volatile unsigned TUN1                : 1;
        volatile unsigned TUN2                : 1;
        volatile unsigned TUN3                : 1;
        volatile unsigned TUN4                : 1;
        volatile unsigned TUN5                : 1;
    };
} OSCTUNEbits @ 0xF9B;

// Register: HLVDCON
extern volatile unsigned char           HLVDCON             @ 0xF9C;
extern volatile unsigned char           LVDCON              @ 0xF9C;
// bit and bitfield definitions
extern volatile bit HLVDEN              @ ((unsigned)&HLVDCON*8)+4;
extern volatile bit IRVST               @ ((unsigned)&HLVDCON*8)+5;
extern volatile bit BGVST               @ ((unsigned)&HLVDCON*8)+6;
extern volatile bit VDIRMAG             @ ((unsigned)&HLVDCON*8)+7;
extern volatile bit HLVDL0              @ ((unsigned)&HLVDCON*8)+0;
extern volatile bit HLVDL1              @ ((unsigned)&HLVDCON*8)+1;
extern volatile bit HLVDL2              @ ((unsigned)&HLVDCON*8)+2;
extern volatile bit HLVDL3              @ ((unsigned)&HLVDCON*8)+3;
extern volatile bit LVDL0               @ ((unsigned)&HLVDCON*8)+0;
extern volatile bit LVDL1               @ ((unsigned)&HLVDCON*8)+1;
extern volatile bit LVDL2               @ ((unsigned)&HLVDCON*8)+2;
extern volatile bit LVDL3               @ ((unsigned)&HLVDCON*8)+3;
extern volatile bit LVDEN               @ ((unsigned)&HLVDCON*8)+4;
extern volatile bit IVRST               @ ((unsigned)&HLVDCON*8)+5;
extern volatile bit LVV0                @ ((unsigned)&HLVDCON*8)+0;
extern volatile bit LVV1                @ ((unsigned)&HLVDCON*8)+1;
extern volatile bit LVV2                @ ((unsigned)&HLVDCON*8)+2;
extern volatile bit LVV3                @ ((unsigned)&HLVDCON*8)+3;
extern volatile bit BGST                @ ((unsigned)&HLVDCON*8)+5;
extern union {
    struct {
        volatile unsigned HLVDL               : 4;
        volatile unsigned HLVDEN              : 1;
        volatile unsigned IRVST               : 1;
        volatile unsigned BGVST               : 1;
        volatile unsigned VDIRMAG             : 1;
    };
    struct {
        volatile unsigned HLVDL0              : 1;
        volatile unsigned HLVDL1              : 1;
        volatile unsigned HLVDL2              : 1;
        volatile unsigned HLVDL3              : 1;
    };
    struct {
        volatile unsigned LVDL0               : 1;
        volatile unsigned LVDL1               : 1;
        volatile unsigned LVDL2               : 1;
        volatile unsigned LVDL3               : 1;
        volatile unsigned LVDEN               : 1;
        volatile unsigned IVRST               : 1;
    };
    struct {
        volatile unsigned LVV0                : 1;
        volatile unsigned LVV1                : 1;
        volatile unsigned LVV2                : 1;
        volatile unsigned LVV3                : 1;
        volatile unsigned                     : 1;
        volatile unsigned BGST                : 1;
    };
} HLVDCONbits @ 0xF9C;

// Register: PIE1
extern volatile unsigned char           PIE1                @ 0xF9D;
// bit and bitfield definitions
extern volatile bit TMR1IE              @ ((unsigned)&PIE1*8)+0;
extern volatile bit TMR2IE              @ ((unsigned)&PIE1*8)+1;
extern volatile bit CCP1IE              @ ((unsigned)&PIE1*8)+2;
extern volatile bit SSP1IE              @ ((unsigned)&PIE1*8)+3;
extern volatile bit TX1IE               @ ((unsigned)&PIE1*8)+4;
extern volatile bit RC1IE               @ ((unsigned)&PIE1*8)+5;
extern volatile bit ADIE                @ ((unsigned)&PIE1*8)+6;
extern volatile bit SSPIE               @ ((unsigned)&PIE1*8)+3;
extern volatile bit TXIE                @ ((unsigned)&PIE1*8)+4;
extern volatile bit RCIE                @ ((unsigned)&PIE1*8)+5;
extern union {
    struct {
        volatile unsigned TMR1IE              : 1;
        volatile unsigned TMR2IE              : 1;
        volatile unsigned CCP1IE              : 1;
        volatile unsigned SSP1IE              : 1;
        volatile unsigned TX1IE               : 1;
        volatile unsigned RC1IE               : 1;
        volatile unsigned ADIE                : 1;
        volatile unsigned                     : 1;
    };
    struct {
        volatile unsigned : 3;
        volatile unsigned SSPIE               : 1;
        volatile unsigned TXIE                : 1;
        volatile unsigned RCIE                : 1;
    };
} PIE1bits @ 0xF9D;

// Register: PIR1
extern volatile unsigned char           PIR1                @ 0xF9E;
// bit and bitfield definitions
extern volatile bit TMR1IF              @ ((unsigned)&PIR1*8)+0;
extern volatile bit TMR2IF              @ ((unsigned)&PIR1*8)+1;
extern volatile bit CCP1IF              @ ((unsigned)&PIR1*8)+2;
extern volatile bit SSP1IF              @ ((unsigned)&PIR1*8)+3;
extern volatile bit TX1IF               @ ((unsigned)&PIR1*8)+4;
extern volatile bit RC1IF               @ ((unsigned)&PIR1*8)+5;
extern volatile bit ADIF                @ ((unsigned)&PIR1*8)+6;
extern volatile bit SSPIF               @ ((unsigned)&PIR1*8)+3;
extern volatile bit TXIF                @ ((unsigned)&PIR1*8)+4;
extern volatile bit RCIF                @ ((unsigned)&PIR1*8)+5;
extern union {
    struct {
        volatile unsigned TMR1IF              : 1;
        volatile unsigned TMR2IF              : 1;
        volatile unsigned CCP1IF              : 1;
        volatile unsigned SSP1IF              : 1;
        volatile unsigned TX1IF               : 1;
        volatile unsigned RC1IF               : 1;
        volatile unsigned ADIF                : 1;
        volatile unsigned                     : 1;
    };
    struct {
        volatile unsigned : 3;
        volatile unsigned SSPIF               : 1;
        volatile unsigned TXIF                : 1;
        volatile unsigned RCIF                : 1;
    };
} PIR1bits @ 0xF9E;

// Register: IPR1
extern volatile unsigned char           IPR1                @ 0xF9F;
// bit and bitfield definitions
extern volatile bit TMR1IP              @ ((unsigned)&IPR1*8)+0;
extern volatile bit TMR2IP              @ ((unsigned)&IPR1*8)+1;
extern volatile bit CCP1IP              @ ((unsigned)&IPR1*8)+2;
extern volatile bit SSP1IP              @ ((unsigned)&IPR1*8)+3;
extern volatile bit TX1IP               @ ((unsigned)&IPR1*8)+4;
extern volatile bit RC1IP               @ ((unsigned)&IPR1*8)+5;
extern volatile bit ADIP                @ ((unsigned)&IPR1*8)+6;
extern volatile bit SSPIP               @ ((unsigned)&IPR1*8)+3;
extern volatile bit TXIP                @ ((unsigned)&IPR1*8)+4;
extern volatile bit RCIP                @ ((unsigned)&IPR1*8)+5;
extern union {
    struct {
        volatile unsigned TMR1IP              : 1;
        volatile unsigned TMR2IP              : 1;
        volatile unsigned CCP1IP              : 1;
        volatile unsigned SSP1IP              : 1;
        volatile unsigned TX1IP               : 1;
        volatile unsigned RC1IP               : 1;
        volatile unsigned ADIP                : 1;
        volatile unsigned                     : 1;
    };
    struct {
        volatile unsigned : 3;
        volatile unsigned SSPIP               : 1;
        volatile unsigned TXIP                : 1;
        volatile unsigned RCIP                : 1;
    };
} IPR1bits @ 0xF9F;

// Register: PIE2
extern volatile unsigned char           PIE2                @ 0xFA0;
// bit and bitfield definitions
extern volatile bit CCP2IE              @ ((unsigned)&PIE2*8)+0;
extern volatile bit TMR3IE              @ ((unsigned)&PIE2*8)+1;
extern volatile bit HLVDIE              @ ((unsigned)&PIE2*8)+2;
extern volatile bit BCL1IE              @ ((unsigned)&PIE2*8)+3;
extern volatile bit EEIE                @ ((unsigned)&PIE2*8)+4;
extern volatile bit C2IE                @ ((unsigned)&PIE2*8)+5;
extern volatile bit C1IE                @ ((unsigned)&PIE2*8)+6;
extern volatile bit OSCFIE              @ ((unsigned)&PIE2*8)+7;
extern volatile bit LVDIE               @ ((unsigned)&PIE2*8)+2;
extern volatile bit BCLIE               @ ((unsigned)&PIE2*8)+3;
extern union {
    struct {
        volatile unsigned CCP2IE              : 1;
        volatile unsigned TMR3IE              : 1;
        volatile unsigned HLVDIE              : 1;
        volatile unsigned BCL1IE              : 1;
        volatile unsigned EEIE                : 1;
        volatile unsigned C2IE                : 1;
        volatile unsigned C1IE                : 1;
        volatile unsigned OSCFIE              : 1;
    };
    struct {
        volatile unsigned                     : 2;
        volatile unsigned LVDIE               : 1;
        volatile unsigned BCLIE               : 1;
    };
} PIE2bits @ 0xFA0;

// Register: PIR2
extern volatile unsigned char           PIR2                @ 0xFA1;
// bit and bitfield definitions
extern volatile bit CCP2IF              @ ((unsigned)&PIR2*8)+0;
extern volatile bit TMR3IF              @ ((unsigned)&PIR2*8)+1;
extern volatile bit HLVDIF              @ ((unsigned)&PIR2*8)+2;
extern volatile bit BCL1IF              @ ((unsigned)&PIR2*8)+3;
extern volatile bit EEIF                @ ((unsigned)&PIR2*8)+4;
extern volatile bit C2IF                @ ((unsigned)&PIR2*8)+5;
extern volatile bit C1IF                @ ((unsigned)&PIR2*8)+6;
extern volatile bit OSCFIF              @ ((unsigned)&PIR2*8)+7;
extern volatile bit LVDIF               @ ((unsigned)&PIR2*8)+2;
extern volatile bit BCLIF               @ ((unsigned)&PIR2*8)+3;
extern union {
    struct {
        volatile unsigned CCP2IF              : 1;
        volatile unsigned TMR3IF              : 1;
        volatile unsigned HLVDIF              : 1;
        volatile unsigned BCL1IF              : 1;
        volatile unsigned EEIF                : 1;
        volatile unsigned C2IF                : 1;
        volatile unsigned C1IF                : 1;
        volatile unsigned OSCFIF              : 1;
    };
    struct {
        volatile unsigned                     : 2;
        volatile unsigned LVDIF               : 1;
        volatile unsigned BCLIF               : 1;
    };
} PIR2bits @ 0xFA1;

// Register: IPR2
extern volatile unsigned char           IPR2                @ 0xFA2;
// bit and bitfield definitions
extern volatile bit CCP2IP              @ ((unsigned)&IPR2*8)+0;
extern volatile bit TMR3IP              @ ((unsigned)&IPR2*8)+1;
extern volatile bit HLVDIP              @ ((unsigned)&IPR2*8)+2;
extern volatile bit BCL1IP              @ ((unsigned)&IPR2*8)+3;
extern volatile bit EEIP                @ ((unsigned)&IPR2*8)+4;
extern volatile bit C2IP                @ ((unsigned)&IPR2*8)+5;
extern volatile bit C1IP                @ ((unsigned)&IPR2*8)+6;
extern volatile bit OSCFIP              @ ((unsigned)&IPR2*8)+7;
extern volatile bit LVDIP               @ ((unsigned)&IPR2*8)+2;
extern volatile bit BCLIP               @ ((unsigned)&IPR2*8)+3;
extern union {
    struct {
        volatile unsigned CCP2IP              : 1;
        volatile unsigned TMR3IP              : 1;
        volatile unsigned HLVDIP              : 1;
        volatile unsigned BCL1IP              : 1;
        volatile unsigned EEIP                : 1;
        volatile unsigned C2IP                : 1;
        volatile unsigned C1IP                : 1;
        volatile unsigned OSCFIP              : 1;
    };
    struct {
        volatile unsigned                     : 2;
        volatile unsigned LVDIP               : 1;
        volatile unsigned BCLIP               : 1;
    };
} IPR2bits @ 0xFA2;

// Register: PIE3
extern volatile unsigned char           PIE3                @ 0xFA3;
// bit and bitfield definitions
extern volatile bit TMR1GIE             @ ((unsigned)&PIE3*8)+0;
extern volatile bit TMR3GIE             @ ((unsigned)&PIE3*8)+1;
extern volatile bit TMR5GIE             @ ((unsigned)&PIE3*8)+2;
extern volatile bit CTMUIE              @ ((unsigned)&PIE3*8)+3;
extern volatile bit TX2IE               @ ((unsigned)&PIE3*8)+4;
extern volatile bit RC2IE               @ ((unsigned)&PIE3*8)+5;
extern volatile bit BCL2IE              @ ((unsigned)&PIE3*8)+6;
extern volatile bit SSP2IE              @ ((unsigned)&PIE3*8)+7;
extern union {
    struct {
        volatile unsigned TMR1GIE             : 1;
        volatile unsigned TMR3GIE             : 1;
        volatile unsigned TMR5GIE             : 1;
        volatile unsigned CTMUIE              : 1;
        volatile unsigned TX2IE               : 1;
        volatile unsigned RC2IE               : 1;
        volatile unsigned BCL2IE              : 1;
        volatile unsigned SSP2IE              : 1;
    };
} PIE3bits @ 0xFA3;

// Register: PIR3
extern volatile unsigned char           PIR3                @ 0xFA4;
// bit and bitfield definitions
extern volatile bit TMR1GIF             @ ((unsigned)&PIR3*8)+0;
extern volatile bit TMR3GIF             @ ((unsigned)&PIR3*8)+1;
extern volatile bit TMR5GIF             @ ((unsigned)&PIR3*8)+2;
extern volatile bit CTMUIF              @ ((unsigned)&PIR3*8)+3;
extern volatile bit TX2IF               @ ((unsigned)&PIR3*8)+4;
extern volatile bit RC2IF               @ ((unsigned)&PIR3*8)+5;
extern volatile bit BCL2IF              @ ((unsigned)&PIR3*8)+6;
extern volatile bit SSP2IF              @ ((unsigned)&PIR3*8)+7;
extern union {
    struct {
        volatile unsigned TMR1GIF             : 1;
        volatile unsigned TMR3GIF             : 1;
        volatile unsigned TMR5GIF             : 1;
        volatile unsigned CTMUIF              : 1;
        volatile unsigned TX2IF               : 1;
        volatile unsigned RC2IF               : 1;
        volatile unsigned BCL2IF              : 1;
        volatile unsigned SSP2IF              : 1;
    };
} PIR3bits @ 0xFA4;

// Register: IPR3
extern volatile unsigned char           IPR3                @ 0xFA5;
// bit and bitfield definitions
extern volatile bit TMR1GIP             @ ((unsigned)&IPR3*8)+0;
extern volatile bit TMR3GIP             @ ((unsigned)&IPR3*8)+1;
extern volatile bit TMR5GIP             @ ((unsigned)&IPR3*8)+2;
extern volatile bit CTMUIP              @ ((unsigned)&IPR3*8)+3;
extern volatile bit TX2IP               @ ((unsigned)&IPR3*8)+4;
extern volatile bit RC2IP               @ ((unsigned)&IPR3*8)+5;
extern volatile bit BCL2IP              @ ((unsigned)&IPR3*8)+6;
extern volatile bit SSP2IP              @ ((unsigned)&IPR3*8)+7;
extern union {
    struct {
        volatile unsigned TMR1GIP             : 1;
        volatile unsigned TMR3GIP             : 1;
        volatile unsigned TMR5GIP             : 1;
        volatile unsigned CTMUIP              : 1;
        volatile unsigned TX2IP               : 1;
        volatile unsigned RC2IP               : 1;
        volatile unsigned BCL2IP              : 1;
        volatile unsigned SSP2IP              : 1;
    };
} IPR3bits @ 0xFA5;

// Register: EECON1
extern volatile unsigned char           EECON1              @ 0xFA6;
// bit and bitfield definitions
extern volatile bit RD                  @ ((unsigned)&EECON1*8)+0;
extern volatile bit WR                  @ ((unsigned)&EECON1*8)+1;
extern volatile bit WREN                @ ((unsigned)&EECON1*8)+2;
extern volatile bit WRERR               @ ((unsigned)&EECON1*8)+3;
extern volatile bit FREE                @ ((unsigned)&EECON1*8)+4;
extern volatile bit CFGS                @ ((unsigned)&EECON1*8)+6;
extern volatile bit EEPGD               @ ((unsigned)&EECON1*8)+7;
extern union {
    struct {
        volatile unsigned RD                  : 1;
        volatile unsigned WR                  : 1;
        volatile unsigned WREN                : 1;
        volatile unsigned WRERR               : 1;
        volatile unsigned FREE                : 1;
        volatile unsigned                     : 1;
        volatile unsigned CFGS                : 1;
        volatile unsigned EEPGD               : 1;
    };
} EECON1bits @ 0xFA6;

// Register: EECON2
extern volatile unsigned char           EECON2              @ 0xFA7;
// bit and bitfield definitions

// Register: EEDATA
extern volatile unsigned char           EEDATA              @ 0xFA8;
// bit and bitfield definitions

// Register: EEADR
extern volatile unsigned char           EEADR               @ 0xFA9;
// bit and bitfield definitions
extern volatile bit EEADR0              @ ((unsigned)&EEADR*8)+0;
extern volatile bit EEADR1              @ ((unsigned)&EEADR*8)+1;
extern volatile bit EEADR2              @ ((unsigned)&EEADR*8)+2;
extern volatile bit EEADR3              @ ((unsigned)&EEADR*8)+3;
extern volatile bit EEADR4              @ ((unsigned)&EEADR*8)+4;
extern volatile bit EEADR5              @ ((unsigned)&EEADR*8)+5;
extern volatile bit EEADR6              @ ((unsigned)&EEADR*8)+6;
extern volatile bit EEADR7              @ ((unsigned)&EEADR*8)+7;
extern union {
    struct {
        volatile unsigned EEADR0              : 1;
        volatile unsigned EEADR1              : 1;
        volatile unsigned EEADR2              : 1;
        volatile unsigned EEADR3              : 1;
        volatile unsigned EEADR4              : 1;
        volatile unsigned EEADR5              : 1;
        volatile unsigned EEADR6              : 1;
        volatile unsigned EEADR7              : 1;
    };
} EEADRbits @ 0xFA9;

// Register: RCSTA1
extern volatile unsigned char           RCSTA1              @ 0xFAB;
extern volatile unsigned char           RCSTA               @ 0xFAB;
extern volatile unsigned char           RC1STA              @ 0xFAB;
// bit and bitfield definitions
//extern volatile bit RX9D               @ ((unsigned)&RCSTA1*8)+0;
//extern volatile bit OERR               @ ((unsigned)&RCSTA1*8)+1;
//extern volatile bit FERR               @ ((unsigned)&RCSTA1*8)+2;
//extern volatile bit ADDEN              @ ((unsigned)&RCSTA1*8)+3;
//extern volatile bit CREN               @ ((unsigned)&RCSTA1*8)+4;
//extern volatile bit SREN               @ ((unsigned)&RCSTA1*8)+5;
//extern volatile bit RX9                @ ((unsigned)&RCSTA1*8)+6;
//extern volatile bit SPEN               @ ((unsigned)&RCSTA1*8)+7;
//extern volatile bit ADEN               @ ((unsigned)&RCSTA1*8)+3;
extern volatile bit RX9D1               @ ((unsigned)&RCSTA1*8)+0;
extern volatile bit OERR1               @ ((unsigned)&RCSTA1*8)+1;
extern volatile bit FERR1               @ ((unsigned)&RCSTA1*8)+2;
extern volatile bit ADDEN1              @ ((unsigned)&RCSTA1*8)+3;
extern volatile bit CREN1               @ ((unsigned)&RCSTA1*8)+4;
extern volatile bit SREN1               @ ((unsigned)&RCSTA1*8)+5;
extern volatile bit RX91                @ ((unsigned)&RCSTA1*8)+6;
extern volatile bit SPEN1               @ ((unsigned)&RCSTA1*8)+7;
extern union {
    struct {
        volatile unsigned RX9D                : 1;
        volatile unsigned OERR                : 1;
        volatile unsigned FERR                : 1;
        volatile unsigned ADDEN               : 1;
        volatile unsigned CREN                : 1;
        volatile unsigned SREN                : 1;
        volatile unsigned RX9                 : 1;
        volatile unsigned SPEN                : 1;
    };
    struct {
        volatile unsigned                     : 3;
        volatile unsigned ADEN                : 1;
    };
    struct {
        volatile unsigned RX9D1               : 1;
        volatile unsigned OERR1               : 1;
        volatile unsigned FERR1               : 1;
        volatile unsigned ADDEN1              : 1;
        volatile unsigned CREN1               : 1;
        volatile unsigned SREN1               : 1;
        volatile unsigned RX91                : 1;
        volatile unsigned SPEN1               : 1;
    };
} RCSTA1bits @ 0xFAB;

// Register: TXSTA1
extern volatile unsigned char           TXSTA1              @ 0xFAC;
extern volatile unsigned char           TXSTA               @ 0xFAC;
extern volatile unsigned char           TX1STA              @ 0xFAC;
// bit and bitfield definitions
//extern volatile bit TX9D               @ ((unsigned)&TXSTA1*8)+0;
//extern volatile bit TRMT               @ ((unsigned)&TXSTA1*8)+1;
//extern volatile bit BRGH               @ ((unsigned)&TXSTA1*8)+2;
//extern volatile bit SENDB              @ ((unsigned)&TXSTA1*8)+3;
//extern volatile bit SYNC               @ ((unsigned)&TXSTA1*8)+4;
//extern volatile bit TXEN               @ ((unsigned)&TXSTA1*8)+5;
//extern volatile bit TX9                @ ((unsigned)&TXSTA1*8)+6;
//extern volatile bit CSRC               @ ((unsigned)&TXSTA1*8)+7;
extern volatile bit TX9D1               @ ((unsigned)&TXSTA1*8)+0;
extern volatile bit TRMT1               @ ((unsigned)&TXSTA1*8)+1;
extern volatile bit BRGH1               @ ((unsigned)&TXSTA1*8)+2;
extern volatile bit SENDB1              @ ((unsigned)&TXSTA1*8)+3;
extern volatile bit SYNC1               @ ((unsigned)&TXSTA1*8)+4;
extern volatile bit TXEN1               @ ((unsigned)&TXSTA1*8)+5;
extern volatile bit TX91                @ ((unsigned)&TXSTA1*8)+6;
extern volatile bit CSRC1               @ ((unsigned)&TXSTA1*8)+7;
extern union {
    struct {
        volatile unsigned TX9D                : 1;
        volatile unsigned TRMT                : 1;
        volatile unsigned BRGH                : 1;
        volatile unsigned SENDB               : 1;
        volatile unsigned SYNC                : 1;
        volatile unsigned TXEN                : 1;
        volatile unsigned TX9                 : 1;
        volatile unsigned CSRC                : 1;
    };
    struct {
        volatile unsigned TX9D1               : 1;
        volatile unsigned TRMT1               : 1;
        volatile unsigned BRGH1               : 1;
        volatile unsigned SENDB1              : 1;
        volatile unsigned SYNC1               : 1;
        volatile unsigned TXEN1               : 1;
        volatile unsigned TX91                : 1;
        volatile unsigned CSRC1               : 1;
    };
} TXSTA1bits @ 0xFAC;

// Register: TXREG1
extern volatile unsigned char           TXREG1              @ 0xFAD;
extern volatile unsigned char           TXREG               @ 0xFAD;
extern volatile unsigned char           TX1REG              @ 0xFAD;
// bit and bitfield definitions

// Register: RCREG1
extern volatile unsigned char           RCREG1              @ 0xFAE;
extern volatile unsigned char           RCREG               @ 0xFAE;
extern volatile unsigned char           RC1REG              @ 0xFAE;
// bit and bitfield definitions

// Register: SPBRG1
extern volatile unsigned char           SPBRG1              @ 0xFAF;
extern volatile unsigned char           SPBRG               @ 0xFAF;
extern volatile unsigned char           SP1BRG              @ 0xFAF;
// bit and bitfield definitions

// Register: SPBRGH1
extern volatile unsigned char           SPBRGH1             @ 0xFB0;
extern volatile unsigned char           SPBRGH              @ 0xFB0;
extern volatile unsigned char           SP1BRGH             @ 0xFB0;
// bit and bitfield definitions

// Register: T3CON
extern volatile unsigned char           T3CON               @ 0xFB1;
// bit and bitfield definitions
extern volatile bit TMR3ON              @ ((unsigned)&T3CON*8)+0;
extern volatile bit T3RD16              @ ((unsigned)&T3CON*8)+1;
extern volatile bit nT3SYNC             @ ((unsigned)&T3CON*8)+2;
extern volatile bit T3SOSCEN            @ ((unsigned)&T3CON*8)+3;
extern volatile bit T3OSCEN             @ ((unsigned)&T3CON*8)+3;
extern volatile bit T3CKPS0             @ ((unsigned)&T3CON*8)+4;
extern volatile bit T3CKPS1             @ ((unsigned)&T3CON*8)+5;
extern volatile bit TMR3CS0             @ ((unsigned)&T3CON*8)+6;
extern volatile bit TMR3CS1             @ ((unsigned)&T3CON*8)+7;
extern union {
    struct {
        volatile unsigned TMR3ON              : 1;
        volatile unsigned T3RD16              : 1;
        volatile unsigned nT3SYNC             : 1;
        volatile unsigned T3SOSCEN            : 1;
        volatile unsigned T3CKPS              : 2;
        volatile unsigned TMR3CS              : 2;
    };
    struct {
        volatile unsigned                     : 3;
        volatile unsigned T3OSCEN             : 1;
        volatile unsigned T3CKPS0             : 1;
        volatile unsigned T3CKPS1             : 1;
        volatile unsigned TMR3CS0             : 1;
        volatile unsigned TMR3CS1             : 1;
    };
} T3CONbits @ 0xFB1;
// bit and bitfield definitions

// Register: TMR3L
extern volatile unsigned char           TMR3L               @ 0xFB2;
// bit and bitfield definitions

// Register: TMR3H
extern volatile unsigned char           TMR3H               @ 0xFB3;
// bit and bitfield definitions

// Register: TMR3
extern volatile unsigned int            TMR3                @ 0xFB2;

// Register: T3GCON
extern volatile unsigned char           T3GCON              @ 0xFB4;
// bit and bitfield definitions
extern volatile bit T3GVAL              @ ((unsigned)&T3GCON*8)+2;
extern volatile bit T3GGO_nDONE         @ ((unsigned)&T3GCON*8)+3;
extern volatile bit T3GSPM              @ ((unsigned)&T3GCON*8)+4;
extern volatile bit T3GTM               @ ((unsigned)&T3GCON*8)+5;
extern volatile bit T3GPOL              @ ((unsigned)&T3GCON*8)+6;
extern volatile bit TMR3GE              @ ((unsigned)&T3GCON*8)+7;
extern volatile bit T3GSS0              @ ((unsigned)&T3GCON*8)+0;
extern volatile bit T3GSS1              @ ((unsigned)&T3GCON*8)+1;
extern volatile bit T3G_DONE            @ ((unsigned)&T3GCON*8)+3;
extern volatile bit T3GGO               @ ((unsigned)&T3GCON*8)+3;
extern union {
    struct {
        volatile unsigned T3GSS               : 2;
        volatile unsigned T3GVAL              : 1;
        volatile unsigned T3GGO_nDONE         : 1;
        volatile unsigned T3GSPM              : 1;
        volatile unsigned T3GTM               : 1;
        volatile unsigned T3GPOL              : 1;
        volatile unsigned TMR3GE              : 1;
    };
    struct {
        volatile unsigned T3GSS0              : 1;
        volatile unsigned T3GSS1              : 1;
        volatile unsigned                     : 1;
        volatile unsigned T3G_DONE            : 1;
    };
    struct {
        volatile unsigned : 3;
        volatile unsigned T3GGO               : 1;
    };
} T3GCONbits @ 0xFB4;

// Register: ECCP1AS
extern volatile unsigned char           ECCP1AS             @ 0xFB6;
extern volatile unsigned char           ECCPAS              @ 0xFB6;
// bit and bitfield definitions
extern volatile bit CCP1ASE             @ ((unsigned)&ECCP1AS*8)+7;
extern volatile bit P1SSBD0             @ ((unsigned)&ECCP1AS*8)+0;
extern volatile bit P1SSBD1             @ ((unsigned)&ECCP1AS*8)+1;
extern volatile bit P1SSAC0             @ ((unsigned)&ECCP1AS*8)+2;
extern volatile bit P1SSAC1             @ ((unsigned)&ECCP1AS*8)+3;
extern volatile bit CCP1AS0             @ ((unsigned)&ECCP1AS*8)+4;
extern volatile bit CCP1AS1             @ ((unsigned)&ECCP1AS*8)+5;
extern volatile bit CCP1AS2             @ ((unsigned)&ECCP1AS*8)+6;
extern volatile bit PSS1BD0             @ ((unsigned)&ECCP1AS*8)+0;
extern volatile bit PSS1BD1             @ ((unsigned)&ECCP1AS*8)+1;
extern volatile bit PSS1AC0             @ ((unsigned)&ECCP1AS*8)+2;
extern volatile bit PSS1AC1             @ ((unsigned)&ECCP1AS*8)+3;
extern volatile bit ECCPASE             @ ((unsigned)&ECCP1AS*8)+7;
extern volatile bit PSSBD0              @ ((unsigned)&ECCP1AS*8)+0;
extern volatile bit PSSBD1              @ ((unsigned)&ECCP1AS*8)+1;
extern volatile bit PSSAC0              @ ((unsigned)&ECCP1AS*8)+2;
extern volatile bit PSSAC1              @ ((unsigned)&ECCP1AS*8)+3;
extern volatile bit ECCPAS0             @ ((unsigned)&ECCP1AS*8)+4;
extern volatile bit ECCPAS1             @ ((unsigned)&ECCP1AS*8)+5;
extern volatile bit ECCPAS2             @ ((unsigned)&ECCP1AS*8)+6;
extern union {
    struct {
        volatile unsigned P1SSBD              : 2;
        volatile unsigned P1SSAC              : 2;
        volatile unsigned CCP1AS              : 3;
        volatile unsigned CCP1ASE             : 1;
    };
    struct {
        volatile unsigned P1SSBD0             : 1;
        volatile unsigned P1SSBD1             : 1;
        volatile unsigned P1SSAC0             : 1;
        volatile unsigned P1SSAC1             : 1;
        volatile unsigned CCP1AS0             : 1;
        volatile unsigned CCP1AS1             : 1;
        volatile unsigned CCP1AS2             : 1;
    };
    struct {
        volatile unsigned PSS1BD              : 2;
        volatile unsigned PSS1AC              : 2;
    };
    struct {
        volatile unsigned PSS1BD0             : 1;
        volatile unsigned PSS1BD1             : 1;
        volatile unsigned PSS1AC0             : 1;
        volatile unsigned PSS1AC1             : 1;
    };
    struct {
        volatile unsigned PSSBD               : 2;
        volatile unsigned PSSAC               : 2;
        volatile unsigned ECCPAS              : 3;
        volatile unsigned ECCPASE             : 1;
    };
    struct {
        volatile unsigned PSSBD0              : 1;
        volatile unsigned PSSBD1              : 1;
        volatile unsigned PSSAC0              : 1;
        volatile unsigned PSSAC1              : 1;
        volatile unsigned ECCPAS0             : 1;
        volatile unsigned ECCPAS1             : 1;
        volatile unsigned ECCPAS2             : 1;
    };
} ECCP1ASbits @ 0xFB6;

// Register: PWM1CON
extern volatile unsigned char           PWM1CON             @ 0xFB7;
extern volatile unsigned char           PWMCON              @ 0xFB7;
// bit and bitfield definitions
extern volatile bit P1RSEN              @ ((unsigned)&PWM1CON*8)+7;
extern volatile bit P1DC0               @ ((unsigned)&PWM1CON*8)+0;
extern volatile bit P1DC1               @ ((unsigned)&PWM1CON*8)+1;
extern volatile bit P1DC2               @ ((unsigned)&PWM1CON*8)+2;
extern volatile bit P1DC3               @ ((unsigned)&PWM1CON*8)+3;
extern volatile bit P1DC4               @ ((unsigned)&PWM1CON*8)+4;
extern volatile bit P1DC5               @ ((unsigned)&PWM1CON*8)+5;
extern volatile bit P1DC6               @ ((unsigned)&PWM1CON*8)+6;
extern volatile bit PRSEN               @ ((unsigned)&PWM1CON*8)+7;
extern volatile bit PDC0                @ ((unsigned)&PWM1CON*8)+0;
extern volatile bit PDC1                @ ((unsigned)&PWM1CON*8)+1;
extern volatile bit PDC2                @ ((unsigned)&PWM1CON*8)+2;
extern volatile bit PDC3                @ ((unsigned)&PWM1CON*8)+3;
extern volatile bit PDC4                @ ((unsigned)&PWM1CON*8)+4;
extern volatile bit PDC5                @ ((unsigned)&PWM1CON*8)+5;
extern volatile bit PDC6                @ ((unsigned)&PWM1CON*8)+6;
extern union {
    struct {
        volatile unsigned P1DC                : 7;
        volatile unsigned P1RSEN              : 1;
    };
    struct {
        volatile unsigned P1DC0               : 1;
        volatile unsigned P1DC1               : 1;
        volatile unsigned P1DC2               : 1;
        volatile unsigned P1DC3               : 1;
        volatile unsigned P1DC4               : 1;
        volatile unsigned P1DC5               : 1;
        volatile unsigned P1DC6               : 1;
    };
    struct {
        volatile unsigned PDC                 : 7;
        volatile unsigned PRSEN               : 1;
    };
    struct {
        volatile unsigned PDC0                : 1;
        volatile unsigned PDC1                : 1;
        volatile unsigned PDC2                : 1;
        volatile unsigned PDC3                : 1;
        volatile unsigned PDC4                : 1;
        volatile unsigned PDC5                : 1;
        volatile unsigned PDC6                : 1;
    };
} PWM1CONbits @ 0xFB7;

// Register: BAUDCON1
extern volatile unsigned char           BAUDCON1            @ 0xFB8;
extern volatile unsigned char           BAUDCON             @ 0xFB8;
extern volatile unsigned char           BAUDCTL             @ 0xFB8;
extern volatile unsigned char           BAUD1CON            @ 0xFB8;
// bit and bitfield definitions
//extern volatile bit ABDEN              @ ((unsigned)&BAUDCON1*8)+0;
//extern volatile bit WUE                @ ((unsigned)&BAUDCON1*8)+1;
//extern volatile bit BRG16              @ ((unsigned)&BAUDCON1*8)+3;
//extern volatile bit CKTXP              @ ((unsigned)&BAUDCON1*8)+4;
//extern volatile bit DTRXP              @ ((unsigned)&BAUDCON1*8)+5;
//extern volatile bit RCIDL              @ ((unsigned)&BAUDCON1*8)+6;
//extern volatile bit ABDOVF             @ ((unsigned)&BAUDCON1*8)+7;
//extern volatile bit SCKP               @ ((unsigned)&BAUDCON1*8)+4;
extern union {
    struct {
        volatile unsigned ABDEN               : 1;
        volatile unsigned WUE                 : 1;
        volatile unsigned                     : 1;
        volatile unsigned BRG16               : 1;
        volatile unsigned CKTXP               : 1;
        volatile unsigned DTRXP               : 1;
        volatile unsigned RCIDL               : 1;
        volatile unsigned ABDOVF              : 1;
    };
    struct {
        volatile unsigned : 4;
        volatile unsigned SCKP                : 1;
    };
} BAUDCON1bits @ 0xFB8;

// Register: PSTR1CON
extern volatile unsigned char           PSTR1CON            @ 0xFB9;
extern volatile unsigned char           PSTRCON             @ 0xFB9;
// bit and bitfield definitions
extern volatile bit STR1A               @ ((unsigned)&PSTR1CON*8)+0;
extern volatile bit STR1B               @ ((unsigned)&PSTR1CON*8)+1;
extern volatile bit STR1C               @ ((unsigned)&PSTR1CON*8)+2;
extern volatile bit STR1D               @ ((unsigned)&PSTR1CON*8)+3;
extern volatile bit STR1SYNC            @ ((unsigned)&PSTR1CON*8)+4;
extern union {
    struct {
        volatile unsigned STR1A               : 1;
        volatile unsigned STR1B               : 1;
        volatile unsigned STR1C               : 1;
        volatile unsigned STR1D               : 1;
        volatile unsigned STR1SYNC            : 1;
    };
} PSTR1CONbits @ 0xFB9;

// Register: T2CON
extern volatile unsigned char           T2CON               @ 0xFBA;
// bit and bitfield definitions
extern volatile bit TMR2ON              @ ((unsigned)&T2CON*8)+2;
extern volatile bit T2CKPS0             @ ((unsigned)&T2CON*8)+0;
extern volatile bit T2CKPS1             @ ((unsigned)&T2CON*8)+1;
extern volatile bit T2OUTPS0            @ ((unsigned)&T2CON*8)+3;
extern volatile bit T2OUTPS1            @ ((unsigned)&T2CON*8)+4;
extern volatile bit T2OUTPS2            @ ((unsigned)&T2CON*8)+5;
extern volatile bit T2OUTPS3            @ ((unsigned)&T2CON*8)+6;
extern union {
    struct {
        volatile unsigned T2CKPS              : 2;
        volatile unsigned TMR2ON              : 1;
        volatile unsigned T2OUTPS             : 4;
    };
    struct {
        volatile unsigned T2CKPS0             : 1;
        volatile unsigned T2CKPS1             : 1;
        volatile unsigned                     : 1;
        volatile unsigned T2OUTPS0            : 1;
        volatile unsigned T2OUTPS1            : 1;
        volatile unsigned T2OUTPS2            : 1;
        volatile unsigned T2OUTPS3            : 1;
    };
} T2CONbits @ 0xFBA;

// Register: PR2
extern volatile unsigned char           PR2                 @ 0xFBB;
// bit and bitfield definitions

// Register: TMR2
extern volatile unsigned char           TMR2                @ 0xFBC;
// bit and bitfield definitions

// Register: CCP1CON
extern volatile unsigned char           CCP1CON             @ 0xFBD;
// bit and bitfield definitions
extern volatile bit CCP1M0              @ ((unsigned)&CCP1CON*8)+0;
extern volatile bit CCP1M1              @ ((unsigned)&CCP1CON*8)+1;
extern volatile bit CCP1M2              @ ((unsigned)&CCP1CON*8)+2;
extern volatile bit CCP1M3              @ ((unsigned)&CCP1CON*8)+3;
extern volatile bit DC1B0               @ ((unsigned)&CCP1CON*8)+4;
extern volatile bit DC1B1               @ ((unsigned)&CCP1CON*8)+5;
extern volatile bit P1M0                @ ((unsigned)&CCP1CON*8)+6;
extern volatile bit P1M1                @ ((unsigned)&CCP1CON*8)+7;
extern union {
    struct {
        volatile unsigned CCP1M               : 4;
        volatile unsigned DC1B                : 2;
        volatile unsigned P1M                 : 2;
    };
    struct {
        volatile unsigned CCP1M0              : 1;
        volatile unsigned CCP1M1              : 1;
        volatile unsigned CCP1M2              : 1;
        volatile unsigned CCP1M3              : 1;
        volatile unsigned DC1B0               : 1;
        volatile unsigned DC1B1               : 1;
        volatile unsigned P1M0                : 1;
        volatile unsigned P1M1                : 1;
    };
} CCP1CONbits @ 0xFBD;
// bit and bitfield definitions

// Register: CCPR1L
extern volatile unsigned char           CCPR1L              @ 0xFBE;
// bit and bitfield definitions

// Register: CCPR1H
extern volatile unsigned char           CCPR1H              @ 0xFBF;
// bit and bitfield definitions

// Register: CCPR1
extern volatile unsigned int            CCPR1               @ 0xFBE;

// Register: ADCON2
extern volatile unsigned char           ADCON2              @ 0xFC0;
// bit and bitfield definitions
extern volatile bit ADFM                @ ((unsigned)&ADCON2*8)+7;
extern volatile bit ADCS0               @ ((unsigned)&ADCON2*8)+0;
extern volatile bit ADCS1               @ ((unsigned)&ADCON2*8)+1;
extern volatile bit ADCS2               @ ((unsigned)&ADCON2*8)+2;
extern volatile bit ACQT0               @ ((unsigned)&ADCON2*8)+3;
extern volatile bit ACQT1               @ ((unsigned)&ADCON2*8)+4;
extern volatile bit ACQT2               @ ((unsigned)&ADCON2*8)+5;
extern union {
    struct {
        volatile unsigned ADCS                : 3;
        volatile unsigned ACQT                : 3;
        volatile unsigned                     : 1;
        volatile unsigned ADFM                : 1;
    };
    struct {
        volatile unsigned ADCS0               : 1;
        volatile unsigned ADCS1               : 1;
        volatile unsigned ADCS2               : 1;
        volatile unsigned ACQT0               : 1;
        volatile unsigned ACQT1               : 1;
        volatile unsigned ACQT2               : 1;
    };
} ADCON2bits @ 0xFC0;

// Register: ADCON1
extern volatile unsigned char           ADCON1              @ 0xFC1;
// bit and bitfield definitions
extern volatile bit TRIGSEL             @ ((unsigned)&ADCON1*8)+7;
extern volatile bit NVCFG0              @ ((unsigned)&ADCON1*8)+0;
extern volatile bit NVCFG1              @ ((unsigned)&ADCON1*8)+1;
extern volatile bit PVCFG0              @ ((unsigned)&ADCON1*8)+2;
extern volatile bit PVCFG1              @ ((unsigned)&ADCON1*8)+3;
extern union {
    struct {
        volatile unsigned NVCFG               : 2;
        volatile unsigned PVCFG               : 2;
        volatile unsigned                     : 3;
        volatile unsigned TRIGSEL             : 1;
    };
    struct {
        volatile unsigned NVCFG0              : 1;
        volatile unsigned NVCFG1              : 1;
        volatile unsigned PVCFG0              : 1;
        volatile unsigned PVCFG1              : 1;
    };
} ADCON1bits @ 0xFC1;

// Register: ADCON0
extern volatile unsigned char           ADCON0              @ 0xFC2;
// bit and bitfield definitions
extern volatile bit ADON                @ ((unsigned)&ADCON0*8)+0;
extern volatile bit GO_nDONE            @ ((unsigned)&ADCON0*8)+1;
extern volatile bit GO                  @ ((unsigned)&ADCON0*8)+1;
extern volatile bit CHS0                @ ((unsigned)&ADCON0*8)+2;
extern volatile bit CHS1                @ ((unsigned)&ADCON0*8)+3;
extern volatile bit CHS2                @ ((unsigned)&ADCON0*8)+4;
extern volatile bit CHS3                @ ((unsigned)&ADCON0*8)+5;
extern volatile bit CHS4                @ ((unsigned)&ADCON0*8)+6;
extern volatile bit DONE                @ ((unsigned)&ADCON0*8)+1;
extern volatile bit nDONE               @ ((unsigned)&ADCON0*8)+1;
extern volatile bit GO_DONE             @ ((unsigned)&ADCON0*8)+1;
extern union {
    struct {
        volatile unsigned ADON                : 1;
        volatile unsigned GO_nDONE            : 1;
        volatile unsigned CHS                 : 5;
    };
    struct {
        volatile unsigned                     : 1;
        volatile unsigned GO                  : 1;
        volatile unsigned CHS0                : 1;
        volatile unsigned CHS1                : 1;
        volatile unsigned CHS2                : 1;
        volatile unsigned CHS3                : 1;
        volatile unsigned CHS4                : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned DONE                : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned nDONE               : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned GO_DONE             : 1;
    };
} ADCON0bits @ 0xFC2;
// bit and bitfield definitions

// Register: ADRESL
extern volatile unsigned char           ADRESL              @ 0xFC3;
// bit and bitfield definitions

// Register: ADRESH
extern volatile unsigned char           ADRESH              @ 0xFC4;
// bit and bitfield definitions

// Register: ADRES
extern volatile unsigned int            ADRES               @ 0xFC3;

// Register: SSP1CON2
extern volatile unsigned char           SSP1CON2            @ 0xFC5;
extern volatile unsigned char           SSPCON2             @ 0xFC5;
// bit and bitfield definitions
//extern volatile bit SEN                @ ((unsigned)&SSP1CON2*8)+0;
//extern volatile bit RSEN               @ ((unsigned)&SSP1CON2*8)+1;
//extern volatile bit PEN                @ ((unsigned)&SSP1CON2*8)+2;
//extern volatile bit RCEN               @ ((unsigned)&SSP1CON2*8)+3;
//extern volatile bit ACKEN              @ ((unsigned)&SSP1CON2*8)+4;
//extern volatile bit ACKDT              @ ((unsigned)&SSP1CON2*8)+5;
//extern volatile bit ACKSTAT            @ ((unsigned)&SSP1CON2*8)+6;
//extern volatile bit GCEN               @ ((unsigned)&SSP1CON2*8)+7;
extern union {
    struct {
        volatile unsigned SEN                 : 1;
        volatile unsigned RSEN                : 1;
        volatile unsigned PEN                 : 1;
        volatile unsigned RCEN                : 1;
        volatile unsigned ACKEN               : 1;
        volatile unsigned ACKDT               : 1;
        volatile unsigned ACKSTAT             : 1;
        volatile unsigned GCEN                : 1;
    };
} SSP1CON2bits @ 0xFC5;

// Register: SSP1CON1
extern volatile unsigned char           SSP1CON1            @ 0xFC6;
extern volatile unsigned char           SSPCON1             @ 0xFC6;
// bit and bitfield definitions
//extern volatile bit CKP                @ ((unsigned)&SSP1CON1*8)+4;
//extern volatile bit SSPEN              @ ((unsigned)&SSP1CON1*8)+5;
//extern volatile bit SSPOV              @ ((unsigned)&SSP1CON1*8)+6;
//extern volatile bit WCOL               @ ((unsigned)&SSP1CON1*8)+7;
//extern volatile bit SSPM0              @ ((unsigned)&SSP1CON1*8)+0;
//extern volatile bit SSPM1              @ ((unsigned)&SSP1CON1*8)+1;
//extern volatile bit SSPM2              @ ((unsigned)&SSP1CON1*8)+2;
//extern volatile bit SSPM3              @ ((unsigned)&SSP1CON1*8)+3;
extern union {
    struct {
        volatile unsigned SSPM                : 4;
        volatile unsigned CKP                 : 1;
        volatile unsigned SSPEN               : 1;
        volatile unsigned SSPOV               : 1;
        volatile unsigned WCOL                : 1;
    };
    struct {
        volatile unsigned SSPM0               : 1;
        volatile unsigned SSPM1               : 1;
        volatile unsigned SSPM2               : 1;
        volatile unsigned SSPM3               : 1;
    };
} SSP1CON1bits @ 0xFC6;

// Register: SSP1STAT
extern volatile unsigned char           SSP1STAT            @ 0xFC7;
extern volatile unsigned char           SSPSTAT             @ 0xFC7;
// bit and bitfield definitions
//extern volatile bit BF                 @ ((unsigned)&SSP1STAT*8)+0;
//extern volatile bit UA                 @ ((unsigned)&SSP1STAT*8)+1;
//extern volatile bit R_nW               @ ((unsigned)&SSP1STAT*8)+2;
//extern volatile bit S                  @ ((unsigned)&SSP1STAT*8)+3;
//extern volatile bit P                  @ ((unsigned)&SSP1STAT*8)+4;
//extern volatile bit D_nA               @ ((unsigned)&SSP1STAT*8)+5;
//extern volatile bit CKE                @ ((unsigned)&SSP1STAT*8)+6;
//extern volatile bit SMP                @ ((unsigned)&SSP1STAT*8)+7;
//extern volatile bit R                  @ ((unsigned)&SSP1STAT*8)+2;
//extern volatile bit D                  @ ((unsigned)&SSP1STAT*8)+5;
//extern volatile bit W                  @ ((unsigned)&SSP1STAT*8)+2;
//extern volatile bit A                  @ ((unsigned)&SSP1STAT*8)+5;
//extern volatile bit nW                 @ ((unsigned)&SSP1STAT*8)+2;
//extern volatile bit nA                 @ ((unsigned)&SSP1STAT*8)+5;
//extern volatile bit R_W                @ ((unsigned)&SSP1STAT*8)+2;
//extern volatile bit D_A                @ ((unsigned)&SSP1STAT*8)+5;
//extern volatile bit nWRITE             @ ((unsigned)&SSP1STAT*8)+2;
//extern volatile bit nADDRESS           @ ((unsigned)&SSP1STAT*8)+5;
extern union {
    struct {
        volatile unsigned BF                  : 1;
        volatile unsigned UA                  : 1;
        volatile unsigned R_nW                : 1;
        volatile unsigned S                   : 1;
        volatile unsigned P                   : 1;
        volatile unsigned D_nA                : 1;
        volatile unsigned CKE                 : 1;
        volatile unsigned SMP                 : 1;
    };
    struct {
        volatile unsigned                     : 2;
        volatile unsigned R                   : 1;
        volatile unsigned : 2;
        volatile unsigned D                   : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned W                   : 1;
        volatile unsigned : 2;
        volatile unsigned A                   : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned nW                  : 1;
        volatile unsigned : 2;
        volatile unsigned nA                  : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned R_W                 : 1;
        volatile unsigned : 2;
        volatile unsigned D_A                 : 1;
    };
    struct {
        volatile unsigned : 2;
        volatile unsigned nWRITE              : 1;
        volatile unsigned : 2;
        volatile unsigned nADDRESS            : 1;
    };
} SSP1STATbits @ 0xFC7;

// Register: SSP1ADD
extern volatile unsigned char           SSP1ADD             @ 0xFC8;
extern volatile unsigned char           SSPADD              @ 0xFC8;
// bit and bitfield definitions

// Register: SSP1BUF
extern volatile unsigned char           SSP1BUF             @ 0xFC9;
extern volatile unsigned char           SSPBUF              @ 0xFC9;
// bit and bitfield definitions

// Register: SSP1MSK
extern volatile unsigned char           SSP1MSK             @ 0xFCA;
extern volatile unsigned char           SSPMSK              @ 0xFCA;
// bit and bitfield definitions
//extern volatile bit MSK0               @ ((unsigned)&SSP1MSK*8)+0;
//extern volatile bit MSK1               @ ((unsigned)&SSP1MSK*8)+1;
//extern volatile bit MSK2               @ ((unsigned)&SSP1MSK*8)+2;
//extern volatile bit MSK3               @ ((unsigned)&SSP1MSK*8)+3;
//extern volatile bit MSK4               @ ((unsigned)&SSP1MSK*8)+4;
//extern volatile bit MSK5               @ ((unsigned)&SSP1MSK*8)+5;
//extern volatile bit MSK6               @ ((unsigned)&SSP1MSK*8)+6;
//extern volatile bit MSK7               @ ((unsigned)&SSP1MSK*8)+7;
extern union {
    struct {
        volatile unsigned MSK0                : 1;
        volatile unsigned MSK1                : 1;
        volatile unsigned MSK2                : 1;
        volatile unsigned MSK3                : 1;
        volatile unsigned MSK4                : 1;
        volatile unsigned MSK5                : 1;
        volatile unsigned MSK6                : 1;
        volatile unsigned MSK7                : 1;
    };
} SSP1MSKbits @ 0xFCA;

// Register: SSP1CON3
extern volatile unsigned char           SSP1CON3            @ 0xFCB;
extern volatile unsigned char           SSPCON3             @ 0xFCB;
// bit and bitfield definitions
//extern volatile bit DHEN               @ ((unsigned)&SSP1CON3*8)+0;
//extern volatile bit AHEN               @ ((unsigned)&SSP1CON3*8)+1;
//extern volatile bit SBCDE              @ ((unsigned)&SSP1CON3*8)+2;
//extern volatile bit SDAHT              @ ((unsigned)&SSP1CON3*8)+3;
//extern volatile bit BOEN               @ ((unsigned)&SSP1CON3*8)+4;
//extern volatile bit SCIE               @ ((unsigned)&SSP1CON3*8)+5;
//extern volatile bit PCIE               @ ((unsigned)&SSP1CON3*8)+6;
//extern volatile bit ACKTIM             @ ((unsigned)&SSP1CON3*8)+7;
extern union {
    struct {
        volatile unsigned DHEN                : 1;
        volatile unsigned AHEN                : 1;
        volatile unsigned SBCDE               : 1;
        volatile unsigned SDAHT               : 1;
        volatile unsigned BOEN                : 1;
        volatile unsigned SCIE                : 1;
        volatile unsigned PCIE                : 1;
        volatile unsigned ACKTIM              : 1;
    };
} SSP1CON3bits @ 0xFCB;

// Register: T1GCON
extern volatile unsigned char           T1GCON              @ 0xFCC;
// bit and bitfield definitions
extern volatile bit T1GVAL              @ ((unsigned)&T1GCON*8)+2;
extern volatile bit T1GGO_nDONE         @ ((unsigned)&T1GCON*8)+3;
extern volatile bit T1GSPM              @ ((unsigned)&T1GCON*8)+4;
extern volatile bit T1GTM               @ ((unsigned)&T1GCON*8)+5;
extern volatile bit T1GPOL              @ ((unsigned)&T1GCON*8)+6;
extern volatile bit TMR1GE              @ ((unsigned)&T1GCON*8)+7;
extern volatile bit T1GSS0              @ ((unsigned)&T1GCON*8)+0;
extern volatile bit T1GSS1              @ ((unsigned)&T1GCON*8)+1;
extern volatile bit T1G_DONE            @ ((unsigned)&T1GCON*8)+3;
extern volatile bit T1GGO               @ ((unsigned)&T1GCON*8)+3;
extern union {
    struct {
        volatile unsigned T1GSS               : 2;
        volatile unsigned T1GVAL              : 1;
        volatile unsigned T1GGO_nDONE         : 1;
        volatile unsigned T1GSPM              : 1;
        volatile unsigned T1GTM               : 1;
        volatile unsigned T1GPOL              : 1;
        volatile unsigned TMR1GE              : 1;
    };
    struct {
        volatile unsigned T1GSS0              : 1;
        volatile unsigned T1GSS1              : 1;
        volatile unsigned                     : 1;
        volatile unsigned T1G_DONE            : 1;
    };
    struct {
        volatile unsigned : 3;
        volatile unsigned T1GGO               : 1;
    };
} T1GCONbits @ 0xFCC;

// Register: T1CON
extern volatile unsigned char           T1CON               @ 0xFCD;
// bit and bitfield definitions
extern volatile bit TMR1ON              @ ((unsigned)&T1CON*8)+0;
extern volatile bit T1RD16              @ ((unsigned)&T1CON*8)+1;
extern volatile bit nT1SYNC             @ ((unsigned)&T1CON*8)+2;
extern volatile bit T1SOSCEN            @ ((unsigned)&T1CON*8)+3;
extern volatile bit RD16                @ ((unsigned)&T1CON*8)+1;
extern volatile bit T1SYNC              @ ((unsigned)&T1CON*8)+2;
extern volatile bit T1OSCEN             @ ((unsigned)&T1CON*8)+3;
extern volatile bit T1CKPS0             @ ((unsigned)&T1CON*8)+4;
extern volatile bit T1CKPS1             @ ((unsigned)&T1CON*8)+5;
extern volatile bit TMR1CS0             @ ((unsigned)&T1CON*8)+6;
extern volatile bit TMR1CS1             @ ((unsigned)&T1CON*8)+7;
extern union {
    struct {
        volatile unsigned TMR1ON              : 1;
        volatile unsigned T1RD16              : 1;
        volatile unsigned nT1SYNC             : 1;
        volatile unsigned T1SOSCEN            : 1;
        volatile unsigned T1CKPS              : 2;
        volatile unsigned TMR1CS              : 2;
    };
    struct {
        volatile unsigned                     : 1;
        volatile unsigned RD16                : 1;
        volatile unsigned T1SYNC              : 1;
        volatile unsigned T1OSCEN             : 1;
        volatile unsigned T1CKPS0             : 1;
        volatile unsigned T1CKPS1             : 1;
        volatile unsigned TMR1CS0             : 1;
        volatile unsigned TMR1CS1             : 1;
    };
} T1CONbits @ 0xFCD;
// bit and bitfield definitions

// Register: TMR1L
extern volatile unsigned char           TMR1L               @ 0xFCE;
// bit and bitfield definitions

// Register: TMR1H
extern volatile unsigned char           TMR1H               @ 0xFCF;
// bit and bitfield definitions

// Register: TMR1
extern volatile unsigned int            TMR1                @ 0xFCE;

// Register: RCON
extern volatile unsigned char           RCON                @ 0xFD0;
// bit and bitfield definitions
extern volatile bit nBOR                @ ((unsigned)&RCON*8)+0;
extern volatile bit nPOR                @ ((unsigned)&RCON*8)+1;
extern volatile bit nPD                 @ ((unsigned)&RCON*8)+2;
extern volatile bit nTO                 @ ((unsigned)&RCON*8)+3;
extern volatile bit nRI                 @ ((unsigned)&RCON*8)+4;
extern volatile bit SBOREN              @ ((unsigned)&RCON*8)+6;
extern volatile bit IPEN                @ ((unsigned)&RCON*8)+7;
extern volatile bit BOR                 @ ((unsigned)&RCON*8)+0;
extern volatile bit POR                 @ ((unsigned)&RCON*8)+1;
extern volatile bit PD                  @ ((unsigned)&RCON*8)+2;
extern volatile bit TO                  @ ((unsigned)&RCON*8)+3;
extern volatile bit RI                  @ ((unsigned)&RCON*8)+4;
extern union {
    struct {
        volatile unsigned nBOR                : 1;
        volatile unsigned nPOR                : 1;
        volatile unsigned nPD                 : 1;
        volatile unsigned nTO                 : 1;
        volatile unsigned nRI                 : 1;
        volatile unsigned                     : 1;
        volatile unsigned SBOREN              : 1;
        volatile unsigned IPEN                : 1;
    };
    struct {
        volatile unsigned BOR                 : 1;
        volatile unsigned POR                 : 1;
        volatile unsigned PD                  : 1;
        volatile unsigned TO                  : 1;
        volatile unsigned RI                  : 1;
    };
} RCONbits @ 0xFD0;

// Register: WDTCON
extern volatile unsigned char           WDTCON              @ 0xFD1;
// bit and bitfield definitions
extern volatile bit SWDTEN              @ ((unsigned)&WDTCON*8)+0;
extern volatile bit SWDTE               @ ((unsigned)&WDTCON*8)+0;
extern union {
    struct {
        volatile unsigned SWDTEN              : 1;
    };
    struct {
        volatile unsigned SWDTE               : 1;
    };
} WDTCONbits @ 0xFD1;

// Register: OSCCON2
extern volatile unsigned char           OSCCON2             @ 0xFD2;
// bit and bitfield definitions
extern volatile bit LFIOFS              @ ((unsigned)&OSCCON2*8)+0;
extern volatile bit MFIOFS              @ ((unsigned)&OSCCON2*8)+1;
extern volatile bit PRISD               @ ((unsigned)&OSCCON2*8)+2;
extern volatile bit SOSCGO              @ ((unsigned)&OSCCON2*8)+3;
extern volatile bit MFIOSEL             @ ((unsigned)&OSCCON2*8)+4;
extern volatile bit SOSCRUN             @ ((unsigned)&OSCCON2*8)+6;
extern volatile bit PLLRDY              @ ((unsigned)&OSCCON2*8)+7;
extern union {
    struct {
        volatile unsigned LFIOFS              : 1;
        volatile unsigned MFIOFS              : 1;
        volatile unsigned PRISD               : 1;
        volatile unsigned SOSCGO              : 1;
        volatile unsigned MFIOSEL             : 1;
        volatile unsigned                     : 1;
        volatile unsigned SOSCRUN             : 1;
        volatile unsigned PLLRDY              : 1;
    };
} OSCCON2bits @ 0xFD2;

// Register: OSCCON
extern volatile unsigned char           OSCCON              @ 0xFD3;
// bit and bitfield definitions
extern volatile bit HFIOFS              @ ((unsigned)&OSCCON*8)+2;
extern volatile bit OSTS                @ ((unsigned)&OSCCON*8)+3;
extern volatile bit IDLEN               @ ((unsigned)&OSCCON*8)+7;
extern volatile bit SCS0                @ ((unsigned)&OSCCON*8)+0;
extern volatile bit SCS1                @ ((unsigned)&OSCCON*8)+1;
extern volatile bit IOFS                @ ((unsigned)&OSCCON*8)+2;
extern volatile bit IRCF0               @ ((unsigned)&OSCCON*8)+4;
extern volatile bit IRCF1               @ ((unsigned)&OSCCON*8)+5;
extern volatile bit IRCF2               @ ((unsigned)&OSCCON*8)+6;
extern union {
    struct {
        volatile unsigned SCS                 : 2;
        volatile unsigned HFIOFS              : 1;
        volatile unsigned OSTS                : 1;
        volatile unsigned IRCF                : 3;
        volatile unsigned IDLEN               : 1;
    };
    struct {
        volatile unsigned SCS0                : 1;
        volatile unsigned SCS1                : 1;
        volatile unsigned IOFS                : 1;
        volatile unsigned                     : 1;
        volatile unsigned IRCF0               : 1;
        volatile unsigned IRCF1               : 1;
        volatile unsigned IRCF2               : 1;
    };
} OSCCONbits @ 0xFD3;

// Register: T0CON
extern volatile unsigned char           T0CON               @ 0xFD5;
// bit and bitfield definitions
extern volatile bit PSA                 @ ((unsigned)&T0CON*8)+3;
extern volatile bit T0SE                @ ((unsigned)&T0CON*8)+4;
extern volatile bit T0CS                @ ((unsigned)&T0CON*8)+5;
extern volatile bit T08BIT              @ ((unsigned)&T0CON*8)+6;
extern volatile bit TMR0ON              @ ((unsigned)&T0CON*8)+7;
extern volatile bit T0PS0               @ ((unsigned)&T0CON*8)+0;
extern volatile bit T0PS1               @ ((unsigned)&T0CON*8)+1;
extern volatile bit T0PS2               @ ((unsigned)&T0CON*8)+2;
extern union {
    struct {
        volatile unsigned T0PS                : 3;
        volatile unsigned PSA                 : 1;
        volatile unsigned T0SE                : 1;
        volatile unsigned T0CS                : 1;
        volatile unsigned T08BIT              : 1;
        volatile unsigned TMR0ON              : 1;
    };
    struct {
        volatile unsigned T0PS0               : 1;
        volatile unsigned T0PS1               : 1;
        volatile unsigned T0PS2               : 1;
    };
} T0CONbits @ 0xFD5;
// bit and bitfield definitions

// Register: TMR0L
extern volatile unsigned char           TMR0L               @ 0xFD6;
// bit and bitfield definitions

// Register: TMR0H
extern volatile unsigned char           TMR0H               @ 0xFD7;
// bit and bitfield definitions

// Register: TMR0
extern volatile unsigned int            TMR0                @ 0xFD6;

// Register: STATUS
extern volatile unsigned char           STATUS              @ 0xFD8;
// bit and bitfield definitions
extern volatile bit CARRY               @ ((unsigned)&STATUS*8)+0;
extern volatile bit DC                  @ ((unsigned)&STATUS*8)+1;
extern volatile bit ZERO                @ ((unsigned)&STATUS*8)+2;
extern volatile bit OV                  @ ((unsigned)&STATUS*8)+3;
extern volatile bit N                   @ ((unsigned)&STATUS*8)+4;
extern union {
    struct {
        volatile unsigned C                   : 1;
        volatile unsigned DC                  : 1;
        volatile unsigned Z                   : 1;
        volatile unsigned OV                  : 1;
        volatile unsigned N                   : 1;
    };
} STATUSbits @ 0xFD8;
// bit and bitfield definitions

// Register: FSR2L
extern volatile unsigned char           FSR2L               @ 0xFD9;
// bit and bitfield definitions

// Register: FSR2H
extern volatile unsigned char           FSR2H               @ 0xFDA;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned                     : 4;
    };
} FSR2Hbits @ 0xFDA;

// Register: FSR2
extern volatile unsigned int            FSR2                @ 0xFD9;

// Register: PLUSW2
extern volatile unsigned char           PLUSW2              @ 0xFDB;
// bit and bitfield definitions

// Register: PREINC2
extern volatile unsigned char           PREINC2             @ 0xFDC;
// bit and bitfield definitions

// Register: POSTDEC2
extern volatile unsigned char           POSTDEC2            @ 0xFDD;
// bit and bitfield definitions

// Register: POSTINC2
extern volatile unsigned char           POSTINC2            @ 0xFDE;
// bit and bitfield definitions

// Register: INDF2
extern volatile unsigned char           INDF2               @ 0xFDF;
// bit and bitfield definitions

// Register: BSR
extern volatile unsigned char           BSR                 @ 0xFE0;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned                     : 4;
    };
} BSRbits @ 0xFE0;
// bit and bitfield definitions

// Register: FSR1L
extern volatile unsigned char           FSR1L               @ 0xFE1;
// bit and bitfield definitions

// Register: FSR1H
extern volatile unsigned char           FSR1H               @ 0xFE2;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned                     : 4;
    };
} FSR1Hbits @ 0xFE2;

// Register: FSR1
extern volatile unsigned int            FSR1                @ 0xFE1;

// Register: PLUSW1
extern volatile unsigned char           PLUSW1              @ 0xFE3;
// bit and bitfield definitions

// Register: PREINC1
extern volatile unsigned char           PREINC1             @ 0xFE4;
// bit and bitfield definitions

// Register: POSTDEC1
extern volatile unsigned char           POSTDEC1            @ 0xFE5;
// bit and bitfield definitions

// Register: POSTINC1
extern volatile unsigned char           POSTINC1            @ 0xFE6;
// bit and bitfield definitions

// Register: INDF1
extern volatile unsigned char           INDF1               @ 0xFE7;
// bit and bitfield definitions

// Register: WREG
extern volatile unsigned char           WREG                @ 0xFE8;
extern volatile unsigned char           W                   @ 0xFE8;
// bit and bitfield definitions
// bit and bitfield definitions

// Register: FSR0L
extern volatile unsigned char           FSR0L               @ 0xFE9;
// bit and bitfield definitions

// Register: FSR0H
extern volatile unsigned char           FSR0H               @ 0xFEA;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned                     : 4;
    };
} FSR0Hbits @ 0xFEA;

// Register: FSR0
extern volatile unsigned int            FSR0                @ 0xFE9;

// Register: PLUSW0
extern volatile unsigned char           PLUSW0              @ 0xFEB;
// bit and bitfield definitions

// Register: PREINC0
extern volatile unsigned char           PREINC0             @ 0xFEC;
// bit and bitfield definitions

// Register: POSTDEC0
extern volatile unsigned char           POSTDEC0            @ 0xFED;
// bit and bitfield definitions

// Register: POSTINC0
extern volatile unsigned char           POSTINC0            @ 0xFEE;
// bit and bitfield definitions

// Register: INDF0
extern volatile unsigned char           INDF0               @ 0xFEF;
// bit and bitfield definitions

// Register: INTCON3
extern volatile unsigned char           INTCON3             @ 0xFF0;
// bit and bitfield definitions
extern volatile bit INT1IF              @ ((unsigned)&INTCON3*8)+0;
extern volatile bit INT2IF              @ ((unsigned)&INTCON3*8)+1;
extern volatile bit INT1IE              @ ((unsigned)&INTCON3*8)+3;
extern volatile bit INT2IE              @ ((unsigned)&INTCON3*8)+4;
extern volatile bit INT1IP              @ ((unsigned)&INTCON3*8)+6;
extern volatile bit INT2IP              @ ((unsigned)&INTCON3*8)+7;
extern volatile bit INT1F               @ ((unsigned)&INTCON3*8)+0;
extern volatile bit INT2F               @ ((unsigned)&INTCON3*8)+1;
extern volatile bit INT1E               @ ((unsigned)&INTCON3*8)+3;
extern volatile bit INT2E               @ ((unsigned)&INTCON3*8)+4;
extern volatile bit INT1P               @ ((unsigned)&INTCON3*8)+6;
extern volatile bit INT2P               @ ((unsigned)&INTCON3*8)+7;
extern union {
    struct {
        volatile unsigned INT1IF              : 1;
        volatile unsigned INT2IF              : 1;
        volatile unsigned                     : 1;
        volatile unsigned INT1IE              : 1;
        volatile unsigned INT2IE              : 1;
        volatile unsigned : 1;
        volatile unsigned INT1IP              : 1;
        volatile unsigned INT2IP              : 1;
    };
    struct {
        volatile unsigned INT1F               : 1;
        volatile unsigned INT2F               : 1;
        volatile unsigned : 1;
        volatile unsigned INT1E               : 1;
        volatile unsigned INT2E               : 1;
        volatile unsigned : 1;
        volatile unsigned INT1P               : 1;
        volatile unsigned INT2P               : 1;
    };
} INTCON3bits @ 0xFF0;

// Register: INTCON2
extern volatile unsigned char           INTCON2             @ 0xFF1;
// bit and bitfield definitions
extern volatile bit RBIP                @ ((unsigned)&INTCON2*8)+0;
extern volatile bit TMR0IP              @ ((unsigned)&INTCON2*8)+2;
extern volatile bit INTEDG2             @ ((unsigned)&INTCON2*8)+4;
extern volatile bit INTEDG1             @ ((unsigned)&INTCON2*8)+5;
extern volatile bit INTEDG0             @ ((unsigned)&INTCON2*8)+6;
extern volatile bit nRBPU               @ ((unsigned)&INTCON2*8)+7;
extern volatile bit RBPU                @ ((unsigned)&INTCON2*8)+7;
extern union {
    struct {
        volatile unsigned RBIP                : 1;
        volatile unsigned                     : 1;
        volatile unsigned TMR0IP              : 1;
        volatile unsigned : 1;
        volatile unsigned INTEDG2             : 1;
        volatile unsigned INTEDG1             : 1;
        volatile unsigned INTEDG0             : 1;
        volatile unsigned nRBPU               : 1;
    };
    struct {
        volatile unsigned : 7;
        volatile unsigned RBPU                : 1;
    };
} INTCON2bits @ 0xFF1;

// Register: INTCON
extern volatile unsigned char           INTCON              @ 0xFF2;
// bit and bitfield definitions
extern volatile bit RBIF                @ ((unsigned)&INTCON*8)+0;
extern volatile bit INT0IF              @ ((unsigned)&INTCON*8)+1;
extern volatile bit TMR0IF              @ ((unsigned)&INTCON*8)+2;
extern volatile bit RBIE                @ ((unsigned)&INTCON*8)+3;
extern volatile bit INT0IE              @ ((unsigned)&INTCON*8)+4;
extern volatile bit TMR0IE              @ ((unsigned)&INTCON*8)+5;
extern volatile bit PEIE_GIEL           @ ((unsigned)&INTCON*8)+6;
extern volatile bit GIE_GIEH            @ ((unsigned)&INTCON*8)+7;
extern volatile bit INT0F               @ ((unsigned)&INTCON*8)+1;
extern volatile bit T0IF                @ ((unsigned)&INTCON*8)+2;
extern volatile bit INT0E               @ ((unsigned)&INTCON*8)+4;
extern volatile bit T0IE                @ ((unsigned)&INTCON*8)+5;
extern volatile bit PEIE                @ ((unsigned)&INTCON*8)+6;
extern volatile bit GIE                 @ ((unsigned)&INTCON*8)+7;
extern volatile bit GIEL                @ ((unsigned)&INTCON*8)+6;
extern volatile bit GIEH                @ ((unsigned)&INTCON*8)+7;
extern union {
    struct {
        volatile unsigned RBIF                : 1;
        volatile unsigned INT0IF              : 1;
        volatile unsigned TMR0IF              : 1;
        volatile unsigned RBIE                : 1;
        volatile unsigned INT0IE              : 1;
        volatile unsigned TMR0IE              : 1;
        volatile unsigned PEIE_GIEL           : 1;
        volatile unsigned GIE_GIEH            : 1;
    };
    struct {
        volatile unsigned                     : 1;
        volatile unsigned INT0F               : 1;
        volatile unsigned T0IF                : 1;
        volatile unsigned : 1;
        volatile unsigned INT0E               : 1;
        volatile unsigned T0IE                : 1;
        volatile unsigned PEIE                : 1;
        volatile unsigned GIE                 : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned GIEL                : 1;
        volatile unsigned GIEH                : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
    };
    struct {
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
        volatile unsigned : 1;
    };
} INTCONbits @ 0xFF2;
// bit and bitfield definitions

// Register: PRODL
extern volatile unsigned char           PRODL               @ 0xFF3;
// bit and bitfield definitions

// Register: PRODH
extern volatile unsigned char           PRODH               @ 0xFF4;
// bit and bitfield definitions

// Register: PROD
extern volatile unsigned int            PROD                @ 0xFF3;

// Register: TABLAT
extern volatile unsigned char           TABLAT              @ 0xFF5;
// bit and bitfield definitions
// bit and bitfield definitions

// Register: TBLPTRL
extern volatile unsigned char           TBLPTRL             @ 0xFF6;
// bit and bitfield definitions

// Register: TBLPTRH
extern volatile unsigned char           TBLPTRH             @ 0xFF7;
// bit and bitfield definitions

// Register: TBLPTRU
extern volatile unsigned char           TBLPTRU             @ 0xFF8;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned TBLPTRU             : 6;
    };
} TBLPTRUbits @ 0xFF8;

// Register: TBLPTR
extern volatile far unsigned char * TBLPTR              @ 0xFF6;
// bit and bitfield definitions

// Register: PCL
extern volatile unsigned char           PCL                 @ 0xFF9;
// bit and bitfield definitions

// Register: PCLATH
extern volatile unsigned char           PCLATH              @ 0xFFA;
// bit and bitfield definitions

// Register: PCLATU
extern volatile unsigned char           PCLATU              @ 0xFFB;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned                     : 5;
    };
} PCLATUbits @ 0xFFB;

// Register: PCLAT
extern volatile unsigned short long int PCLAT               @ 0xFF9;

// Register: STKPTR
extern volatile unsigned char           STKPTR              @ 0xFFC;
// bit and bitfield definitions
extern volatile bit STKUNF              @ ((unsigned)&STKPTR*8)+6;
extern volatile bit STKFUL              @ ((unsigned)&STKPTR*8)+7;
extern volatile bit STKPTR0             @ ((unsigned)&STKPTR*8)+0;
extern volatile bit STKPTR1             @ ((unsigned)&STKPTR*8)+1;
extern volatile bit STKPTR2             @ ((unsigned)&STKPTR*8)+2;
extern volatile bit STKPTR3             @ ((unsigned)&STKPTR*8)+3;
extern volatile bit STKPTR4             @ ((unsigned)&STKPTR*8)+4;
extern volatile bit STKOVF              @ ((unsigned)&STKPTR*8)+7;
extern volatile bit SP0                 @ ((unsigned)&STKPTR*8)+0;
extern volatile bit SP1                 @ ((unsigned)&STKPTR*8)+1;
extern volatile bit SP2                 @ ((unsigned)&STKPTR*8)+2;
extern volatile bit SP3                 @ ((unsigned)&STKPTR*8)+3;
extern volatile bit SP4                 @ ((unsigned)&STKPTR*8)+4;
extern union {
    struct {
        volatile unsigned STKPTR              : 5;
        volatile unsigned                     : 1;
        volatile unsigned STKUNF              : 1;
        volatile unsigned STKFUL              : 1;
    };
    struct {
        volatile unsigned STKPTR0             : 1;
        volatile unsigned STKPTR1             : 1;
        volatile unsigned STKPTR2             : 1;
        volatile unsigned STKPTR3             : 1;
        volatile unsigned STKPTR4             : 1;
        volatile unsigned : 2;
        volatile unsigned STKOVF              : 1;
    };
    struct {
        volatile unsigned SP0                 : 1;
        volatile unsigned SP1                 : 1;
        volatile unsigned SP2                 : 1;
        volatile unsigned SP3                 : 1;
        volatile unsigned SP4                 : 1;
    };
} STKPTRbits @ 0xFFC;
// bit and bitfield definitions

// Register: TOSL
extern volatile unsigned char           TOSL                @ 0xFFD;
// bit and bitfield definitions

// Register: TOSH
extern volatile unsigned char           TOSH                @ 0xFFE;
// bit and bitfield definitions

// Register: TOSU
extern volatile unsigned char           TOSU                @ 0xFFF;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned                     : 5;
    };
} TOSUbits @ 0xFFF;

// Register: TOS
extern volatile unsigned short long int TOS                 @ 0xFFD;

// Register: TMR0_Internal
extern volatile unsigned int            TMR0_Internal       @ 0x1000;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned InternalTMR         : 16;
    };
} TMR0_Internalbits @ 0x1000;

// Register: TMR0_Prescale
extern volatile unsigned char           TMR0_Prescale       @ 0x1002;
// bit and bitfield definitions

// Register: TMR1_Internal
extern volatile unsigned int            TMR1_Internal       @ 0x1003;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned InternalTMR         : 16;
    };
} TMR1_Internalbits @ 0x1003;

// Register: TMR1_Prescale
extern volatile unsigned char           TMR1_Prescale       @ 0x1005;
// bit and bitfield definitions

// Register: TMR2_Prescale
extern volatile unsigned char           TMR2_Prescale       @ 0x1006;
// bit and bitfield definitions

// Register: TMR3_Internal
extern volatile unsigned int            TMR3_Internal       @ 0x1007;
// bit and bitfield definitions
extern union {
    struct {
        volatile unsigned InternalTMR         : 16;
    };
} TMR3_Internalbits @ 0x1007;

// Register: TMR3_Prescale
extern volatile unsigned char           TMR3_Prescale       @ 0x1009;
// bit and bitfield definitions


#endif
