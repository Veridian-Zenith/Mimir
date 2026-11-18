#include "mimir/scanner.hh"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <unistd.h>

namespace mimir {

namespace {

constexpr std::size_t BUFFER_SIZE = 4096;

[[nodiscard]]
bool parse_clamd_line(std::string_view line, scan_result& result) {
    if (line.find("OK") != std::string_view::npos) {
        result.infected = false;
        return true;
    }
    if (line.find("FOUND") != std::string_view::npos) {
        result.infected = true;
        auto pos = line.find("FOUND");
        auto start = line.rfind(' ', pos);
        if (start != std::string_view::npos) {
            auto threat = line.substr(start + 1, pos - start - 1);
            result.threats.emplace_back(std::string(threat));
        }
        return true;
    }
    return false;
}

struct PcloseGuard {
    FILE* f;
    ~PcloseGuard() { if (f) pclose(f); }
};

void process_stream(FILE* stream, scan_progress& progress, progress_callback callback) {
    std::array<char, BUFFER_SIZE> buffer{};
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), stream) != nullptr) {
        std::string line(buffer.data());
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        progress.current_file = line;
        scan_result result{};
        if (parse_clamd_line(line, result)) {
            progress.results.push_back(std::move(result));
        }
        if (callback) {
            callback(progress);
        }
    }
}

[[nodiscard]]
int run_clamdscan(const std::string& path, scan_progress& progress,
                  progress_callback callback) {
    std::string cmd = "clamdscan --fdpass --no-summary \"" + path + "\"";
    FILE* stream = popen(cmd.c_str(), "r");
    if (!stream) {
        return -1;
    }
    PcloseGuard guard{stream};
    process_stream(stream, progress, callback);
    return pclose(stream);
}

[[nodiscard]]
int run_clamscan(const std::string& path, scan_progress& progress,
                 progress_callback callback) {
    std::string cmd = "clamscan --no-summary \"" + path + "\"";
    FILE* stream = popen(cmd.c_str(), "r");
    if (!stream) {
        return -1;
    }
    PcloseGuard guard{stream};
    process_stream(stream, progress, callback);
    return pclose(stream);
}

}

scanner::scanner() = default;
scanner::~scanner() = default;

scanner::scanner(scanner&&) noexcept = default;
scanner& scanner::operator=(scanner&&) noexcept = default;

void scanner::scan_file(std::string_view path, progress_callback callback, scan_options) {
    progress_ = {};
    progress_.status = scan_status::running;

    auto path_str = std::string(path);
    int exit_code = 0;

    if (clamd_available()) {
        exit_code = run_clamdscan(path_str, progress_, callback);
    } else {
        exit_code = run_clamscan(path_str, progress_, callback);
    }

    if (exit_code < 0) {
        progress_.status = scan_status::error;
        progress_.error = "Failed to execute scanner";
    } else if (exit_code == 0) {
        progress_.status = scan_status::finished;
    } else if (exit_code == 1) {
        progress_.status = scan_status::finished;
    } else {
        progress_.status = scan_status::error;
        progress_.error = "Scanner exited with code " + std::to_string(exit_code);
    }
    if (callback) {
        callback(progress_);
    }
}

void scanner::scan_directory(std::string_view path, progress_callback callback, scan_options opts) {
    progress_ = {};
    progress_.status = scan_status::running;

    auto path_str = std::string(path);

    std::string cmd = clamd_available() ? "clamdscan --fdpass --no-summary" : "clamscan --no-summary";
    if (opts.follow_symlinks) {
        cmd += " --follow-dir-symlinks=1 --follow-file-symlinks=1";
    }
    cmd += " -r \"" + path_str + "\"";

    FILE* stream = popen(cmd.c_str(), "r");
    if (!stream) {
        progress_.status = scan_status::error;
        progress_.error = "Failed to execute scanner";
        if (callback) {
            callback(progress_);
        }
        return;
    }
    PcloseGuard guard{stream};
    process_stream(stream, progress_, callback);
    int exit_code = pclose(stream);

    if (exit_code < 0) {
        progress_.status = scan_status::error;
        progress_.error = "Failed to execute scanner";
    } else if (exit_code == 0) {
        progress_.status = scan_status::finished;
    } else if (exit_code == 1) {
        progress_.status = scan_status::finished;
    } else {
        progress_.status = scan_status::error;
        progress_.error = "Scanner exited with code " + std::to_string(exit_code);
    }
    if (callback) {
        callback(progress_);
    }
}

void scanner::cancel() {
    progress_.status = scan_status::idle;
    progress_.error = "Cancelled";
}

bool scanner::clamd_available() const {
    FILE* stream = popen("clamdscan --version 2>/dev/null", "r");
    if (!stream) {
        return false;
    }
    pclose(stream);
    return true;
}

scan_progress scanner::last_progress() const {
    return progress_;
}

}