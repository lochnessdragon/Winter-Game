#pragma once

#include <string>

void write_file(const std::string& filename, const char* data);
char* read_file(const std::string& filename);
std::string read_file_str(const std::string& filename);