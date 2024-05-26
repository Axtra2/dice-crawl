#pragma once

#include <optional>
#include <string>

std::optional<std::string> getFileContents(const char *filename);

bool writeToFile(std::string_view sv, const char* filename);
