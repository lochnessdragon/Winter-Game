#pragma once

#include <fstream>

namespace io {
	std::streamsize getFileSize(std::ifstream& file);
};