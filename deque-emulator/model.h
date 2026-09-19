#pragma once

#include <vector>
#include <string>

struct Model {
    using container = std::vector<std::string>;
    container items;
    container::iterator iterator = items.begin();
};
