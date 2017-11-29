#include "util.h"

#include <chrono> // for seeding PRNG with the time
#include <random>

using util::PRNG;

using generator_t = std::mt19937;

static generator_t::result_type getSeed() {
    auto seed = static_cast<generator_t::result_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    seed = 3668391672;
    printf("seed = %u\n", seed);
    return seed;
}

struct PRNG::impl_t {
    generator_t gen{getSeed()};
    std::uniform_real_distribution<double> defaultDist{0.0, 1.0};
};


PRNG::PRNG() {
    impl = new impl_t;
}

PRNG::PRNG(const PRNG& other) {
    impl = new impl_t{*other.impl};
}

PRNG::~PRNG() {
    delete impl;
}

void util::swap(PRNG& a, PRNG& b) {
    using std::swap;
    swap(a.impl, b.impl);
}

double PRNG::operator()() {
    return impl->defaultDist(impl->gen);
}

thread_local PRNG util::rand;