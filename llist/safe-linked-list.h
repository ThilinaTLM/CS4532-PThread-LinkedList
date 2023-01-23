//
// Created by tlm on 1/23/23.
//

#ifndef CS4532_LAB01_SAFE_LINKED_LIST_H
#define CS4532_LAB01_SAFE_LINKED_LIST_H

#include "node.h"
#include "abstract-linked-list.h"
#include <mutex>

namespace llist {

    template<typename T>
    class SafeLinkedList: public AbstractLinkedList<T> {

    private:
        Node<T> *head;
        Node<T> *tail;
        long size;
        pthread_mutex_t mtx{};

    public:
        SafeLinkedList();

        bool member(T data);

        void insert(T data);

        void remove(T data);

    };

    template<typename T>
    SafeLinkedList<T>::SafeLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    template<typename T>
    void SafeLinkedList<T>::insert(T data) {
        pthread_mutex_lock(&mtx);
        auto *node = new Node<T>(data);
        if (head == nullptr) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        size++;
        pthread_mutex_unlock(&mtx);
    }

    template<typename T>
    bool SafeLinkedList<T>::member(T data) {
        pthread_mutex_lock(&mtx);
        Node<T> *current = head;
        while (current != nullptr) {
            if (current->data == data) {
                pthread_mutex_unlock(&mtx);
                return true;
            }
            current = current->next;
        }
        pthread_mutex_unlock(&mtx);
        return false;
    }

    template<typename T>
    void SafeLinkedList<T>::remove(T data) {
        pthread_mutex_lock(&mtx);
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
                pthread_mutex_unlock(&mtx);
                return;
            }
            previous = current;
            current = current->next;
        }
        pthread_mutex_unlock(&mtx);
    }

}


#endif //CS4532_LAB01_SAFE_LINKED_LIST_H
