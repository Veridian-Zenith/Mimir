#pragma once

#include <chrono>
#include <string>
#include <optional>

namespace mimav {

struct signature_info {
  std::string signature_count;
  std::string signature_date;
  std::string main_version;
  std::string daily_version;
  bool daily_loaded = false;
  bool main_loaded = false;
};

enum class update_status {
  idle,
  running,
  finished,
  error
};

struct update_progress {
  update_status status = update_status::idle;
  std::optional<std::string> error;
  int percentage = 0;
};

}

namespace mimir {

using namespace mimav;

[[nodiscard]] std::optional<signature_info> get_signature_info();
[[nodiscard]] update_progress update_signatures();
[[nodiscard]] bool clamd_running();

}
