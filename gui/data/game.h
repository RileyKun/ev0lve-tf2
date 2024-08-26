//
// Created by panzerfaust on 5/6/2020.
//

#ifndef RENDERER_TEST_GAME_H
#define RENDERER_TEST_GAME_H

#include <string>

namespace gui::data {
    class game {
    public:
        std::string name{};
        unsigned int product_count{};

        void* texture{};
        unsigned int texture_size{};
    };
}

#endif //RENDERER_TEST_GAME_H
