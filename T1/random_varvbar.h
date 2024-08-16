#pragma once

#include <vector>
#include <random>
#include <numeric>

std::vector<float> GetRandomVector(int size) {
    static std::mt19937 engine;
    std::uniform_real_distribution<float> d(-100, 100);

    std::vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}