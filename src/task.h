#pragma once

#include <Arduino.h>

template <class T>
class Task {
 public:
  Task(const char* name, uint32_t stackDepth, UBaseType_t priority) : m_name(name), m_stackDepth(stackDepth), m_priority(priority) {}
  virtual ~Task(){};

  void begin() {
    BaseType_t res = xTaskCreate(taskFunction, m_name, m_stackDepth, this, m_priority, nullptr);
    assert(res == pdPASS);
  }

  virtual void run() = 0;

 private:
  static void taskFunction(void* params) {
    T* t = static_cast<T*>(params);
    t->run();
  }

  const char* m_name;
  uint32_t m_stackDepth;
  UBaseType_t m_priority;
};