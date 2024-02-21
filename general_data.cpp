#include "general_data.h"

sf::Font LoadFont(std::string path) {
    sf::Font ans;
    ans.loadFromFile(path);
    return ans;
}
