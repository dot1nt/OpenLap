#include "filter.h"

#include <Arduino.h>

Filter::Filter(size_t size) : m_size(size) {
  m_buffer = new int[size]();
}

Filter::~Filter() {
  delete[] m_buffer;
}

int Filter::process(int newData) {
  m_sum -= m_buffer[m_index];
  m_buffer[m_index] = newData;
  m_sum += newData;
  m_index = (m_index + 1) % m_size;
  if (m_count < m_size)
    m_count++;

  return m_sum / m_count;
}