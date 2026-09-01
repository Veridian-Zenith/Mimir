#include "mimir/scanner.hh"

namespace mimir {

scanner::scanner() = default;
scanner::~scanner() = default;

scanner::scanner(scanner&&) noexcept = default;
scanner& scanner::operator=(scanner&&) noexcept = default;

void scanner::scan_file(std::string_view, progress_callback, scan_options) {}
void scanner::scan_directory(std::string_view, progress_callback, scan_options) {}
void scanner::cancel() {}
bool scanner::clamd_available() const { return false; }
scan_progress scanner::last_progress() const { return progress_; }

}
