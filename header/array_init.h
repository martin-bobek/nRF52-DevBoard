#ifndef ARRAY_INIT__
#define ARRAY_INIT__

#include <cstddef>
#include <initializer_list>

struct Element {
public:
    constexpr Element(size_t _pos, uint32_t _val) : pos(_pos), val(_val) {}
    size_t pos;
    uint32_t val;
};

template <size_t N, uint32_t Default>
class ArrayInit {
public:
    constexpr ArrayInit(const std::initializer_list<Element> list) : arr() {
        for (size_t i = 0; i < N; i++)
            arr[i] = Default;

        for (auto it = list.begin(); it < list.end(); it++)
            arr[it->pos] = it->val;
    }

    uint32_t operator[](size_t index) const { return arr[index]; }
private:
    uint32_t arr[N];
};

#endif
