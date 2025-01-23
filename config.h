#include <string>

std::string get_path_func(std::string name) {
    if (!name.empty()) {
        char first_char = std::tolower(name[0]);
        if(isalnum(first_char)) {
            return "out/" + std::string(1, first_char);
        } else {
            return "out/unknown";
        }
    }
    return "out/unknown";
}