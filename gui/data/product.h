//
// Created by panzerfaust on 5/6/2020.
//

#ifndef RENDERER_TEST_PRODUCT_H
#define RENDERER_TEST_PRODUCT_H

#include <string>

namespace gui::data {
    enum class product_status {
        STATUS_OFFLINE,
        STATUS_ONLINE
    };

    class product {
    public:
        int id{};
        std::string name{};
        std::string last_update{};
        product_status status{};
        std::string custom_message{};
    };
}

#endif //RENDERER_TEST_PRODUCT_H
