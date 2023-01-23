//
// Created by tlm on 1/23/23.
//

#ifndef CS4532_LAB01_SAFE_FAST_LINKED_LIST_H
#define CS4532_LAB01_SAFE_FAST_LINKED_LIST_H

#include "node.h"
#include "abstract-linked-list.h"
#include <mutex>
#include <shared_mutex>

namespace llist {

    template<typename T>
    class SafeFastLinkedList: public AbstractLinkedList<T> {

    private:
        Node<T> *head;
        Node<T> *tail;
        long size;
        std::shared_timed_mutex mutex{};

    public:
        SafeFastLinkedList();

        bool member(T data);

        void insert(T data);

        void remove(T data);

    };

    template<typename T>
    SafeFastLinkedList<T>::SafeFastLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    template<typename T>
    void SafeFastLinkedList<T>::insert(T data) {
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
        auto *node = new Node<T>(data);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        size++;
    }

    template<typename T>
    bool SafeFastLinkedList<T>::member(T data) {
        std::shared_lock<std::shared_timed_mutex> lock(mutex);
        Node<T> *current = head;
        while (current != nullptr) {
            if (current->data == data) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    template<typename T>
    void SafeFastLinkedList<T>::remove(T data) {
        std::unique_lock<std::shared_timed_mutex> lock(mutex);
        Node<T> *current = head;
        Node<T> *previous = nullptr;
        while (current != nullptr) {
            if (current->data == data) {
                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                size--;
                return;
            }
            previous = current;
            current = current->next;
        }
    }

}


#endif //CS4532_LAB01_SAFE_FAST_LINKED_LIST_H
