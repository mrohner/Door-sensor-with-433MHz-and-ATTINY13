/*
ATTINY13A Pins
--------------
PB0: reed switch      over 10kOhm to GND
PB1: same reed switch resp. connected to PB0
PB2: 433Mhz sender
PB4: TAMPERing switch over 10kOhm to GND

Markus Rohner 13.7.2018

*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "PinChangeInterrupt.h"

volatile int message = 0;

const int SPEED = 350/8; //350nominal  / 8 prescaler (processor runs at 1.2 MHz, uses less power this way)
const byte START = 31;
const byte _SHORT = 1;
const byte _LONG = 3;
// The code you may change per device
const byte OPEN =  0b00000001;//Decimal: 131586 (24Bit) Binary: 000000100000001000000010
const byte CLOSE = 0b00000010;//Decimal: 65793 (24Bit) Binary: 000000010000000100000001
const byte TAMPER = 0b00000011;//Decimal: 197379 (24Bit) Binary: 000000110000001100000011

#define TRANS_PIN PB2
#define INPUT_PIN0 PB0
#define INPUT_PIN1 PB1
#define TAMPER_PIN PB4

void setup(){
  // Configure TRANS_PIN as output
  DDRB |= (1 << TRANS_PIN);
  // Set INPUT_PINs as Input
  DDRB &= ~(1 << INPUT_PIN0 | 1 << INPUT_PIN1 | 1 << TAMPER_PIN);
  //pinMode(INPUT_PIN0, INPUT);//INPUT_PULLUP
  //pinMode(INPUT_PIN1, INPUT);
  MCUCR = (1 << SM1);   // define power down sleep mode
  // attach the new PinChangeInterrupts and enable event functions below
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(INPUT_PIN0), tick, RISING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(INPUT_PIN1), tock, FALLING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(TAMPER_PIN), tamper, RISING);
  enable_PinChange_interrupts ();
}


void loop() {
  // change into sleep mode
  // Set TRANS_PIN low
  PORTB &= ~(1 << TRANS_PIN);
  ADCSRA = 0 << ADEN;   // Power reduction ADC
  sleep_mode(); 
  ADCSRA = 1 << ADEN;   // Power reduction ADC reverse
  disable_PinChange_interrupts ();
  switch (message) {
    case 1:
      send(OPEN); 
    break;
    case 2:
      send(CLOSE);
    break;
    case 3:
      send(TAMPER);
    break;
  }
  message = 0;
  _delay_ms(50);
  enable_PinChange_interrupts ();  
}

void tick(void) {
  message = 1;
}

void tock(void) {
  message = 2;
}

void tamper(void) {
  message = 3;
}

void enable_PinChange_interrupts () {
  enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(INPUT_PIN0));
  enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(INPUT_PIN1));
  enablePinChangeInterrupt(digitalPinToPinChangeInterrupt(TAMPER_PIN));
}


void disable_PinChange_interrupts () {
  disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(INPUT_PIN0));
  disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(INPUT_PIN1));
  disablePinChangeInterrupt(digitalPinToPinChangeInterrupt(TAMPER_PIN));
}


void send(byte id){
  for(byte i=0;i<13;i++) {
    sendStart();
    sendUniqueId(id);
  }
}


void sendStart() {
  digitalWrite(1);
  _delay_us((SPEED*_SHORT));
  digitalWrite(0);
  _delay_us((SPEED*START));
}


void sendUniqueId(byte id) {
  for(byte x=0;x<3;x++) {//  send 3 times same 8 bits = 24 bit code
    for (unsigned int y = 8; y-- > 0;){ 
      if(bitRead(id,y)) sendPulse1();
      else sendPulse0();
    }
  }
}


void sendPulse0() {
  digitalWrite(1);
  _delay_us((SPEED*_SHORT));
  digitalWrite(0);
  _delay_us((SPEED*_LONG));
}


void sendPulse1() {
  digitalWrite(1);
  _delay_us((SPEED*_LONG));
  digitalWrite(0);
  _delay_us((SPEED*_SHORT));
}


void digitalWrite(bool val){
  if(val)PORTB |= (1 << TRANS_PIN);
  else PORTB &= ~(1 << TRANS_PIN);
}
