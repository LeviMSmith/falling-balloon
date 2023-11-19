#include "core.h"
#include "utils/threadpool.h"

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>

struct ThreadPoolState {
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex eventMutex;
  std::condition_variable eventVar;
  bool stopping = false;
};

std::unique_ptr<ThreadPoolState> thread_pool_state = std::make_unique<ThreadPoolState>();

Result ThreadPool::start(size_t num_threads) {
  for (size_t i = 0; i < num_threads; i++) {
    thread_pool_state->workers.emplace_back([=] {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock{thread_pool_state->eventMutex};
          thread_pool_state->eventVar.wait(lock, [=] { return thread_pool_state->stopping || !thread_pool_state->tasks.empty(); });
          if (thread_pool_state->stopping && thread_pool_state->tasks.empty()) {
            break;
          }

          task = std::move(thread_pool_state->tasks.front());
          thread_pool_state->tasks.pop();
        }

        task();
      }
    });
  }

  return Result::SUCCESS;
}

void ThreadPool::stop() noexcept {
  {
    std::unique_lock<std::mutex> lock{thread_pool_state->eventMutex};
    thread_pool_state->stopping = true;
  }

  thread_pool_state->eventVar.notify_all();

  for (auto &thread : thread_pool_state->workers) {
    thread.join();
  }
}

template <class T> auto enqueue(T task) -> std::future<decltype(task())> {
  auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(
      std::move(task));
  {
    std::unique_lock<std::mutex> lock{thread_pool_state->eventMutex};
    thread_pool_state->tasks.emplace([=] { (*wrapper)(); });
  }
  thread_pool_state->eventVar.notify_one();
  return wrapper->get_future();
}
