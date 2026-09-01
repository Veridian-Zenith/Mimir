#pragma once

#include <filesystem>
#include <string>
#include <optional>

namespace mimir {

struct settings {
  std::string scan_path;
  bool use_clamd = true;
  bool confirm_delete = true;
  bool show_notifications = true;

  std::filesystem::path config_dir() const;
  std::filesystem::path settings_file() const;
};

[[nodiscard]] settings load_settings();
void save_settings(const settings&);

}
