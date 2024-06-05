#include <string_view>
#include <optional>
#include <fstream>
#include <string>

std::optional<std::string> getFileContents(const char* filename) {
    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    if (fin) {
        std::string contents;
        fin.seekg(0, std::ios::end);
        contents.resize(fin.tellg());
        fin.seekg(0, std::ios::beg);
        fin.read(&contents[0], contents.size());
        fin.close();
        return contents;
    }
    return std::nullopt;
}

bool writeToFile(std::string_view sv, const char* filename) {
    std::ofstream fout(filename);
    if (fout) {
        return false;
    }
    fout << sv;
    return true;
}
