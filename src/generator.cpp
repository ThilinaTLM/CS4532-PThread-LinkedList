//
// Created by tlm on 1/23/23.
//

#include <vector>
#include <random>
#include <algorithm>
#include <mutex>

constexpr int MIN_VALUE = 0;
constexpr int MAX_VALUE = 65535;
constexpr int SEED = 2023;

namespace generator {


    class NumberGenerator {
    private:
        // props
        int random_seed;
        int min_value;
        int max_value;
        int size;

        // state
        std::vector<int> values;
        int n_index = 0;


    public:
        explicit NumberGenerator(int size) :
                random_seed(SEED),
                min_value(MIN_VALUE),
                max_value(MAX_VALUE),
                size(size) {
            init();
        }

        bool has() {
            auto res = n_index < size;
            return res;
        }

        bool has_until(int n) {
            auto res = n_index < n;
            return res;
        }

        int next() {
            auto val = values[n_index];
            n_index++;
            return val;
        }

        void init() {
            // generate number sequence
            for (int i = min_value; i <= max_value; i++) {
                values.push_back(i);
            }
            // shuffle randomly
            std::mt19937 gen(random_seed);
            std::shuffle(values.begin(), values.end(), gen);
        }

    };

}
