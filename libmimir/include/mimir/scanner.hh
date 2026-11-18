#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

namespace mimir {

enum class scan_status {
  idle,
  running,
  finished,
  error
};

struct scan_result {
  std::string path;
  bool infected;
  std::vector<std::string> threats;
};

struct scan_progress {
  scan_status status = scan_status::idle;
  std::string current_file;
  std::optional<std::string> error;
  std::vector<scan_result> results;
};

using progress_callback = std::function<void(const scan_progress&)>;

struct scan_options {
  bool follow_symlinks = false;
  bool scan_archives = true;
  bool scan_mail = true;
  bool detect_pua = false;
  bool all_match = true;
};

class scanner {
public:
  explicit scanner();
  ~scanner();

  scanner(const scanner&) = delete;
  scanner& operator=(const scanner&) = delete;
  scanner(scanner&&) noexcept;
  scanner& operator=(scanner&&) noexcept;

  void scan_file(std::string_view path, progress_callback callback, scan_options opts = {});
  void scan_directory(std::string_view path, progress_callback callback, scan_options opts = {});
  void cancel();

  [[nodiscard]] bool clamd_available() const;
  [[nodiscard]] scan_progress last_progress() const;

private:
  scan_progress progress_;
};

}
