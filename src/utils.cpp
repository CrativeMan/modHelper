#include "utils.hpp"

#include <ctime>

namespace util {
std::string getDateAsString() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
  return buf;
}
} // namespace util
