#include "utils.hpp"

#include <cassert>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

namespace utils {

using namespace std::chrono_literals;

static const auto kMainThreadSleepTime = 30s;
static const auto kPhilosophersWaitSecondsMin = 0;
static const auto kPhilosophersWaitSecondsMax = 5;

void Perform(const int count) {
  assert(count >= kPhilosophersMinCount);

  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> distribution(kPhilosophersWaitSecondsMin,
                                                  kPhilosophersWaitSecondsMax);

  std::vector<std::mutex> forks(count);

  std::vector<std::thread> threads;
  threads.reserve(count);

  bool should_countinue = true;

  for (auto i = 0; i < count; i++) {
    threads.push_back(std::thread{[&, i]() {
      const auto left_fork = i;
      const auto right_fork = (i + 1 == count ? 0 : i + 1);

      while (should_countinue) {
        const auto thinking_time =
            std::chrono::duration<int>(distribution(generator));
        std::cout << "Philosopher " << i << " thinks for "
                  << thinking_time.count() << '\n';
        std::this_thread::sleep_for(thinking_time);

        std::cout << "Philosopher " << i << " tries to take left fork "
                  << left_fork << '\n';
        std::lock_guard<std::mutex> left_fork_mutex{forks[left_fork]};

        std::cout << "Philosopher " << i << " tries to take right fork "
                  << right_fork << '\n';
        std::lock_guard<std::mutex> right_fork_mutex{forks[right_fork]};

        const auto meal_time =
            std::chrono::duration<int>(distribution(generator));
        std::cout << "Philosopher " << i << " eats for " << meal_time.count()
                  << '\n';
        std::this_thread::sleep_for(meal_time);

        std::cout << "Philosopher " << i << " releases forks " << left_fork
                  << " and " << right_fork << '\n';
      }
    }});
  }

  std::this_thread::sleep_for(kMainThreadSleepTime);

  std::cout << "should_countinue is false now\n";
  should_countinue = false;

  for (auto &t : threads) {
    t.join();
  }
}

} // namespace utils
