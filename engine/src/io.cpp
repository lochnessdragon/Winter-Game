#include <io.h>

#include <limits>

std::streamsize io::getFileSize(std::ifstream& file) {
	// retrieved from stack overflow answer: https://stackoverflow.com/questions/22984956/tellg-function-give-wrong-size-of-file/22986486#22986486
	file.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = file.gcount();
	file.clear();   //  Since ignore will have set eof.
	file.seekg(0, std::ios_base::beg);

	return length;
}