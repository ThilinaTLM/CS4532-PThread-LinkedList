//
// Created by tlm on 1/23/23.
//

#ifndef CS4532_LAB01_NODE_H
#define CS4532_LAB01_NODE_H

namespace llist {

    template <typename T>
    class Node {
    public:
        [[maybe_unused]] T data;
        [[maybe_unused]] Node<T>* next;

        explicit Node(T data);
    };

    template<typename T>
    Node<T>::Node(T data) : next(nullptr) {
        this->data = data;
    }

}

#endif //CS4532_LAB01_NODE_H
