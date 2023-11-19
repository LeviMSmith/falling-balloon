#include "core.h"
#include "utils/threadpool.h"

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

std::vector<std::thread> ThreadPool::workers;
std::queue<std::function<void()>> ThreadPool::tasks;
std::mutex ThreadPool::eventMutex;
std::condition_variable ThreadPool::eventVar;
bool ThreadPool::stopping = false;

Result ThreadPool::start(size_t num_threads) {
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

  return Result::SUCCESS;
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
