#include <memory>
#include <string>

template <typename... Args>
std::string format(const std::string& format, Args... args) {
  auto size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}