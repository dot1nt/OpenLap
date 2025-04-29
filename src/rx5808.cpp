#include "rx5808.h"

#include <Arduino.h>
#include <SPI.h>

Rx5808::Rx5808(uint8_t inputPin, uint8_t selPin) : m_selPin(selPin), m_inputPin(inputPin) {}

void Rx5808::init() {
  pinMode(m_selPin, OUTPUT);

  SPI.begin();
  SPI.setBitOrder(LSBFIRST);

  digitalWrite(m_selPin, LOW);
  SPI.transfer(0x10);
  SPI.transfer(0x01);
  SPI.transfer(0x00);
  SPI.transfer(0x00);
  digitalWrite(m_selPin, HIGH);
}

void Rx5808::setFrequency(uint16_t freq) {
  uint32_t d = (freq - 479) / 2;

  uint8_t dh = d >> 5;
  uint8_t dl = d & 0x1F;

  uint8_t d0 = (dl << 5) | 0x11;
  uint8_t d1 = (dh << 4) | (dl >> 3);
  uint8_t d2 = dh >> 4;
  uint8_t d3 = 0x00;

  digitalWrite(m_selPin, LOW);
  SPI.transfer(d0);
  SPI.transfer(d1);
  SPI.transfer(d2);
  SPI.transfer(d3);
  digitalWrite(m_selPin, HIGH);
}

int Rx5808::readRssi() {
  return analogRead(m_inputPin);
} 