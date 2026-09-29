#include "timer.hpp"
#include <chrono>
#include <string>

using namespace std::chrono;

struct TimeDetails {
  uint64_t days;
  uint64_t hours;
  uint64_t minutes;
  uint64_t seconds;
  uint64_t milliseconds;
  uint64_t remaining_microseconds;
};

steady_clock::time_point get_current_time() { return steady_clock::now(); }

std::string get_formatted_duration(steady_clock::time_point start,
                                   steady_clock::time_point end) {
  auto duration = duration_cast<microseconds>(end - start);

  TimeDetails result;

  constexpr uint64_t US_PER_MS = 1000;
  constexpr uint64_t US_PER_SEC = 1000 * US_PER_MS;
  constexpr uint64_t US_PER_MIN = 60 * US_PER_SEC;
  constexpr uint64_t US_PER_HOUR = 60 * US_PER_MIN;
  constexpr uint64_t US_PER_DAY = 24 * US_PER_HOUR;

  result.days = duration.count() / US_PER_DAY;
  uint64_t rem = duration.count() % US_PER_DAY;

  result.hours = rem / US_PER_HOUR;
  rem %= US_PER_HOUR;

  result.minutes = rem / US_PER_MIN;
  rem %= US_PER_MIN;

  result.seconds = rem / US_PER_SEC;
  rem %= US_PER_SEC;

  result.milliseconds = rem / US_PER_MS;
  result.remaining_microseconds = rem % US_PER_MS;

  std::string formatted;

  if (result.days > 0) {
    formatted += std::format("{}d ", result.days);
  }
  if (result.hours > 0) {
    formatted += std::format("{}h ", result.hours);
  }
  if (result.minutes > 0) {
    formatted += std::format("{}m ", result.minutes);
  }
  if (result.seconds > 0) {
    formatted += std::format("{}s ", result.seconds);
  }
  if (result.milliseconds > 0) {
    formatted += std::format("{}ms ", result.milliseconds);
  }
  if (result.remaining_microseconds > 0 || formatted.empty()) {
    formatted += std::format("{}us", result.remaining_microseconds);
  }

  return formatted;
}
