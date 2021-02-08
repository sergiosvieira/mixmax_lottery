#include <iostream>
#include "lotofacil.h"
#include "lotomania.h"

template <typename T>
Vector generate(Type type, T gen, std::function<unsigned int(int)> randomize) {
    Vector result;
    Vector permutation(params[type].N);
    std::iota(permutation.begin(), permutation.end(), 1);
    std::random_shuffle(permutation.begin(), permutation.end(), randomize);
    for (int i = 1; i < params[type].N; i++) {
        std::swap(permutation[i], permutation[std::uniform_real_distribution<double>(0, i)(gen)]);
    }
    for (int i = 0; i < params[type].M; ++i) {
        result.push_back(permutation[i]);
    }
    std::sort(result.begin(), result.end());
    return result;
}

double std_dev(const Vector& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / v.size());
}

Map init(Type type) {
    Map result;
    Param p = params[type];
    for (int i = 1; i <= p.N; ++i) {
        result[i] = 0;
    }
    return result;
}

void count_freq(Type t,
               Map& f,
               const Vector& v) {
    for (const auto& i: v) {
        if (f.find(i) == f.end()) {
            f[i] = 1;
        } else {
            f[i]++;
        }
    }
}


template <typename INSERTER>
void intersect(const Vector& A, const Vector& B, INSERTER output) {
    auto it = B.begin();
    for (const auto& a: A) {
        it = std::lower_bound(it, B.end(), a);
        if (it == B.end()) {
            // since there are no values greater or equal the current,
            // there also won't be any for subsequent value (all greater than current)
            return;
        }
        if (*it == a) {
            output = a;
            ++it;
        }
    }
}

int count_hits(Type t,
               const Vector& v,
               const Vector& m) {
    Vector r;
    intersect(v, m, std::back_inserter(r));
    return r.size();
}

void generate_games(const GamesMap& gm) {
    for (const auto& [type, total]: gm) {
        std::cout << titles[type] << '\n';
        for (int i = 0; i < total; ++i) {
            Vector r = generate<mixmax_engine>(type, mixmax_gen, mixmax);
            std::cout << r << '\n';
        }
    }
}

int check_games(Type type,
                 const Matrix& games,
                 const Vector& result) {
    int max = 0;
    for (const auto& g: games) {
        int hits = count_hits(type, g, result);
        if (hits > max) max = hits;
        std::cout << g << " hits: " << hits << "\n";
    }
    std::cout << result << " <- result\n";
    return max;
}

void check_all_games(Type type,
                 const Matrix& games,
                 const Matrix& results) {
    int max = 0;
    for (const auto& r: results) {
        int max_hits = check_games(type, games, r);
        if (max_hits > max) max = max_hits;
    }
    std::cout << "Max hits: " << max << '\n';
}

Pair get_min_max_hits(Type type, const Vector& game, const Matrix& results) {
    Pair p = {params[type].M, 0};
    for (const auto& r: results) {
        int hits = count_hits(type, game, r);
        if (hits < p.first) p.first = hits;
        if (hits > p.second) p.second = hits;
    }
    return p;
}

int get_max_hits(Type type, const Vector& game, const Matrix& results) {
    int result = 0;
    for (const auto& r: results) {
        int hits = count_hits(type, game, r);
        if (hits > result) result = hits;
    }
    return result;
}

int get_min_hits(Type type, const Vector& game, const Matrix& results) {
    int result = params[type].M;
    for (const auto& r: results) {
        int hits = count_hits(type, game, r);
        if (hits < result) result = hits;
    }
    return result;
}


Matrix generate_max_hits(Type type, const Matrix& all, int size, int max_hits) {
    MapH history;
    Matrix result;
    int counter = 0;
    while (counter < size) {
        Vector r = generate<mixmax_engine>(type, mixmax_gen, mixmax);
        if (history.find(r) != history.end()) continue;
        int mh = get_max_hits(type, r, all);
        if (max_hits == mh) {
            std::cout << "max hits: " << r << "\n";
            history[r] = true;
            ++counter;
        }
    }
    return result;
}

Matrix generate_min_hits(Type type, const Matrix& all, int size, int min_hits) {
    MapH history;
    Matrix result;
    int counter = 0;
    while (counter < size) {
        Vector r = generate<mixmax_engine>(type, mixmax_gen, mixmax);
        if (history.find(r) != history.end()) continue;
        int mh = get_min_hits(type, r, all);
        if (min_hits == mh) {
            std::cout << "min hits: " << r << "\n";
            history[r] = true;
            ++counter;
        }
    }
    return result;
}

Matrix generate_min_max_hits(Type type, const Matrix& all, int size, int min_hits, int max_hits) {
    MapH history;
    Matrix result;
    int counter = 0;
    while (counter < size) {
        Vector r = generate<mixmax_engine>(type, mixmax_gen, mixmax);
        if (history.find(r) != history.end()) continue;
        auto [min_, max_] = get_min_max_hits(type, r, all);
        if (min_hits == min_ && max_hits == max_) {
            std::cout << "min_max hits: " << r << "\n";
            history[r] = true;
            ++counter;
        }
    }
    return result;
}

void sort_results(Matrix& m) {
    for (auto& v: m) {
        std::sort(v.begin(), v.end());
    }
}

int main() {
    GamesMap games = {
        {Type::Lotofacil, 100},
        {Type::MegaSena, 2},
        {Type::Quina, 2},
        {Type::Lotomania, 2},
        {Type::Timemania, 2},
        {Type::Duplasena, 2},
    };
   generate_games(games);
   return 0;
}
