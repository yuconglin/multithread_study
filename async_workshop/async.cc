#include <algorithm>
#include <iostream>
#include <vector>
#include <future>
#include "rand.h"

using namespace std;

double accum(double *beg, double *end, double init) {
    return std::accumulate(beg, end, init);
}

int main() {
    vector<double> v(16);
    iota(v.begin(), v.end(), 1.0);
    vector<double> vrand(10'000);
    generate(vrand.begin(), vrand.end(), RandGen());
	//auto v0 = &v[0];
    //auto sz = v.size();
    auto v0 = &vrand[0];
    auto sz = vrand.size();
	
    auto f1 = async(accum, v0, v0 + sz/4, 0.0);
    auto f2 = async(accum, v0 + sz/4, v0 + 2*sz/4, 0.0);
    auto f3 = async(accum, v0 + 2*sz/4, v0 + 3*sz/4, 0.0);
    auto f4 = async(accum, v0 + 3*sz/4, v0 + sz, 0.0);
    cout << f1.get() + f2.get() + f3.get() + f4.get();
}
