#ifndef HEADER_H
#define HEADER_H

#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <iomanip>
#include "mixmax.hpp"


using Pair = std::pair<int, int>;
using Map = std::map<int, int>;
using Vector = std::vector<int>;
using Matrix = std::vector<Vector>;
using MapH = std::map<Vector, bool>;
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
        {Type::Lotofacil, {25, 16, 5}},
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
    size_t c = 0;
    out << '{';
    for (const auto& i: v) {
        //out << std::setw(2) << std::setfill('0') << i;
        out << i;
        if (c++ < v.size() - 1) out << " ";
    }
    out << '}';
    return out;
}

static myID_t count = std::chrono::steady_clock::now().time_since_epoch().count();
static mixmax_engine mixmax_gen{0, 0, 0, count};

unsigned int mixmax(int n) {
    return mixmax_gen.get_next() % n;
}


#endif // HEADER_H
