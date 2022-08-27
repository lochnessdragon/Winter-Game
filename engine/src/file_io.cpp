#include <file_io.h>

#include <iostream>
#include <fstream>
#include <exception>

void write_file(const std::string& filename, const char* data) {
    std::ofstream file;
    file.open(filename);
    if(file.is_open()) {
        file << data;
    } else {
        throw std::runtime_error("Failed to write to file!");
    }

    file.close();
}

char* read_file(const std::string& filename) {
    std::ifstream file;
    file.open(filename);

    char* data;

    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        std::streampos size = file.tellg();
        file.seekg(0, std::ios::beg);

        data = new char [size + 1];

        file.read(data, size);
        data[size] = '\0';
    } else {
        throw std::runtime_error("Failed to read file!");
    }
    
    file.close();

    return data;
}