#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "core.h"

#include <future>

// Don't usually like stateful things, but it makes sense for a
// global thread pool

namespace ThreadPool {
  constexpr size_t NUM_THREADS = 3;

  Result start(size_t num_threads = NUM_THREADS);
  void stop() noexcept;
  template <class T> auto enqueue(T task) -> std::future<decltype(task())>;
};

#endif // THREADPOOL_H_
