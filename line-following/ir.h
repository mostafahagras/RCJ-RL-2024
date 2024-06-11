#ifndef IR_H
#define IR_H
#include <avr/io.h>

// Explanation: https://youtu.be/UhTRrjYXqPU?si=KGEqJ7RZYB6U8LjE

#define _IR0()  ((PINB & _BV(PB0)) ? 1 : 0)  // PB0 corresponds to digital pin 53
#define _IR1()  ((PINB & _BV(PB2)) ? 1 : 0)  // PB2 corresponds to digital pin 51
#define _IR2()  ((PINL & _BV(PL0)) ? 1 : 0)  // PL0 corresponds to digital pin 49
#define _IR3()  ((PINL & _BV(PL2)) ? 1 : 0)  // PL2 corresponds to digital pin 47
#define _IR4()  ((PINL & _BV(PL4)) ? 1 : 0)  // PL4 corresponds to digital pin 45
#define _IR6()  ((PINL & _BV(PL6)) ? 1 : 0)  // PL6 corresponds to digital pin 43
#define _IR7()  ((PING & _BV(PG0)) ? 1 : 0)  // PG0 corresponds to digital pin 41
#define _IR8()  ((PING & _BV(PG2)) ? 1 : 0)  // PG2 corresponds to digital pin 39
#define _IR9()  ((PINC & _BV(PC0)) ? 1 : 0)  // PC0 corresponds to digital pin 37
#define _IR10() ((PINC & _BV(PC2)) ? 1 : 0)  // PC2 corresponds to digital pin 35

#define SILVER_1()  ((PIND & _BV(PD2)) ? 0 : 1) // PD2 corresponds to digital pin 2
#define SILVER_2()  ((PIND & _BV(PD3)) ? 0 : 1) // PD3 corresponds to digital pin 3
#define SILVER_3()  ((PING & _BV(PG5)) ? 0 : 1) // PD3 corresponds to digital pin 4

#endif