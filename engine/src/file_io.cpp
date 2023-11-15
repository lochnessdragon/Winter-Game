#include <file_io.h>

#include <iostream>
#include <fstream>
#include <exception>
#include <limits>
#include <log.h>

void write_file(const std::string& filename, const char* data) {
    std::ofstream file;
    file.open(filename);
    if(file.is_open()) {
        file << data;
    } else {
        throw std::runtime_error(std::string("Failed to write to file: ") + filename);
    }

    file.close();
}

char* read_file(const std::string& filename) {
    std::ifstream file(filename);

    char* data;

    if(file.is_open()) {
        file.ignore(std::numeric_limits<std::streamsize>::max());
        std::streamsize size = file.gcount();
        file.clear();   //  Since ignore will have set eof.
        file.seekg(0, std::ios_base::beg);

        //Log::getRendererLog()->trace("Creating new file buf with size: {}", size);
        data = new char [(uint32_t)size + 1u]; // extra byte for the end stream delim.

        file.read(data, size);
        //Log::getRendererLog()->trace("Read: {} bytes.", file.gcount());
        
        data[size] = '\0';

    } else {
        throw std::runtime_error(std::string("Failed to read file: ") + filename);
    }

    return data;
}

std::string read_file_str(const std::string& filename) {
    std::ifstream file(filename, std::ios::in);
    std::string data(std::istreambuf_iterator<char>{file}, {});

    return data;
}