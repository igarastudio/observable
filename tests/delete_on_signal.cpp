// Observable Library
// Copyright (c) 2026-present David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "obs/signal.h"
#include "test.h"

int counter = 0;

class A {
public:
  explicit A(obs::signal<void()>& sig) {
    m_conn = sig.connect(&A::on_signal, this);
  }
  void next(A* next) {
    m_next = next;
  }
private:
  virtual void on_signal() {
    ++counter;
    delete m_next;
  }
  obs::scoped_connection m_conn;
  A* m_next = nullptr;
};

int main() {
  obs::signal<void()> signal;
  {
    A a(signal);
    a.next(new A(signal));
    signal();
  }
  signal();

  EXPECT_EQ(1, counter);
}
