#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-non-private-member-variables-in-classes"

#include <iostream>
#include <chrono>
#include <tuple>
#include <vector>
#include <pthread.h>

#include "llist.h"
#include "generator.cpp"

struct ThreadContext {

    // thread props
    int thread_id;
    int value_count;
    int member_ops;
    int insert_ops;
    int remove_ops;
    llist::AbstractLinkedList<int> *list;
    generator::NumberGenerator *gen_insert;
    generator::NumberGenerator *gen_member;
    generator::NumberGenerator *gen_remove;

    // results
    std::vector<long>* durations;

};

double mean(const std::vector<long> &results) {
    double sum = 0;
    for (auto result: results) {
        sum += result;
    }
    return sum / (long double) results.size();
}

double std_dev(std::vector<long> &results, long double mean) {
    double sum = 0;
    for (auto result: results) {
        sum += pow((double) result - mean, 2);
    }
    return sqrt((double) sum / (double) results.size());
}

void *run(void *args) {

    ThreadContext context = *(ThreadContext *) args;
    llist::AbstractLinkedList<int>* list = context.list;
    generator::NumberGenerator gen_insert = *context.gen_insert;
    generator::NumberGenerator gen_member = *context.gen_member;
    generator::NumberGenerator gen_remove = *context.gen_remove;

    // results
    std::vector<long> insert_results;
    std::vector<long> member_results;
    std::vector<long> remove_results;

    // insert operations
    for (int i = 0; i < context.insert_ops; i++) {
        auto val = gen_insert.next();
        auto start = std::chrono::high_resolution_clock::now();
        list->insert(val);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        context.durations->push_back(duration);
    }

    // member operations
    for (int i = 0; i < context.member_ops; i++) {
        auto val = gen_insert.next();
        auto start = std::chrono::high_resolution_clock::now();
        list->member(val);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        context.durations->push_back(duration);
    }

    // remove operations
    for (int i = 0; i < context.remove_ops; i++) {
        auto val = gen_remove.next();
        auto start = std::chrono::high_resolution_clock::now();
        list->remove(val);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        context.durations->push_back(duration);
    }
}

enum Operation {
    MEMBER = 0,
    INSERT = 1,
    REMOVE = 2
};

enum ListType {
    LINKED_LIST = 0,
    SAFE_LINKED_LIST = 1,
    SAFE_FAST_LINKED_LIST = 2
};

std::pair<double, double> start_test(ListType lt, int threads, int num_of_vals, int num_of_op, std::tuple<double, double, double> op_frac) {
    int const m_member = floor(std::get<Operation::MEMBER>(op_frac) * (double) num_of_op);
    int const m_insert = floor(std::get<Operation::INSERT>(op_frac) * (double) num_of_op);
    int const m_remove = floor(std::get<Operation::REMOVE>(op_frac) * (double) num_of_op);

    // initiate list
    llist::AbstractLinkedList<int> *list;
    switch (lt) {
        case LINKED_LIST:
            list = new llist::LinkedList<int>();
            break;
        case SAFE_LINKED_LIST:
            list = new llist::SafeLinkedList<int>();
            break;
        case SAFE_FAST_LINKED_LIST:
            list = new llist::SafeFastLinkedList<int>();
            break;
    }

    // number generator
    auto gen = generator::NumberGenerator(num_of_vals);
    while (gen.has()) {
        list->insert(gen.next());
    }

    // starting threads
    pthread_t thread_ids[threads];
    std::vector<ThreadContext*> contexts;

    for (int i = 0; i < threads; i++) {
        auto ctx = new ThreadContext();
        ctx->thread_id = i;
        ctx->value_count = num_of_vals;
        ctx->member_ops = m_member / threads;
        ctx->insert_ops = m_insert / threads;
        ctx->remove_ops = m_remove / threads;
        ctx->list = list;
        ctx->durations = new std::vector<long>();
        ctx->gen_insert = new generator::NumberGenerator(m_insert);
        ctx->gen_member = new generator::NumberGenerator(m_member);
        ctx->gen_remove = new generator::NumberGenerator(m_remove);
        pthread_create(&thread_ids[i], NULL, run, ctx);
        contexts.push_back(ctx);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    std::vector<long> durations;
    for (auto ctx: contexts) {
        for (auto duration: *ctx->durations) {
            durations.push_back(duration);
        }
    }

    auto duration_mean = mean(durations);
    auto duration_std_dev = std_dev(durations, duration_mean);

    return std::make_pair(duration_mean, duration_std_dev);
}

void run_case(ListType lt, int n, int m, std::tuple<double, double, double> op_frac) {
    std::cout << "================================ CASE ==============================" << std::endl;
    std::cout << "N: " << n << ", M: " << m << ", M_member: " <<
    std::get<Operation::MEMBER>(op_frac) << ", M_insert: " <<
    std::get<Operation::INSERT>(op_frac) << ", M_remove: " <<
    std::get<Operation::REMOVE>(op_frac) << std::endl << std::endl;

    std::vector const threads = {1, 2, 4, 8};
    int trials = 400;

    if (lt == ListType::LINKED_LIST) {
        std::cout << ":: Linked List" << std::endl;
    } else if (lt == ListType::SAFE_LINKED_LIST) {
        std::cout << ":: Safe Linked List" << std::endl;
    } else if (lt == ListType::SAFE_FAST_LINKED_LIST) {
        std::cout << ":: Safe Fast Linked List" << std::endl;
    }
    for (auto thread: threads) {
        // run 400 times and get average
        double mean = 0;
        double std_dev = 0;
        for (int i = 0; i < trials; i++) {
            auto res = start_test(
                    lt,
                    thread,
                    n, m,
                    op_frac
            );
            mean += res.first;
            std_dev += res.second;
        }
        mean /= trials;
        std_dev /= trials;
        std::cout << "Threads: " << thread << ", Mean: " << mean << ", Std Dev: " << std_dev << std::endl;
    }

}


int main() {
//    run_case(ListType::SAFE_FAST_LINKED_LIST, 1000, 10000, std::make_tuple(0.99, 0.005, 0.005));
//    run_case(ListType::SAFE_FAST_LINKED_LIST, 1000, 10000, std::make_tuple(0.90, 0.05, 0.05));
    run_case(ListType::SAFE_FAST_LINKED_LIST, 1000, 10000, std::make_tuple(0.50, 0.25, 0.25));
}

#pragma clang diagnostic pop