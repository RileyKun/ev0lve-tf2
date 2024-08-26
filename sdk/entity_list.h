//
// Created by panzerfaust on 5/26/2020.
//

#ifndef EV0LVE_TF_ENTITY_LIST_H
#define EV0LVE_TF_ENTITY_LIST_H

namespace sdk {
    class c_entity_list {
        virtual void pad_0() = 0;
        virtual void pad_1() = 0;
        virtual void pad_2() = 0;
    public:
        virtual entity* get(int id) = 0;
        virtual entity* get_from_handle(handle h) = 0;
        virtual int count(bool incl_client) = 0;
        virtual int highest() = 0;
    };

    inline c_entity_list* entity_list = nullptr;
}

#endif //EV0LVE_TF_ENTITY_LIST_H
