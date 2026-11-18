#include "mimir/settings.hh"
#include <cstdlib>
#include <fstream>
#include <sstream>

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

namespace {

std::string escape_json(const std::string& s) {
    std::ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default: o << c;
        }
    }
    return o.str();
}

std::string read_file(const fs::path& path) {
    std::ifstream f(path);
    if (!f) return "";
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

settings parse_settings(const std::string& content) {
    settings s;
    std::istringstream stream(content);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("\"scan_path\"") != std::string::npos) {
            auto start = line.find('"');
            auto end = line.rfind('"');
            if (start != std::string::npos && end != std::string::npos && start < end) {
                s.scan_path = line.substr(start + 1, end - start - 1);
            }
        }
        if (line.find("\"use_clamd\"") != std::string::npos) {
            s.use_clamd = line.find("true") != std::string::npos;
        }
        if (line.find("\"confirm_delete\"") != std::string::npos) {
            s.confirm_delete = line.find("true") != std::string::npos;
        }
        if (line.find("\"show_notifications\"") != std::string::npos) {
            s.show_notifications = line.find("true") != std::string::npos;
        }
    }
    return s;
}

}

settings load_settings() {
    settings s;
    const char* home = std::getenv("HOME");
    if (!home) return s;
    fs::path config_path = fs::path{home} / ".config" / "mimir" / "settings.json";
    std::string content = read_file(config_path);
    if (content.empty()) return s;
    return parse_settings(content);
}

void save_settings(const settings& s) {
    const char* home = std::getenv("HOME");
    if (!home) return;
    fs::path config_dir = fs::path{home} / ".config" / "mimir";
    fs::create_directories(config_dir);
    fs::path config_path = config_dir / "settings.json";
    std::ofstream f(config_path);
    if (!f) return;
    f << "{\n";
    f << "  \"scan_path\": \"" << escape_json(s.scan_path) << "\",\n";
    f << "  \"use_clamd\": " << (s.use_clamd ? "true" : "false") << ",\n";
    f << "  \"confirm_delete\": " << (s.confirm_delete ? "true" : "false") << ",\n";
    f << "  \"show_notifications\": " << (s.show_notifications ? "true" : "false") << "\n";
    f << "}\n";
}

}