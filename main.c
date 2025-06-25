#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
short i = 4;
short j = 1;
short adc_value[3] ;
char buffer[50];  
char BUFFER_REC[100]; // Buffer to store received characters
char buf[100];
int k = 0;            // Index for the buffer
void configure_interrupt_priorities(void) {
    // Set the priority for Timer interrupts (TIM6)
    NVIC_SetPriority(TIM6_DAC_IRQn, 4); // Priority level 1 (lower number = higher priority)

    // Set the priority for ADC interrupts (ADC1)
    NVIC_SetPriority(ADC_IRQn, 5); // Priority level 2

    // Set the priority for EXTI interrupts (EXTI0)
    NVIC_SetPriority(EXTI0_IRQn, 2); // Priority level 3

    // Set the priority for USART2 interrupts (USART2)
    NVIC_SetPriority(USART2_IRQn, 3); // Priority level 4

}
void TIM3_PWM_Config(void)
{
    // Activer l'horloge TIM3 et GPIOB
    RCC->APB1ENR |= (1 << 1);    // Activer l'horloge de TIM3
    RCC->AHB1ENR |= (1 << 0) | (1 << 1);    // Activer l'horloge de GPIOA et GPIOB

    // Configurer PA6 , PA7, PB0, PB1 pour TIM3 (cannels for the timer3) as AF2
    GPIOA->MODER |= (2 << (6 * 2)) | (2 << (7 * 2));    // Mettre PA6 et PA7 en mode fonction alternative
    GPIOB->MODER |= (2 << (0 * 2)) | (2 << (1 * 2));    // Mettre PB0 et PB1 en mode fonction alternative
    GPIOA->AFR[0] |= (2 << (6 * 4)) | (2 << (7 * 4));    // Sélectionner AF2 pour PA6 et PA7
    GPIOB->AFR[0] |= (2 << (0 * 4)) | (2 << (1 * 4));    // Sélectionner AF2 pour PB0 et PB1

    // Configurer TIM3 pour PWM
    TIM3->PSC = 15;                // Prescaler: 16 MHz / (15 + 1) = 1MHz
    TIM3->ARR = 999;               // Auto-reload: 1 MHz / (999 + 1) = 1 KHz (Freq ta3 l PWM)

    // Configure PWM mode for all channels
    TIM3->CCMR1 |= (6 << 4) | (6 << 12);  // PWM mode 1 for CH1 and CH2
    TIM3->CCMR2 |= (6 << 4) | (6 << 12);  // PWM mode 1 for CH3 and CH4

    // Enable the output channels
    TIM3->CCER |= 0x1111;

    // Initialize PWM duty cycle to 0
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;

    // Start the timer
 
}
void USART2_Init(void)
{
   
    RCC->AHB1ENR |= (1 << 0);             // Enable GPIOA clock
    RCC->APB1ENR |= (1 << 17);            // Enable USART2 clock

    // Configure PA2 as TX and PA3 as RX in alternate function mode
    GPIOA->MODER |= (1 << 5) | (1 << 7);  // Set PA2 and PA3 to alternate function mode (MODER[2*2]=10, MODER[2*3]=10)
    GPIOA->AFR[0] |= (7 << 8) | (7 << 12);// Set alternate function 7 for PA2 (AFR[2]) and PA3 (AFR[3])

    // Configure USART2
    USART2->BRR = 0x683;                  // Set baud rate to 9600 (assuming 16 MHz clock)
    USART2->CR1 |= (1 << 3) ;             // Enable transmitter (TE)
    USART2->CR1 |= (1 << 2) ;             // Enable receiver (RE)
    USART2->CR1 |= (1 << 5) ;             // Enable RXNE interrupt (RXNEIE)
    USART2->CR1 |= (1 << 4) ;             // Enable IDLE interrupt (IDLEIE)
    USART2->CR1 |= (1 << 13);             // Enable USART2 (UE)

    // Enable USART2 interrupt in the NVIC
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_SendChar(char c)
{
    while (!((USART2->SR >> 7) & 1));     // Wait until TXE (bit 7) is set
    USART2->DR = c;                       // Write data to the USART data register
}

void USART2_SendString(char *str)
{
    while (*str)
    {
        USART2_SendChar(*str);            // Send each character in the string
        str++;
    }
}

void USART2_IRQHandler(void)
{
    // RXNE flag: A character is received
    if ((USART2->SR >> 5) & 1)  // Check if RXNE (bit 5) is set
    {
        if (k < 99)  // Prevent buffer overflow
        {
            BUFFER_REC[k++] = (char)USART2->DR;
        }
    }

    // IDLE flag: End of a message
    if ((USART2->SR >> 4) & 1)  // Check if IDLE (bit 4) is set
    {
        (void)USART2->DR; // Clear the IDLE flag by reading DR

        BUFFER_REC[k] = '\0';  // Null-terminate the received string
        strcpy(buf, BUFFER_REC);  // Copy received data to buf

        if (strstr(buf, "AVANCE+")) {
            if ((TIM3->CCR1 < 999) && (TIM3->CCR3 < 999)) {
                TIM3->CCR1 += 100;
                TIM3->CCR2 = 0;
                TIM3->CCR3 += 100;
                TIM3->CCR4 = 0;
            }
        } else if (strstr(buf, "AVANCE-")) {
            if ((TIM3->CCR1 > 100) && (TIM3->CCR3 > 100)) {
                TIM3->CCR1 -= 100;
                TIM3->CCR2 = 0;
                TIM3->CCR3 -= 100;
                TIM3->CCR4 = 0;
            }
        } else if (strstr(buf, "STOP")) {
            TIM3->CCR1 = 0;
            TIM3->CCR2 = 0;
            TIM3->CCR3 = 0;
            TIM3->CCR4 = 0;
        } else if (strstr(buf, "ARRIERE+")) {
            if ((TIM3->CCR2 < 999) && (TIM3->CCR4 <999)) {
                TIM3->CCR1 = 0;
                TIM3->CCR2 += 100;
                TIM3->CCR3 = 0;
                TIM3->CCR4 += 100;
            }
        } else if (strstr(buf, "ARRIERE-")) {
            if ((TIM3->CCR2 > 100) && (TIM3->CCR4 > 100)) {
                TIM3->CCR1 = 0;
                TIM3->CCR2 -= 100;
                TIM3->CCR3 = 0;
                TIM3->CCR4 -= 100;
            }
        } else if (strstr(buf, "DROITE")) {
            TIM3->CCR1 = 999;
            TIM3->CCR2 = 0;
            TIM3->CCR3 = 0;
            TIM3->CCR4 = 0;
        } else if (strstr(buf, "GAUCHE")) {
            TIM3->CCR1 = 0;
            TIM3->CCR2 = 0;
            TIM3->CCR3 = 999;
            TIM3->CCR4 = 0;
        }

        USART2_SendString(BUFFER_REC);  // Echo received command
        memset(BUFFER_REC, 0, sizeof(BUFFER_REC));  // Clear buffer
        memset(buf, 0, sizeof(BUFFER_REC));  // Clear buffer
        k = 0;  // Reset index
    }
}


void config_TIMER6(void)
{
    RCC->APB1ENR |= (1 << 4);    // Enable TIM6 clock
    TIM6->PSC = 15999;           // Set prescaler to 15999 (1 kHz clock)
    TIM6->ARR = 999;            // Set auto-reload to 999 (1 seconds interval)
    TIM6->DIER = 0x1;            // Enable update interrupt
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

}

void config_EXTI(void)
{
    RCC->APB2ENR |= 1 << 14;     // Enable SYSCFG clock
    SYSCFG->EXTICR[0] = 0X0; // Select PA0 as EXTI0 source
    EXTI->RTSR |= 0x1;           // Trigger interrupt on rising edge of PA0
    EXTI->IMR |= 0x1;            // Enable interrupt for EXTI line 0
    NVIC_EnableIRQ(EXTI0_IRQn);  // Enable EXTI0 interrupt in NVIC
}
void config_ADC(void)
{   RCC->AHB1ENR |= 0X3;
    GPIOB->MODER |= (3 << 8) | (3 << 10) | (3 << 12); // PB4, PB5, PB6 analog mode
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enable clock for adc1

    ADC1->CR1 = (ADC_CR1_SCAN | ADC_CR1_DISCEN | ADC_CR1_EOCIE);  //mode scan  /  discontinue enabled / enable interrupt
    ADC1->CR1 |= (0 << 24) | (0 << 25) ;       // RES à 0 result is set to 12 bit
    ADC1->CR2 |= ADC_CR2_EOCS ; // enable EOC flag
    ADC1->SQR1 = (0 << 20);      // Sequence length = 1 (L=0)
    ADC1->SMPR2 |= (0 << 3); //Channel 1 sampling time (239.5 cycles)
    NVIC_EnableIRQ(ADC_IRQn);    // Enable ADC whole interrupt
    ADC1->SQR3 = 1 << 0; // Le canal i est configuré pour SQ1
    ADC1->CR2 |= 1;              // Turn on ADC
}
void EXTI0_IRQHandler(void)
{
    if ((EXTI->PR & 0x1) != 0) // If interrupt flag is set
    {
 j++;
        TIM6->CR1 |= 0x1;     // Start TIM6
 TIM3->CR1 |= 0x1;    // Start TIM3
        EXTI->PR |= 0x1;      // Clear the EXTI0 interrupt flag
    }
} 

void TIM6_DAC_IRQHandler(void)
{
    if ((TIM6->SR & 0x1) != 0) // If update flag is set (UIF=1)
    {
 ADC1->SQR3 = i << 0; // Le canal i est configuré pour SQ1
        ADC1->CR2 |= (1 << 30); // Start ADC conversion
        TIM6->SR &= ~0x1; // Clear update interrupt flag
    }
}

void ADC_IRQHandler(void)
{
    if (ADC1->SR & 0x2) // Check if EOC flag is set
    {  
        adc_value[i-4] = ADC1->DR;   // Read ADC data register
        i++;
        if (i > 6)  // After 3 conversions
        {
            sprintf(buffer, "ch1=%d ch2=%d ch3=%d\r\n", adc_value[0], adc_value[1], adc_value[2]); // Format string
            USART2_SendString(buffer);
            i = 4; // Reset counter
        }
        ADC1->SR &= ~(1 << 1); // Clear EOC flag
    }
}
int main(void)
{  
    config_EXTI();
    config_TIMER6();
    config_ADC();
    USART2_Init();                        // Initialize USART2
    TIM3_PWM_Config();
    configure_interrupt_priorities();
    while (1){
}
return 0;
}
