#ifndef _PROJECT_HEADER_H
#define _PROJECT_HEADER_H

#define TIME_OUT 120000

// #define DCAR_RELAYS
#define DCAR_MOTORS


#define SSerialRX 0 //Serial Receive pin
#define SSerialTX 1 //Serial Transmit pin
#define RS485Transmit HIGH
#define RS485Receive LOW
#define SSerialTxControl 2

#define Pin13LED 13

#ifdef	DCAR_RELAYS
	#define RELAY_M1 8
	#define M1_PWM 5

	#define RELAY_M2 10
	#define M2_PWM 6

	#define RELAY_M3 7
	#define M3_PWM 9
#endif

#ifdef DCAR_MOTORS
	#define RELAY_1_M1 10
	#define RELAY_2_M1 11
	#define M1_PWM 5

	#define RELAY_1_M2 7
	#define RELAY_2_M2 8
	#define M2_PWM 6

	#define RELAY_1_M3 3
	#define RELAY_2_M3 4
	#define M3_PWM 9
#endif

#define SW1 A5
#define SW2 A4
#define SW3 A3
#define SW4 A2
#define SW5 A1

#define TIMER_INTERRUPT_PERIOD 100000 //microsecond

typedef unsigned char uint8_t;

typedef unsigned char tBYTE;
typedef unsigned int tWORD;

#endif
