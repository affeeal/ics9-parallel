#include "utils.hpp"

#include <cassert>

#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include <fmt/format.h>

namespace utils {

using namespace std::chrono_literals;

namespace {

const auto kMainThreadSleepTime = 30s;

const auto kActionPerformingMin = 0;
const auto kActionPerformingMax = 5;
const auto kRightForkWaitingMin = 6;
const auto kRightForkWaitingMax = 10;

void Cout(std::mutex &cout_mutex, std::string_view message) {
  std::lock_guard<std::mutex> cout_lock{cout_mutex};
  std::cout << message;
}

} // namespace

void Perform(const int count) {
  assert(count >= kPhilosophersMinCount);

  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> action_performing(kActionPerformingMin,
                                                       kActionPerformingMax);
  std::uniform_int_distribution<int> right_fork_waiting(kRightForkWaitingMin,
                                                        kRightForkWaitingMax);
  std::mutex cout_mutex;

  std::vector<std::timed_mutex> forks(count);

  std::vector<std::thread> threads;
  threads.reserve(count);

  bool should_countinue = true;

  for (auto i = 0; i < count; i++) {
    threads.push_back(std::thread{[&, i]() {
      const auto left_fork = i;
      const auto right_fork = (i + 1 == count ? 0 : i + 1);

      while (should_countinue) {
        const auto thinking_time =
            std::chrono::duration<int>(action_performing(generator));
        Cout(cout_mutex, fmt::format("Philosopher {} thinks for {} seconds\n",
                                     i, thinking_time.count()));
        std::this_thread::sleep_for(thinking_time);

        Cout(cout_mutex,
             fmt::format("Philosopher {} tries to take left fork {}\n", i,
                         left_fork));
        std::unique_lock<std::timed_mutex> left_fork_mutex{forks[left_fork]};

        const auto right_fork_waiting_time =
            std::chrono::duration<int>(right_fork_waiting(generator));
        Cout(cout_mutex,
             fmt::format(
                 "Philosopher {} tries to take right fork {} for {} seconds\n",
                 i, right_fork, right_fork_waiting_time.count()));
        std::unique_lock<std::timed_mutex> right_fork_mutex{
            forks[right_fork], right_fork_waiting_time};

        if (!right_fork_mutex.owns_lock()) {
          Cout(cout_mutex,
               fmt::format("Philosopher {} releases left fork {} because he "
                           "couldn't take right fork {}\n",
                           i, left_fork, right_fork));
          left_fork_mutex.unlock();
          continue;
        }

        const auto meal_time =
            std::chrono::duration<int>(action_performing(generator));
        Cout(cout_mutex, fmt::format("Philosopher {} eats for {} seconds\n", i,
                                     meal_time.count()));
        std::this_thread::sleep_for(meal_time);

        Cout(cout_mutex,
             fmt::format("Philosopher {} releases forks {} and {}\n", i,
                         left_fork, right_fork));
      }
    }});
  }

  std::this_thread::sleep_for(kMainThreadSleepTime);

  Cout(cout_mutex, "Flag should_countinue is set to false\n");
  should_countinue = false;

  for (auto &t : threads) {
    t.join();
  }
}

} // namespace utils
