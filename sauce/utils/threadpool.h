#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "core.h"

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  static Result create(ThreadPool *&thread_pool);
  static void destroy(ThreadPool *&thread_pool);

  template <class T> auto enqueue(T task) -> std::future<decltype(task())> {
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
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex eventMutex;
  std::condition_variable eventVar;
  bool stopping = false;

  void start(size_t numThreads);

  void stop() noexcept;
};

#endif // THREADPOOL_H_
