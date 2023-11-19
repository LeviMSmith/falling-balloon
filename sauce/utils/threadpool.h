#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "core.h"

#include <future>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

// A lot of new things for me here. I first went for a namespace
// to keep things organized, and for this global pool, simply and stateful.
// I learned though, that doesn't really work as well for template functions
// since they really have to be implemented in the header. Instead, to
// still have private functions and allow me to implment some of the class
// in it's own translation unit, I went for a full static class.

class ThreadPool {
public:
  static Result start(size_t num_threads = NUM_THREADS);
  static void stop() noexcept;

  template <class T> 
  static auto enqueue(T task) -> std::future<decltype(task())> {
    auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(
        std::move(task));
    {
      std::unique_lock<std::mutex> lock{eventMutex};
      tasks.emplace([=] { (*wrapper)(); });
    }
    eventVar.notify_one();
    return wrapper->get_future();
  }

private:
  static constexpr size_t NUM_THREADS = 3;

  static std::vector<std::thread> workers;
  static std::queue<std::function<void()>> tasks;

  static std::mutex eventMutex;
  static std::condition_variable eventVar;
  static bool stopping;
};

#endif // THREADPOOL_H_
