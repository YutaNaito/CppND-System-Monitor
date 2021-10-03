#include "format.h"

#include <string>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string Format::ElapsedTime(long seconds) {
  int HH = seconds / 3600;
  int MM = (seconds % 3600) / 60;
  int ss = seconds % 60;
  return (to_string(HH) + ":" + to_string(MM) + ":" + to_string(ss));
}