#include "mimir/signature.hh"
#include <optional>

namespace mimir {

std::optional<signature_info> get_signature_info() {
  return std::nullopt;
}

update_progress update_signatures() {
  return {};
}

bool clamd_running() {
  return false;
}

}
