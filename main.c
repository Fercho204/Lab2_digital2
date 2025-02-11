//Digital 2
//Laboratorio 2
//Fernando Palomo - 22357

#define F_CPU 16000000UL  // Definir la frecuencia del CPU para las funciones de delay
#include <util/delay.h>   // Librería para funciones de delay
#include <avr/io.h>       // Librería para manejo de registros del microcontrolador
#include <stdlib.h>       // Librería estándar para funciones como dtostrf
#include "LCD/LCD.h"      // Incluir la librería de la LCD
#include "ADC/ADC.h"      // Incluir la librería del ADC
#include "UART/UART.h"    // Incluir la librería del UART

char str[10];             // Buffer para almacenar cadenas de caracteres
uint16_t dato_adc1, dato_adc2, contador;  // Variables para almacenar valores del ADC y el contador

// Función para convertir un número a ASCII
void int_to_ascii(uint16_t value, char *buffer) {
	buffer[0] = (value / 100) + '0';  // Obtener centenas y convertir a ASCII
	buffer[1] = ((value / 10) % 10) + '0';  // Obtener decenas y convertir a ASCII
	buffer[2] = (value % 10) + '0';  // Obtener unidades y convertir a ASCII
	buffer[3] = '\0';  // Terminar la cadena con un carácter nulo
}

// Función para actualizar la LCD con los valores de los potenciómetros y el contador
void update_LCD(uint16_t pot1, uint16_t pot2, int16_t counter) {
	char buffer[10];  // Buffer para almacenar cadenas de caracteres

	// Convertir el valor del potenciómetro 1 a voltaje y mostrarlo en la LCD
	dtostrf((5.0 / 1023) * pot1, 1, 2, buffer);  // Convertir a cadena con 2 decimales
	LCD_Set_Cursor(1, 2);  // Posicionar el cursor en la fila 1, columna 2
	LCD_Write_String(buffer);  // Mostrar el valor en la LCD
	LCD_Write_String("V");  // Mostrar la unidad "V" (voltios)

	// Convertir el valor del potenciómetro 2 a voltaje y mostrarlo en la LCD
	dtostrf((5.0 / 1023) * pot2, 1, 2, buffer);  // Convertir a cadena con 2 decimales
	LCD_Set_Cursor(7, 2);  // Posicionar el cursor en la fila 1, columna 7
	LCD_Write_String(buffer);  // Mostrar el valor en la LCD
	LCD_Write_String("V");  // Mostrar la unidad "V" (voltios)

	// Mostrar el valor del contador en la LCD
	LCD_Set_Cursor(13, 2);  // Posicionar el cursor en la fila 1, columna 13
	int_to_ascii(counter, buffer);  // Convertir el contador a ASCII
	LCD_Write_String(buffer);  // Mostrar el valor en la LCD
}

int main(void) {
	ADC_init();  // Inicializar el ADC
	initLCD8bits();  // Inicializar la LCD en modo de 8 bits
	UART_Init(9600);  // Inicializar el UART con un baud rate de 9600
	LCD_Set_Cursor(1, 1);  // Posicionar el cursor en la fila 1, columna 1
	LCD_Write_String("S1:   S2:   CONT");  // Mostrar el encabezado en la LCD

	while (1) {
		// Mostrar el menú en la terminal UART
		UART_Transmit_String("\n \r");
		UART_Transmit_String("Seleccione una opcion: \n \r");
		UART_Transmit_String("1. Leer potenciometro \n \r");
		UART_Transmit_String("2. Modificar contador (+ o -) \n \r");

		char option = UART_Receive();  // Leer la opción seleccionada por el usuario

		switch (option) {
			case '1':  // Opción 1: Leer valores del potenciómetro
			dato_adc1 = ADC_read(1);  // Leer el valor del potenciómetro 1
			dato_adc2 = ADC_read(2);  // Leer el valor del potenciómetro 2
			update_LCD(dato_adc1, dato_adc2, contador);  // Actualizar la LCD

			// Enviar los valores de los potenciómetros a la terminal UART
			UART_Transmit_String("Pot1: ");
			dtostrf((5.0 / 1023) * dato_adc1, 1, 2, str);  // Convertir a cadena
			UART_Transmit_String(str);  // Enviar el valor
			UART_Transmit_String("V");  // Enviar la unidad "V"

			UART_Transmit_String(", Pot2: ");
			dtostrf((5.0 / 1023) * dato_adc2, 1, 2, str);  // Convertir a cadena
			UART_Transmit_String(str);  // Enviar el valor
			UART_Transmit_String("V");
			UART_Transmit_String("\n \r");
			break;

			case '2':  // Opción 2: Modificar el contador
			UART_Transmit_String("Ingrese + o - para modificar el contador:\n \r");
			char command = UART_Receive();  // Leer el comando del usuario
			UART_ProcessCommand(command);  // Procesar el comando (incrementar o decrementar)
			update_LCD(dato_adc1, dato_adc2, contador);  // Actualizar la LCD

			dtostrf(contador, 1, 0, str);  // Convertir el contador a cadena
			UART_Transmit_String("\n \r");
			break;

			default:  // Opción inválida
			UART_Transmit_String("Opcion invalida.\n \r");
			break;
		}
	}
}
