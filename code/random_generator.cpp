#include <random>
#define int int64_t

// random generator
std::mt19937 rng(1007);
std::uniform_int_distribution<int> uni(0, 1e12);

signed randint(signed mi, signed ma)
{
    return uni(rng) % (ma - mi) + mi;
}