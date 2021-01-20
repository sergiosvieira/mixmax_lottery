#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "mixmax.hpp"

using Vector = std::vector<int>;
struct Param {int N = 0, M = 0, K = 0;};
enum class Type {MegaSena, Lotofacil, Quina, Lotomania, Timemania, Duplasena};
struct state_hash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};
using TypeParamMap = std::unordered_map<Type, Param, state_hash>;
using GamesMap = std::unordered_map<Type, int, state_hash>;
using TypeTitle = std::unordered_map<Type, std::string, state_hash>;

TypeParamMap params = {
    {Type::MegaSena, {60, 6, 6}},
    {Type::Lotofacil, {25, 15, 5}},
    {Type::Quina, {80, 5, 5}},
    {Type::Lotomania, {100, 50, 10}},
    {Type::Timemania, {80, 10, 10}},
    {Type::Duplasena, {50, 6, 6}},
};

TypeTitle titles = {
    {Type::MegaSena, "MegaSena"},
    {Type::Lotofacil, "Lotofácil"},
    {Type::Quina, "Quina"},
    {Type::Lotomania, "Lotomania"},
    {Type::Timemania, "Timemania"},
    {Type::Duplasena, "Duplasena"},
};

std::ostream& operator<<(std::ostream& out, const Vector& v) {
    out << "[";
    size_t c = 0;
    for (const auto& i: v) {
        out << i;
        if (c++ < v.size() - 1) out << ", ";
    }
    out << "]";
    return out;
}

static myID_t count = std::chrono::steady_clock::now().time_since_epoch().count();
static mixmax_engine gen{0, 0, 0, count};

unsigned int minmax(int n)
{
    return gen.get_next() % n;
}

Vector generate(Type type) {
    Vector result;
    Vector permutation(params[type].N);
    std::iota(permutation.begin(), permutation.end(), 1);
    std::random_shuffle(permutation.begin(), permutation.end(), minmax);
    for (int i = 1; i < params[type].N; i++) {
        std::swap(permutation[i], permutation[std::uniform_real_distribution<double>(0, i)(gen)]);
    }
    int si = gen.get_next() % (params[type].N - params[type].M);
    for (int i = si; i < si + params[type].M; ++i) {
        result.push_back(permutation[i]);
    }
    std::sort(result.begin(), result.end());
    return result;
}


int main() {
    GamesMap games = {
        {Type::MegaSena, 2},
        {Type::Lotofacil, 2},
        {Type::Quina, 2},
        {Type::Lotomania, 2},
        {Type::Timemania, 2},
        {Type::Duplasena, 2},
        };
    for (auto [k, v]: games) {
        std::cout << titles[k] << '\n';
        for (size_t i = 0; i < v; ++i) {
            std::cout << '\t' << generate(k) << '\n';
        }
    }
    return 0;
}
