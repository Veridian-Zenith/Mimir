#pragma once

#include <string_view>
#include <string>

namespace mimir {

struct theme_color {
  std::string_view token;
  std::string_view hex;
};

struct theme {
  std::string_view name = "blackxred";

  theme_color bg_primary = {"--bg-primary", "#000000"};
  theme_color bg_surface = {"--bg-surface", "#0D0D0D"};
  theme_color bg_surface_hover = {"--bg-surface-hover", "#1A1A1A"};

  theme_color text_primary = {"--text-primary", "#FFFFFF"};
  theme_color text_secondary = {"--text-secondary", "#B3B3B3"};
  theme_color text_disabled = {"--text-disabled", "#666666"};

  theme_color accent_primary = {"--accent-primary", "#FF1A1A"};
  theme_color accent_secondary = {"--accent-secondary", "#CC0000"};
  theme_color accent_glow = {"--accent-glow", "#FF1A1A33"};

  theme_color success = {"--success", "#1AFF1A"};
  theme_color warning = {"--warning", "#FFB31A"};
  theme_color error = {"--error", "#FF0033"};
  theme_color info = {"--info", "#1A99FF"};

  theme_color border = {"--border", "#262626"};
  theme_color border_active = {"--border-active", "#FF1A1A"};
};

[[nodiscard]] constexpr theme default_theme() noexcept {
  return {};
}

}
