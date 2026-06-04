// Observable Library
// Copyright (c) 2026-present David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef OBS_FAST_LIST_H_INCLUDED
#define OBS_FAST_LIST_H_INCLUDED
#pragma once

#include <algorithm>
#include <atomic>
#include <cassert>
#include <functional>
#include <vector>

namespace obs {

template<typename T>
class fast_list {
  std::vector<T*> m_list;
  std::atomic<unsigned int> m_iterating = { 0 };

public:
  using iterator = typename std::vector<T*>::iterator;

  fast_list() = default;
  ~fast_list() = default;

  bool empty() const { return m_list.empty(); }
  iterator begin() { return m_list.begin(); }
  iterator end() { return m_list.end(); }

  void start_iteration() {
    ++m_iterating;
  }

  void end_iteration() {
    assert(m_iterating > 0);
    --m_iterating;

    // Clean up deleted items when we were iterating
    if (m_iterating == 0) {
      for (auto it = m_list.begin(); it != m_list.end(); ) {
        if (*it == nullptr)
          it = m_list.erase(it);
        else
          ++it;
      }
    }
  }

  void push_back(T* value) {
    m_list.push_back(value);
  }

  void erase(T* value) {
    auto it = std::find(m_list.begin(), m_list.end(), value);
    if (it != m_list.end()) {
      if (m_iterating > 0)
        *it = nullptr;
      else
        m_list.erase(it);
    }
  }
};

} // namespace obs

#endif
