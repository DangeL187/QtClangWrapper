#include "readFile.hpp"

std::string readFile(const std::string& file_path) {
    std::string output;

    std::ifstream file(file_path);
    if (!file.is_open()) return "File " + file_path + " not found";

    std::string line;
    while (std::getline(file, line)) {
        output += line + "\n";
    }
    file.close();

    return output;
}