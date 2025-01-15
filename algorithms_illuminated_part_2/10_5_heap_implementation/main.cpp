#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>

class Heap {
public:
    void push(int val) {
        _data.push_back(val);

        size_t j = _data.size() - 1;

        while (j > 0) {
            size_t i = parent_idx(j);
            if (_data[i] > _data[j]) break;

            std::swap(_data[i], _data[j]);
            j = i;
        }
    }

    int pop() {
        assert(!_data.empty());

        int result = _data.front();
        if (_data.size() > 1) _data.front() = _data.back();
        _data.pop_back();
        heapify(0);

        return result;
    }

    size_t size() const {
        return _data.size();
    }

    size_t empty() const {
        return _data.empty();
    }

    auto begin() {
        return _data.begin();
    }

    auto end() {
        return _data.end();
    }
private:
    enum Direction {
        LEFT,
        RIGHT
    };

    std::vector<int> _data;

    /*
     * i - индекс родителя
     * jl - индекс левого ребенка
     * jr - индекс правого ребенка
     *
     * jl = 2 * i + 1
     * jr = 2 * i + 2
     * @link https://ru.wikipedia.org/wiki/%D0%94%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%B0%D1%8F_%D0%BA%D1%83%D1%87%D0%B0
     */
    static size_t child_idx(size_t i, Direction direction) {
        return 2 * i + 1 + static_cast<int>(direction);
    }

    /*
     * i - индекс родителя
     * jl - индекс левого ребенка
     * jr - индекс правого ребенка
     *
     * 1) jl = 2*i + 1
     * 2) jr = 2*i + 2
     * @link https://ru.wikipedia.org/wiki/%D0%94%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%B0%D1%8F_%D0%BA%D1%83%D1%87%D0%B0
     *
     * 3) i = (jl - 1) / 2 из (1)
     * 4) i = (jr - 2) / 2 из (2)
     *
     * 5) jr = jl + 1 из (1) и (2)
     * 6) jl % 2 = 1
     * 7) jr % 2 = 0
     * @link https://upload.wikimedia.org/wikipedia/commons/thumb/3/38/Max-Heap.svg/800px-Max-Heap.svg.png
     *
     * 8) jr = jr + jr % 2 из (7)
     * 9) jr = jl + jl % 2 из (5) и (6)
     * 10) j + j % 2 = (jr + jr % 2) || (jl + jl % 2)
     *
     * 11) i = (j + j % 2 - 2) / 2
     */
    static size_t parent_idx(size_t j) {
        assert(j > 0);
        return (j + j % 2 - 2) / 2;
    }

    void heapify(size_t i) {
        while (true) {
            size_t largest = i;
            size_t jl = child_idx(i, LEFT);
            size_t jr = child_idx(i, RIGHT);

            if (jl < _data.size() && _data[jl] > _data[largest]) largest = jl;
            if (jr < _data.size() && _data[jr] > _data[largest]) largest = jr;

            if (i == largest) break;

            std::swap(_data[i], _data[largest]);
            i = largest;
        }
    }
};

enum Operation {
    PUSH,
    POP
};

void test(Heap& heap, std::vector<int>& expected_heap, Operation op, int val = 0)
{
    int expected_val;
    switch (op) {
        case PUSH:
            std::cout << "push " << val << std::endl;

            heap.push(val);
            assert(std::is_heap(std::begin(heap), std::end(heap)) && "pop");

            expected_heap.push_back(val);
            std::push_heap(std::begin(expected_heap), std::end(expected_heap));
            break;
        case POP:
            val = heap.pop();
            assert(std::is_heap(std::begin(heap), std::end(heap)) && "pop");

            expected_val = expected_heap.front();
            std::pop_heap(std::begin(expected_heap), std::end(expected_heap));
            expected_heap.pop_back();

            std::cout << "pop actual " << val << " vs expected " << expected_val << std::endl;
            assert(val == expected_val && "pop");
            break;
    }
}

int main() {
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> val_distrib(0, 100);
        std::uniform_int_distribution<> operation_distrib(0, 1);

        Heap heap;
        std::vector<int> expected_heap;
        for (size_t i = 0; i < 1'000'000; ++i) {
            Operation op = heap.empty() ? PUSH : static_cast<Operation>(operation_distrib(gen));
            test(heap, expected_heap, op, val_distrib(gen));
        }
    }

    return 0;
}
