#include <random>

std::mt19937 mt;
std::uniform_real_distribution<double> dist(1.0, 10'000.0);

class RandGen {
public:
    RandGen()  {}
    double operator()() { return dist(mt); }
};
