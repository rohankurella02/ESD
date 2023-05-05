//1. // LED blink
#include <lpc17xx.h>

    void
    delay_ms(unsigned int ms)
{
    unsigned int i, j;

    for (i = 0; i < ms; i++)
        for (j = 0; j < 20000; j++)
            ;
}

/* start the main program */
int main(void)
{
    SystemInit();                     /*Clock and PLL configuration*/
    LPC_PINCON->PINSEL4 = 0x00000000; /*Configure the PORT2 Pins as GPIO;*/
    LPC_GPIO2->FIODIR = 0x000000ff;   /*Configure the PORT2 pins as OUTPUT;*/

    while (1)
    {
        LPC_GPIO2->FIOSET = 0x000000ff; /* Make all the Port pins as high */
        delay_ms(10);

        LPC_GPIO2->FIOCLR = 0x000000ff; /* Make all the Port pins as low */
        delay_ms(10);
    }
}

// 2. Systick LED program // Praogram//
#include <LPC17xx.h>

/* Systick Register address, refer datasheet for more info */
#define STCTRL (*((volatile unsigned long *)0xE000E010))
#define STRELOAD (*((volatile unsigned long *)0xE000E014))
#define STCURR (*((volatile unsigned long *)0xE000E018))

/*******STCTRL bits*******/

#define SBIT_ENABLE 0
#define SBIT_TICKINT 1
#define SBIT_CLKSOURCE 2

/* 100000000Mhz * 1ms = 1000000 - 1 */
#define RELOAD_VALUE 99999999

#define LED 2 // P2_2

int main(void)
{
    SystemInit();

    STRELOAD = RELOAD_VALUE; // Reload value for 100ms tick

    /* Enable the Systick, Systick Interrup and select CPU Clock Source */
    STCTRL = (1 << SBIT_ENABLE) | (1 << SBIT_TICKINT) | (1 << SBIT_CLKSOURCE);

    LPC_GPIO2->FIODIR = (1 << LED); /* Configure the Led Pin as Output */

    while (1)
    {
        // do nothing
    }
}

void SysTick_Handler(void)
{
    LPC_GPIO2->FIOPIN ^= (1 << LED); /* Toggle the LED1 (P2_0) */
}

//3. // PLL ON Program
#include <LPC17xx.h>

#define CCLKCFG (*(volatile unsigned long *)(0x400FC104))
#define PLL0CON (*(volatile unsigned long *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned long *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned long *)(0x400FC088))
#define PLL0CFG (*(volatile unsigned long *)(0x400FC084))

// function prototypes
void delay(void);

int main()
{

    LPC_GPIO2->FIODIR |= 0x0000007C;
    // CCLKCFG=0x000000EE; // divider divides by this number plus 1
    // Set PLL0 multiplier
    PLL0CFG = 0x0015013A;  // arbitrary multiply value, divide value left at 1
    PLL0FEED = 0x000000AA; // Feed the PLL
    PLL0FEED = 0x00000055;
    // Turn on PLL0
    PLL0CON |= 1 << 0;
    PLL0FEED = 0x000000AA; // Feed the PLL
    PLL0FEED = 0x00000055;
    // Wait for main PLL (PLL0) to come up
    while ((PLL0STAT & (1 << 24)) == 0x00);
    // Wait for PLOCK0 to become 1
    while ((PLL0STAT & (1 << 26)) == 0x00);
    // Connect to the PLL0

    PLL0CON |= 1 << 1;
    PLL0FEED = 0x000000AA; // Feed the PLL
    PLL0FEED = 0x00000055;
    while ((PLL0STAT & (1 << 25)) == 0x00); // Wait for PLL0 to connect
    while (1)
    {
        LPC_GPIO2->FIOPIN ^= (0x0000007C);
        delay();
    }
}

void delay(void)
{ // delay function.
    int j; // loop variable j
    for (j = 0; j < 50000; j++)
    {
        j++;
        j--; // waste time
    }
}


//4. PLL OFF Program
#include <LPC17xx.h>

#define CCLKCFG (*(volatile unsigned char *)(0x400FC104))
#define PLL0CON (*(volatile unsigned char *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned char *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned char *)(0x400FC088))

// function prototypes
void delay(void);

int main()
{

    LPC_GPIO2->FIODIR |= 0x0000007C;
    CCLKCFG = 0x000000FF;
    // Disconnect PLL0
    PLL0CON &= !(1 << 1); // Clears bit 1 of PLL0CON, the Connect bit
    PLL0FEED = 0xAA;      // Feed the PLL. Enables action of above line
    PLL0FEED = 0x55;      //
    // Wait for PLL0 to disconnect. Wait for bit 25 to become 0.
    while ((PLL0STAT & (1 << 25)) != 0x00); // Bit 25 shows connection status
    // Turn off PLL0; on completion, PLL0 is bypassed.
    PLL0CON &= !(1 << 0); // Bit 0 of PLL0CON disables PLL
    PLL0FEED = 0xAA;      // Feed the PLL. Enables action of above line
    PLL0FEED = 0x55;
    // Wait for PLL0 to shut down
    while ((PLL0STAT & (1 << 24)) != 0x00); // Bit 24 shows enable status
    /****Insert Optional Extra Code Here****
    to change PLL0 settings or clock source.
    **OR** just continue with PLL0 disabled and bypassed*/
    // blink at the new clock frequency
    while (1)
    {
        LPC_GPIO2->FIOPIN ^= (0x0000007C);
        delay();
    }
}

void delay(void)
{ // delay function.
    int j; // loop variable j
    for (j = 0; j < 5000000; j++)
    {
        j++;
        j--; // waste time
    }
}

//5. // PWM Program//

#include <lpc17xx.h>

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 50000; j++)
            ;
}

#define SBIT_CNTEN 0
#define SBIT_PWMEN 2
#define SBIT_PWMMR0R 1

#define SBIT_LEN0 0
#define SBIT_LEN1 1
#define SBIT_LEN2 2
#define SBIT_LEN3 3
#define SBIT_LEN4 4

#define SBIT_PWMENA1 9
#define SBIT_PWMENA2 10
#define SBIT_PWMENA3 11
#define SBIT_PWMENA4 12

#define PWM_1 0 // P2_0 (0-1 Bits of PINSEL4)
#define PWM_2 2 // P2_1 (2-3 Bits of PINSEL4)
#define PWM_3 4 // P2_2 (4-5 Bits of PINSEL4)
#define PWM_4 6 // P2_3 (6-7 Bits of PINSEL4)

int main(void)
{
    int dutyCycle;
    SystemInit();
    /* Cofigure pins(P2_0 - P2_3) for PWM mode. */
    LPC_PINCON->PINSEL4 = (1 << PWM_1) | (1 << PWM_2) | (1 << PWM_3) | (1 << PWM_4);

    /* Enable Counters,PWM module */
    LPC_PWM1->TCR = (1 << SBIT_CNTEN) | (1 << SBIT_PWMEN);

    LPC_PWM1->PR = 0x0; /* No Prescalar */
    LPC_PWM1->MCR = (1 << SBIT_PWMMR0R); /*Reset on PWMMR0, reset TC if it matches MR0 */
    LPC_PWM1->MR0 = 100; /* set PWM cycle(Ton+Toff)=100) */
    LPC_PWM1->MR1 = 50;  /* Set 50% Duty Cycle for all four channels */
    LPC_PWM1->MR2 = 50;
    LPC_PWM1->MR3 = 50;
    LPC_PWM1->MR4 = 50;
    /* Trigger the latch Enable Bits to load the new Match Values */
    LPC_PWM1->LER = (1 << SBIT_LEN0) | (1 << SBIT_LEN1) |
                    (1 << SBIT_LEN2) | (1 << SBIT_LEN3) | (1 << SBIT_LEN4);

    /* Enable the PWM output pins for PWM_1-PWM_4(P2_0 - P2_3) */
    LPC_PWM1->PCR = (1 << SBIT_PWMENA1) | (1 << SBIT_PWMENA2) |
                    (1 << SBIT_PWMENA3) | (1 << SBIT_PWMENA4);

    while (1)
    {
        for (dutyCycle = 0; dutyCycle < 100; dutyCycle++)
        {
            LPC_PWM1->MR1 = dutyCycle; /* Increase the dutyCycle from 0-
            100 */
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->MR4 = dutyCycle;
            /* Trigger the latch Enable Bits to load the new Match Values */
            LPC_PWM1->LER = (1 << SBIT_LEN0) | (1 << SBIT_LEN1) |
                            (1 << SBIT_LEN2) | (1 << SBIT_LEN3) | (1 << SBIT_LEN4);
            delay_ms(5);
        }
        for (dutyCycle = 100; dutyCycle > 0; dutyCycle--)
        {
            LPC_PWM1->MR1 = dutyCycle; /* Decrease the dutyCycle from
            100-0 */
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->MR4 = dutyCycle;
            /* Trigger the latch Enable Bits to load the new Match Values */
            LPC_PWM1->LER = (1 << SBIT_LEN0) | (1 << SBIT_LEN1) |
                            (1 << SBIT_LEN2) | (1 << SBIT_LEN3) | (1 << SBIT_LEN4);
            delay_ms(5);
        }
    }
}


//6. // Interrupt Program //
#include <lpc17xx.h>

#define PINSEL_EINT0 20
#define PINSEL_EINT1 22

#define LED1 0
#define LED2 1

#define SBIT_EINT0 0
#define SBIT_EINT1 1

#define SBIT_EXTMODE0 0
#define SBIT_EXTMODE1 1

#define SBIT_EXTPOLAR0 0
#define SBIT_EXTPOLAR1 1

void EINT0_IRQHandler(void)
{
    LPC_SC->EXTINT = (1 << SBIT_EINT0); /* Clear Interrupt Flag */
    LPC_GPIO2->FIOPIN ^= (1 << LED1);   /* Toggle the LED1 everytime
       INTR0 is generated */
}

void EINT1_IRQHandler(void)
{
    LPC_SC->EXTINT = (1 << SBIT_EINT1); /* Clear Interrupt Flag */
    LPC_GPIO2->FIOPIN ^= (1 << LED2);   /* Toggle the LED2 everytime
       INTR1 is generated */
}

int main()
{
    SystemInit();

    LPC_SC->EXTINT = (1 << SBIT_EINT0) | (1 << SBIT_EINT1); /*
        Clear Pending interrupts */
    LPC_PINCON->PINSEL4 = (1 << PINSEL_EINT0) |
                          (1 << PINSEL_EINT1); /* Configure P2_10,P2_11 as EINT0/1 */
    LPC_SC->EXTMODE = (1 << SBIT_EXTMODE0) |
                      (1 << SBIT_EXTMODE1); /* Configure EINTx as Edge Triggered*/
    LPC_SC->EXTPOLAR = (1 << SBIT_EXTPOLAR0) |
                       (1 << SBIT_EXTPOLAR0); /* Configure EINTx as Falling Edge */

    LPC_GPIO2->FIODIR = (1 << LED1) | (1 << LED2); /*
        Configure LED pins as OUTPUT */
    LPC_GPIO2->FIOPIN = 0x00;

    NVIC_EnableIRQ(EINT0_IRQn); /* Enable the EINT0,EINT1 interrupts
                                 */
    NVIC_EnableIRQ(EINT1_IRQn);
    while (1)
    {
        // Do nothing
    }
}


//7. // UART Hello world Program//
#include <LPC17xx.h>

void delay(unsigned int r1);
void UART0_Init(void);
void UART0_IRQHandler(void);

unsigned long int r = 0, i = 0;
unsigned char tx0_flag = 0;
unsigned char *ptr, arr[] = "Hello world";

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    UART0_Init();
    while (1)
    {
        ptr = arr;
        while (*ptr != '\0')
        {
            LPC_UART0->THR = *ptr++;
            while (tx0_flag == 0x00)
                ;
            tx0_flag = 0x00;
            for (i = 0; i < 200; i++)
                ;
        }
        for (i = 0; i < 500; i++)
            delay(625); // delay
    }
}

void UART0_Init(void)
{
    LPC_SC->PCONP |= 0x00000008;       // UART0 peripheral enable
    LPC_PINCON->PINSEL0 |= 0x00000050; // for selecting TX0[P0.2-->5:4]
    and RX0 [P0 .3 -- > 7:6] of UART0
            LPC_UART0->LCR = 0x00000083; // enable divisor latch, parity disable, 1
    stop bit, 8bit word length line control register LPC_UART0->DLM = 0X00;
    LPC_UART0->DLL = 0x13; // select baud rate 9600 bps
    LPC_UART0->LCR = 0X00000003;
    LPC_UART0->FCR = 0x07;
    LPC_UART0->IER = 0X03; // select Transmit and receive interrupt

    NVIC_EnableIRQ(UART0_IRQn); // Assigning channel
}

void UART0_IRQHandler(void)
{
    unsigned long Int_Stat;
    Int_Stat = LPC_UART0->IIR;           // reading the data from interrupt identification register 
    Int_Stat = Int_Stat & 0x06; // masking other than txmit int & rcve data indicator
    if ((Int_Stat & 0x02) == 0x02) // transmit interrupt
        tx0_flag = 0xff;
}

void delay(unsigned int r1)
{
    for (r = 0; r < r1; r++);
}

//8. // Polling Program //
#include <lpc17xx.h>

#define SwitchPinNumber 11
#define LedPinNumber 0

/* start the main program */
int main()
{
    uint32_t switchStatus;
    SystemInit();                   /* Clock and PLL configuration */
    LPC_PINCON->PINSEL4 = 0x000000; /* Configure the Pins for
    GPIO */
    /* Configure the LED pin as output and SwitchPin as input */
    LPC_GPIO2->FIODIR = ((1 << LedPinNumber) | (0 << SwitchPinNumber));

    while (1)
    {
        /* Turn On all the leds and wait for one second */
        switchStatus = (LPC_GPIO2->FIOPIN >> SwitchPinNumber) & 0x01; /*
         Read the switch status */
        if (switchStatus == 1) /* Turn ON/OFF LEDs depending
         on switch status */
        {
            LPC_GPIO2->FIOPIN = (1 << LedPinNumber);
        }
        else
        {
            LPC_GPIO2->FIOPIN = (0 << LedPinNumber);
        }
    }
}