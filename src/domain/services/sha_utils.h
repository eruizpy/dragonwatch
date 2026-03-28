#pragma once

#include <cstddef>
#include <string>

namespace dragonwatch::domain {

std::string normalizeSha(const std::string& sha, std::size_t shortLen = 7);

}  // namespace dragonwatch::domain
