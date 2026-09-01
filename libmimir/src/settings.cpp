#include "mimir/settings.hh"
#include <filesystem>

namespace mimir {

namespace fs = std::filesystem;

fs::path settings::config_dir() const {
  const char* xdg = std::getenv("XDG_CONFIG_HOME");
  if (xdg) return fs::path{xdg} / "mimir";
  const char* home = std::getenv("HOME");
  if (home) return fs::path{home} / ".config" / "mimir";
  return "/etc/mimir";
}

fs::path settings::settings_file() const {
  return config_dir() / "settings.json";
}

settings load_settings() {
  return {};
}

void save_settings(const settings&) {}

}
