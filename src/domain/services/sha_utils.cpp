#include "sha_utils.h"

namespace dragonwatch::domain {

std::string normalizeSha(const std::string& sha, std::size_t shortLen) {
  if (sha.empty()) {
    return "";
  }
  if (sha.size() <= shortLen) {
    return sha;
  }
  return sha.substr(0, shortLen);
}

}  // namespace dragonwatch::domain
