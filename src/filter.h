#pragma once

#include <Arduino.h>

class Filter {
 public:
  Filter(size_t size);
  ~Filter();
  int process(int newData);

 private:
  int *m_buffer;
  size_t m_index;
  size_t m_count;
  size_t m_size;
  int m_sum;
};