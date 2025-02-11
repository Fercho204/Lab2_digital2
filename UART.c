#include "UART.h"
uint16_t contador = 0;  // Inicialización del contador

// Función para inicializar el UART
void UART_Init(uint16_t ubrr) {
	DDRD |= (1 << DDD1);  // Configurar el pin TX (PD1) como salida
	DDRD &= ~(1 << DDD0);  // Configurar el pin RX (PD0) como entrada
	UCSR0A = 0;  // Limpiar el registro de estado
	UCSR0B = 0;  // Limpiar el registro de control
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);  // Habilitar interrupción de recepción, RX y TX
	UCSR0C = 0;  // Limpiar el registro de configuración
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // Configurar el formato del frame: 8 bits de datos, 1 bit de parada
	UBRR0 = 103;  // Configurar el baud rate para 9600 (UBRR = 103 para 16 MHz)
}

// Función para transmitir un carácter por UART
void UART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));  // Esperar a que el buffer de transmisión esté vacío
	UDR0 = data;  // Colocar el dato en el buffer de transmisión
}

// Función para recibir un carácter por UART
char UART_Receive(void) {
	while (!(UCSR0A & (1 << RXC0)));  // Esperar a que se reciba un dato
	return UDR0;  // Leer el dato recibido
}

// Función para transmitir una cadena de caracteres por UART
void UART_Transmit_String(char *str) {
	while (*str) {  // Recorrer la cadena hasta el carácter nulo
		UART_Transmit(*str++);  // Transmitir cada carácter
	}
}

// Función para procesar comandos recibidos por UART
void UART_ProcessCommand(char command) {
	if (command == '+') {
		contador++;  // Incrementar el contador
		} else if (command == '-') {
		contador--;  // Decrementar el contador
	}
	char buffer[10];
	itoa(contador, buffer, 10);  // Convertir el contador a cadena
	UART_Transmit_String("Contador: ");  // Enviar mensaje a la terminal
	UART_Transmit_String(buffer);  // Enviar el valor del contador
}
