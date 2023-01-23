//
// Created by tlm on 1/23/23.
//

#ifndef CS4532_LAB01_ABSTRACT_LINKED_LIST_H
#define CS4532_LAB01_ABSTRACT_LINKED_LIST_H

namespace llist {

    template <typename T>
    class AbstractLinkedList {
    public:
        virtual bool member(T data) = 0;

        virtual void insert(T data) = 0;

        virtual void remove(T data) = 0;
    };

}

#endif //CS4532_LAB01_ABSTRACT_LINKED_LIST_H
