#pragma once

#include <chrono>
#include <string>

std::chrono::steady_clock::time_point get_current_time();

std::string get_formatted_duration(std::chrono::steady_clock::time_point start,
                                   std::chrono::steady_clock::time_point end);
