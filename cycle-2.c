//1. // LED blink
#include <lpc17xx.h>

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 20000; j++);
}
int main(void)
{
    SystemInit();                     
    LPC_PINCON->PINSEL4 = 0x00000000; 
    LPC_GPIO2->FIODIR = 0x000000ff;  

    while (1)
    {
        LPC_GPIO2->FIOSET = 0x000000ff; 
        delay_ms(10);

        LPC_GPIO2->FIOCLR = 0x000000ff; 
        delay_ms(10);
    }
}

// 2. Systick LED program // Praogram//
#include <LPC17xx.h>

#define STCTRL (*((volatile unsigned long *)0xE000E010))
#define STRELOAD (*((volatile unsigned long *)0xE000E014))
#define STCURR (*((volatile unsigned long *)0xE000E018))

#define SBIT_ENABLE 0
#define SBIT_TICKINT 1
#define SBIT_CLKSOURCE 2
#define RELOAD_VALUE 99999999
#define LED 2 
int main(void)
{
    SystemInit();
    STRELOAD = RELOAD_VALUE; 
    STCTRL = (1 << SBIT_ENABLE) | (1 << SBIT_TICKINT) | (1 << SBIT_CLKSOURCE);
    LPC_GPIO2->FIODIR = (1 << LED);
    while (1)
    {

    }
}
void SysTick_Handler(void)
{
    LPC_GPIO2->FIOPIN ^= (1 << LED);
}

//3. // PLL ON Program
#include <LPC17xx.h>
#define CCLKCFG (*(volatile unsigned long *)(0x400FC104))
#define PLL0CON (*(volatile unsigned long *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned long *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned long *)(0x400FC088))
#define PLL0CFG (*(volatile unsigned long *)(0x400FC084))

void delay(void);

int main()
{

    LPC_GPIO2->FIODIR |= 0x0000007C;
    PLL0CFG = 0x0015013A; 
    PLL0FEED = 0x000000AA;
    PLL0FEED = 0x00000055;
    PLL0CON |= 1 << 0;
    PLL0FEED = 0x000000AA;
    PLL0FEED = 0x00000055;
    while ((PLL0STAT & (1 << 24)) == 0x00);
    while ((PLL0STAT & (1 << 26)) == 0x00);
    PLL0CON |= 1 << 1;
    PLL0FEED = 0x000000AA;
    PLL0FEED = 0x00000055;
    while ((PLL0STAT & (1 << 25)) == 0x00);
    while (1)
    {
        LPC_GPIO2->FIOPIN ^= (0x0000007C);
        delay();
    }
}

void delay(void)
{ 
    int j; 
    for (j = 0; j < 50000; j++)
    {
        j++;
        j--; 
    }
}


//4. PLL OFF Program
#include <LPC17xx.h>

#define CCLKCFG (*(volatile unsigned char *)(0x400FC104))
#define PLL0CON (*(volatile unsigned char *)(0x400FC080))
#define PLL0FEED (*(volatile unsigned char *)(0x400FC08C))
#define PLL0STAT (*(volatile unsigned char *)(0x400FC088))

void delay(void);

int main()
{

    LPC_GPIO2->FIODIR |= 0x0000007C;
    CCLKCFG = 0x000000FF;
    PLL0CON &= !(1 << 1);
    PLL0FEED = 0xAA;  
    PLL0FEED = 0x55; 
    while ((PLL0STAT & (1 << 25)) != 0x00);
    PLL0CON &= !(1 << 0);
    PLL0FEED = 0xAA; 
    PLL0FEED = 0x55;
    while ((PLL0STAT & (1 << 24)) != 0x00);
    while (1)
    {
        LPC_GPIO2->FIOPIN ^= (0x0000007C);
        delay();
    }
}

void delay(void)
{
    int j;
    for (j = 0; j < 5000000; j++)
    {
        j++;
        j--;
    }
}

//5. // PWM Program//
#include <lpc17xx.h>

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 50000; j++);
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

#define PWM_1 0
#define PWM_2 2 
#define PWM_3 4 
#define PWM_4 6

int main(void)
{
    int dutyCycle;
    SystemInit();
    LPC_PINCON->PINSEL4 = (1 << PWM_1) | (1 << PWM_2) | (1 << PWM_3) | (1 << PWM_4);
    LPC_PWM1->TCR = (1 << SBIT_CNTEN) | (1 << SBIT_PWMEN);
    LPC_PWM1->PR = 0x0; 
    LPC_PWM1->MCR = (1 << SBIT_PWMMR0R);
    LPC_PWM1->MR0 = 100; 
    LPC_PWM1->MR1 = 50; 
    LPC_PWM1->MR2 = 50;
    LPC_PWM1->MR3 = 50;
    LPC_PWM1->MR4 = 50;
    LPC_PWM1->LER = (1 << SBIT_LEN0) | (1 << SBIT_LEN1) |
                    (1 << SBIT_LEN2) | (1 << SBIT_LEN3) | (1 << SBIT_LEN4);
    LPC_PWM1->PCR = (1 << SBIT_PWMENA1) | (1 << SBIT_PWMENA2) |
                    (1 << SBIT_PWMENA3) | (1 << SBIT_PWMENA4);
    while (1)
    {
        for (dutyCycle = 0; dutyCycle < 100; dutyCycle++)
        {
            LPC_PWM1->MR1 = dutyCycle; 
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->MR4 = dutyCycle;
            LPC_PWM1->LER = (1 << SBIT_LEN0) | (1 << SBIT_LEN1) |
                            (1 << SBIT_LEN2) | (1 << SBIT_LEN3) | (1 << SBIT_LEN4);
            delay_ms(5);
        }
        for (dutyCycle = 100; dutyCycle > 0; dutyCycle--)
        {
            LPC_PWM1->MR1 = dutyCycle;
            LPC_PWM1->MR2 = dutyCycle;
            LPC_PWM1->MR3 = dutyCycle;
            LPC_PWM1->MR4 = dutyCycle;
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
    LPC_SC->EXTINT = (1 << SBIT_EINT0);
    LPC_GPIO2->FIOPIN ^= (1 << LED1);
}

void EINT1_IRQHandler(void)
{
    LPC_SC->EXTINT = (1 << SBIT_EINT1); 
    LPC_GPIO2->FIOPIN ^= (1 << LED2);
}

int main()
{
    SystemInit();

    LPC_SC->EXTINT = (1 << SBIT_EINT0) | (1 << SBIT_EINT1); 
    LPC_PINCON->PINSEL4 = (1 << PINSEL_EINT0) |
                          (1 << PINSEL_EINT1); 
    LPC_SC->EXTMODE = (1 << SBIT_EXTMODE0) |
                      (1 << SBIT_EXTMODE1);
    LPC_SC->EXTPOLAR = (1 << SBIT_EXTPOLAR0) |
                       (1 << SBIT_EXTPOLAR0); 
    LPC_GPIO2->FIODIR = (1 << LED1) | (1 << LED2); 
    LPC_GPIO2->FIOPIN = 0x00;
    NVIC_EnableIRQ(EINT0_IRQn);
    NVIC_EnableIRQ(EINT1_IRQn);
    while (1)
    {

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
            delay(625);
    }
}

void UART0_Init(void)
{
    LPC_SC->PCONP |= 0x00000008; 
    LPC_PINCON->PINSEL0 |= 0x00000050;
    LPC_UART0->LCR = 0x00000083; 
    LPC_UART0->DLM = 0X00;
    LPC_UART0->DLL = 0x13; 
    LPC_UART0->LCR = 0X00000003;
    LPC_UART0->FCR = 0x07;
    LPC_UART0->IER = 0X03;
    NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler(void)
{
    unsigned long Int_Stat;
    Int_Stat = LPC_UART0->IIR;
    Int_Stat = Int_Stat & 0x06;
    if ((Int_Stat & 0x02) == 0x02) 
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

int main()
{
    uint32_t switchStatus;
    SystemInit();
    LPC_PINCON->PINSEL4 = 0x000000;
    LPC_GPIO2->FIODIR = ((1 << LedPinNumber) | (0 << SwitchPinNumber));
    while (1)
    {
        switchStatus = (LPC_GPIO2->FIOPIN >> SwitchPinNumber) & 0x01; 
        if (switchStatus == 1)
        {
            LPC_GPIO2->FIOPIN = (1 << LedPinNumber);
        }
        else
        {
            LPC_GPIO2->FIOPIN = (0 << LedPinNumber);
        }
    }
}