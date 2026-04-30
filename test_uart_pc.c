#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <conio.h>  // Pour _kbhit() et _getch() sur Windows

// ═══════════════════════════════════════════════════════════════
// SIMULATION DU RING BUFFER (même code que sur STM32)
// ═══════════════════════════════════════════════════════════════

#define UART_BUFFER_SIZE 64

typedef struct {
    volatile char buffer[UART_BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer_t;

static RingBuffer_t rx_ring = { .head = 0, .tail = 0 };

// Simule l'interruption : ajoute un caractère dans le buffer
void simulate_UART_receive(char c) {
    uint8_t next_head = (rx_ring.head + 1) % UART_BUFFER_SIZE;
    if (next_head != rx_ring.tail) {
        rx_ring.buffer[rx_ring.head] = c;
        rx_ring.head = next_head;
    }
}

// Même fonction que sur STM32
bool UART1_GetChar(char *c) {
    if (rx_ring.head == rx_ring.tail) {
        return false;
    }
    *c = rx_ring.buffer[rx_ring.tail];
    rx_ring.tail = (rx_ring.tail + 1) % UART_BUFFER_SIZE;
    return true;
}

// ═══════════════════════════════════════════════════════════════
// MÊME CODE DE COMMANDE QUE STM32
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
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        cmd_buf.buffer[cmd_buf.index++] = c;
    }
    return false;
}

void Command_Execute(void) {
    if (!cmd_buf.ready) return;
    
    printf("\n> Recu: %s\n", cmd_buf.buffer);
    
    if (strcmp(cmd_buf.buffer, "LED ON") == 0) {
        printf("OK: LED ALLUMEE\n");
    }
    else if (strcmp(cmd_buf.buffer, "LED OFF") == 0) {
        printf("OK: LED ETEINTE\n");
    }
    else if (strcmp(cmd_buf.buffer, "STATUS") == 0) {
        printf("ETAT: LED ETEINTE (simulation)\n");
    }
    else if (strcmp(cmd_buf.buffer, "HELP") == 0) {
        printf("Commandes: LED ON, LED OFF, STATUS, HELP\n");
    }
    else {
        printf("ERREUR: Commande inconnue\n");
    }
    
    cmd_buf.index = 0;
    cmd_buf.ready = false;
}

// ═══════════════════════════════════════════════════════════════
// MAIN - Simulation de l'UART avec le clavier
// ═══════════════════════════════════════════════════════════════

int main(void) {
    printf("=== SIMULATION STM32 UART ===\n");
    printf("Tape tes commandes (LED ON, LED OFF, STATUS, HELP)\n");
    printf("Appuie sur ENTREE pour valider\n");
    printf("Appuie sur 'q' pour quitter\n\n> ");
    
    char c;
    
    while (1) {
        // ─── SIMULATION DE L'INTERRUPTION UART ───
        // Sur PC : on lit le clavier et on simule "réception UART"
        if (_kbhit()) {  // Si touche pressée (comme interruption)
            c = _getch();  // Lit la touche (sans afficher)
            
            if (c == 'q') break;  // Quitter
            
            // Simule l'arrivée d'un caractère par UART
            simulate_UART_receive(c);
            
            // Echo (affiche ce que tu tapes)
            if (c != '\r') {
                putchar(c);
            }
        }
        
        // ─── MÊME CODE QUE SUR STM32 ───
        while (UART1_GetChar(&c)) {
            if (Command_Build(c)) {
                printf("\n");  // Saut de ligne après commande
            }
        }
        
        if (cmd_buf.ready) {
            Command_Execute();
            printf("> ");
        }
    }
    
    printf("\nAu revoir!\n");
    return 0;
}