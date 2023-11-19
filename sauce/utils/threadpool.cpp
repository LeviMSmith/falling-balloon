#include "core.h"
#include "utils/threadpool.h"

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

constexpr u8 NUM_THREADS = 3;

Result ThreadPool::create(ThreadPool*& thread_pool) {
  thread_pool = new ThreadPool;

  thread_pool->start(NUM_THREADS);

  return Result::SUCCESS;
}

void ThreadPool::destroy(ThreadPool*& thread_pool) {
  if (thread_pool != nullptr) {
    thread_pool->stop();
    delete thread_pool;
  }
}

void ThreadPool::start(size_t num_threads) {
  for (size_t i = 0; i < num_threads; i++) {
    workers.emplace_back([=] {
      while (true) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock{eventMutex};
          eventVar.wait(lock, [=] { return stopping || !tasks.empty(); });
          if (stopping && tasks.empty()) {
            break;
          }

          task = std::move(tasks.front());
          tasks.pop();
        }

        task();
      }
    });
  }
}

void ThreadPool::stop() noexcept {
  {
    std::unique_lock<std::mutex> lock{eventMutex};
    stopping = true;
  }

  eventVar.notify_all();

  for (auto &thread : workers) {
    thread.join();
  }
}
