#include "mimir/signature.hh"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <optional>
#include <regex>
#include <string>

namespace mimir {

namespace {

constexpr std::size_t BUFFER_SIZE = 4096;

[[nodiscard]]
std::optional<signature_info> parse_version_output() {
    std::array<char, BUFFER_SIZE> buffer{};
    signature_info info{};

    FILE* stream = popen("clamscan --version 2>&1", "r");
    if (!stream) {
        return std::nullopt;
    }

    bool has_content = false;
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), stream) != nullptr) {
        std::string line(buffer.data());
        has_content = true;
        if (line.find("ClamAV") != std::string::npos) {
            std::regex ver_regex(R"(ClamAV\s+(\d+)\.(\d+)\.(\d+))");
            std::smatch match;
            if (std::regex_search(line, match, ver_regex)) {
                info.main_version = match[0].str();
            }
        }
        if (line.find("main.cvd") != std::string::npos ||
            line.find("daily.cld") != std::string::npos ||
            line.find("daily.cvd") != std::string::npos) {
            std::regex sig_regex(R"((\d+)\s+(\w+)\s+(\d+-\w+-\d+))");
            std::smatch match;
            if (std::regex_search(line, match, sig_regex)) {
                if (line.find("main") != std::string::npos) {
                    info.signature_count = match[1].str();
                    info.signature_date = match[3].str();
                    info.main_loaded = true;
                }
                if (line.find("daily") != std::string::npos) {
                    info.daily_version = match[1].str();
                    info.daily_loaded = true;
                }
            }
        }
    }
    pclose(stream);

    if (!has_content) {
        return std::nullopt;
    }
    return info;
}

[[nodiscard]]
update_progress run_freshclam() {
    update_progress progress{};
    progress.status = update_status::running;

    std::array<char, BUFFER_SIZE> buffer{};
    FILE* stream = popen("freshclam --quiet 2>&1", "r");
    if (!stream) {
        progress.status = update_status::error;
        progress.error = "Failed to execute freshclam";
        return progress;
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), stream) != nullptr) {
        std::string line(buffer.data());
        if (line.find("Downloading") != std::string::npos) {
            progress.percentage = 50;
        }
    }

    int exit_code = pclose(stream);
    if (exit_code == 0) {
        progress.status = update_status::finished;
        progress.percentage = 100;
    } else if (exit_code < 0) {
        progress.status = update_status::error;
        progress.error = "Failed to execute freshclam";
    } else {
        progress.status = update_status::error;
        progress.error = "freshclam exited with code " + std::to_string(exit_code);
    }
    return progress;
}

}

std::optional<signature_info> get_signature_info() {
    return parse_version_output();
}

update_progress update_signatures() {
    return run_freshclam();
}

bool clamd_running() {
    FILE* stream = popen("systemctl is-active clamav-daemon 2>/dev/null", "r");
    if (!stream) {
        return false;
    }
    std::array<char, 64> buffer{};
    if (fgets(buffer.data(), static_cast<int>(buffer.size()), stream) != nullptr) {
        std::string result(buffer.data());
        pclose(stream);
        return result.find("active") != std::string::npos;
    }
    pclose(stream);
    return false;
}

}