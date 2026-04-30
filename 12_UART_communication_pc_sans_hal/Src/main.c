#include "stm32f1xx.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "core_cm3.h"

// Redirige printf vers SWO
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(ptr[i]);
    }
    return len;
}
// ═══════════════════════════════════════════════════════════════
// PARTIE 1 : RING BUFFER (même code que simulation PC)
// ═══════════════════════════════════════════════════════════════

#define UART_BUFFER_SIZE 64

typedef struct {
    volatile char buffer[UART_BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer_t;

static RingBuffer_t rx_ring = { .head = 0, .tail = 0 };

bool UART1_GetChar(char *c) {
    if (rx_ring.head == rx_ring.tail) {
        return false;
    }
    *c = rx_ring.buffer[rx_ring.tail];
    rx_ring.tail = (rx_ring.tail + 1) % UART_BUFFER_SIZE;
    return true;
}

void UART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void UART1_SendString(const char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

// ═══════════════════════════════════════════════════════════════
// PARTIE 2 : INTERRUPTION (remplace la fonction .weak)
// ═══════════════════════════════════════════════════════════════

void USART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {
        char c = USART1->DR;
        uint8_t next_head = (rx_ring.head + 1) % UART_BUFFER_SIZE;
        if (next_head != rx_ring.tail) {
            rx_ring.buffer[rx_ring.head] = c;
            rx_ring.head = next_head;
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// PARTIE 3 : INIT UART
// ═══════════════════════════════════════════════════════════════

void UART1_Init(uint32_t baudrate) {
    // Horloges
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    // GPIO : PA9=TX, PA10=RX
    GPIOA->CRH &= ~(0xFF << 4);
    GPIOA->CRH |= (0x0B << 4) | (0x04 << 8);

    // Baudrate
    USART1->BRR = 8000000 / baudrate;

    // Activer TX, RX, UART
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    // ACTIVER INTERRUPTION RX
    USART1->CR1 |= USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART1_IRQn);
}

// ═══════════════════════════════════════════════════════════════
// PARTIE 4 : LED (PC13)
// ═══════════════════════════════════════════════════════════════

void LED_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x3 << 20);
    GPIOC->ODR |= (1 << 13);  // Éteint
}

void LED_On(void)  { GPIOC->ODR &= ~(1 << 13); }
void LED_Off(void) { GPIOC->ODR |=  (1 << 13); }

// ═══════════════════════════════════════════════════════════════
// PARTIE 5 : COMMANDES
// ═══════════════════════════════════════════════════════════════

#define CMD_BUFFER_SIZE 32

typedef struct {
    char buffer[CMD_BUFFER_SIZE];
    uint8_t index;
    bool ready;
} CommandBuffer_t;

static CommandBuffer_t cmd_buf = { .index = 0, .ready = false };

bool Command_Build(char c) {
    if (c == '\r' || c == '\n') {
        if (cmd_buf.index > 0) {
            cmd_buf.buffer[cmd_buf.index] = '\0';
            cmd_buf.ready = true;
            return true;
        }
        return false;
    }
    if (cmd_buf.index < CMD_BUFFER_SIZE - 1) {
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        cmd_buf.buffer[cmd_buf.index++] = c;
    }
    return false;
}

void Command_Execute(void) {
    if (!cmd_buf.ready) return;

    UART1_SendString("\r\n> Recu: ");
    UART1_SendString(cmd_buf.buffer);
    UART1_SendString("\r\n");

    if (strcmp(cmd_buf.buffer, "LED ON") == 0) {
        LED_On();
        UART1_SendString("OK: LED ALLUMEE\r\n");
    }
    else if (strcmp(cmd_buf.buffer, "LED OFF") == 0) {
        LED_Off();
        UART1_SendString("OK: LED ETEINTE\r\n");
    }
    else if (strcmp(cmd_buf.buffer, "STATUS") == 0) {
        UART1_SendString((GPIOC->ODR & (1<<13)) ? "ETAT: ETEINTE\r\n" : "ETAT: ALLUMEE\r\n");
    }
    else if (strcmp(cmd_buf.buffer, "HELP") == 0) {
        UART1_SendString("Commandes: LED ON, LED OFF, STATUS, HELP\r\n");
    }
    else {
        UART1_SendString("ERREUR: Commande inconnue\r\n");
    }

    cmd_buf.index = 0;
    cmd_buf.ready = false;
}

// ═══════════════════════════════════════════════════════════════
// MAIN
// ═══════════════════════════════════════════════════════════════

int main(void) {
    LED_Init();
    UART1_Init(9600);

    UART1_SendString("\r\n=== STM32 Pret ===\r\n> ");

    char c;
    while (1) {
        // Lis les caractères reçus par interruption
        while (UART1_GetChar(&c)) {
            UART1_SendChar(c);  // Echo
            if (Command_Build(c)) {
                // Rien, juste pour le saut de ligne
            }
        }

        // Exécute si commande prête
        if (cmd_buf.ready) {
            Command_Execute();
            UART1_SendString("> ");
        }
    }
    //printf("LED ON\n");
}
