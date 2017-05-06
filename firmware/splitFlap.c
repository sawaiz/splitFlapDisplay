// splitFlap.c
// Sawaiz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Motor and Sensor
#define motorPinA PINB3
#define motorPinB PINB4
#define hallSensorPin PINB0
// Data Transfer
#define dataInPin PINB0
#define dataOutPin PINB2
#define clockPin PINB1

// All pins are on Port B
#define motorPortA PORTB
#define motorPortB PORTB
#define hallSensorPort PORTB
#define dataInPort PORTB
#define dataOutPort PORTB
#define clockPort PORTB

// All pins are on Port B
#define motorDirPortA DDRB
#define motorDirPortB DDRB
#define hallSensorDirPort DDRB
#define dataInDirPort DDRB
#define dataOutDirPort DDRB
#define clockDirPort DDRB

//inputs are on PINB
#define hallSensorPinAddress PINB
#define dataInPinAddress PINB0

// Prototypes
void stepForward(int steps);
void enableInterupt();
void startTimer();
void home();

// Globals
volatile uint8_t motorPosition = 0;
volatile uint8_t charBuffer = 0;
volatile uint8_t currentChar = 0;

ISR(INT0_vect) {
  // Clock has been triggered
  // Bit shift buffer
  charBuffer <<= 1;
  // Zero gets shifted in, if the input is high, set it to 1
  if ((dataInPort >> dataInPin) & 1) {
    charBuffer |= 1;
  }
  // Set output to highest Bit
  if ((charBuffer >> 7) & 1) {
    dataOutPort |= (1 << dataOutPin);
  } else {
    dataOutPort &= ~(1 << dataOutPin);
  }
  // Reset timeout timer
  TCNT0 = 0;
}

// If the timer overflows, and data has changed, set the char
ISR(TIM0_OVF_vect) {
  if (charBuffer != currentChar) {
    // Move forward to set the char
    stepForward(20);
    // Set the current char
    currentChar = charBuffer;
  }
}

// Stepper motor
void stepForward(int steps) {
  int i = 0;
  for (i=0 ; i < steps; i++){
    if(motorPosition == 0 ){
      motorPortA &= ~(1 << motorPinA);
      motorPortB |= (1 << motorPinB);
      motorPosition++;
    } else if (motorPosition == 1){
      motorPortA |= (1 << motorPinA);
      motorPortB |= (1 << motorPinB);
      motorPosition++;
    } else if (motorPosition == 2){
      motorPortA |= (1 << motorPinA);
      motorPortB &= ~(1 << motorPinB);
      motorPosition++;      
    } else if (motorPosition == 3){
      motorPortA &= ~(1 << motorPinA);
      motorPortB &= ~(1 << motorPinB);
      motorPosition = 0;
    }
    _delay_ms(5);
  }
}

// Enable clock interrupt
void enableInterupt() {
  // Enable Global Interrupts
  sei();
  // MCU Control Register  // Low  Change  Fall Rise
  MCUCR &= ~(1 << ISC00);  // 0    1       0    1
  MCUCR |= 1 << ISC01;     // 0    0       1    1
  // GIMSK – General Interrupt Mask Register
  GIMSK |= 1 << INT0;
}

// Start timer
void startTimer() {
  // Connect timer to system clk  
  // Sets the timeout, ex: 1MHz clk, 1us * 256 (8 bit timer) * 8 (prescalar)
  // Enable prescalar     // stop clk clk/8  clk/64 clk/256 clk/1024
  TCCR0B &= ~(1 << CS00); // 0    1   0      1       0      1
  TCCR0B &= ~(1 << CS01); // 0    0   1      1       0      0
  TCCR0B |= (1 << CS02);  // 0    0   0      0       1      1
  // Enable Global Interrupts
  sei();
  // Enable timer overflow interupt
  TIMSK0 |= (1 << TOIE0);
}

// Home the wheel
void home() {
  // If the pin is high, try again
  while (hallSensorPinAddress & (1 << hallSensorPin)) {
    // If the hall sensor is not activated
    stepForward(1);
  }
  // Then move to the blank character
  stepForward(400);
}

int main() {
  // Motor and data out are output
  motorDirPortA |= 1 << motorPinA;
  motorDirPortB |= 1 << motorPinB;
  dataOutDirPort |= 1 << dataOutPin;
  // Hall sensor, clock, and dataIn inputs
  hallSensorDirPort &= ~(1 << hallSensorPin);
  clockDirPort &= ~(1 << clockPin);
  dataInDirPort &= ~(1 << dataInPin);

  // Home the wheel
  home();

  // Enable interrupts to start accepting data.
  enableInterupt();

  // Hang out and wait for interupts
  while (1) {
    // stepForward(400);
  }

  // Program should never make it here, so error out
  return 1;
}
