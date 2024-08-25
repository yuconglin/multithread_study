#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <future>
#include "rand.h"

double accum(double *beg, double *end, double init) {
    return std::accumulate(beg, end, init);
}

int main() {
	std::vector<double> v(16);
	std::iota(v.begin(), v.end(), 1.0);
	std::vector<double> vrand(10'000);
	std::generate(vrand.begin(), vrand.end(), RandGen());
	
	auto v0 = &v[0];
	auto sz = v.size();
	//auto v0 = &vrand[0];
	//auto sz = vrand.size();
	
	using task_type = double(double *, double *, double);
	std::packaged_task<task_type> pt1{accum};
	std::packaged_task<task_type> pt2{accum};
	std::packaged_task<task_type> pt3{accum};
	std::packaged_task<task_type> pt4{accum};
	
	auto f1{pt1.get_future()};
	auto f2{pt2.get_future()};
	auto f3{pt3.get_future()};
	auto f4{pt4.get_future()};
	
	std::thread t1{std::move(pt1), v0, v0 + sz/4, 0};
	std::thread t2{std::move(pt2), v0 + sz/4, v0 + 2*sz/4, 0};
	std::thread t3{std::move(pt3), v0 + 2*sz/4, v0 + 3*sz/4, 0};
	std::thread t4{std::move(pt4), v0 + 3*sz/4, v0 + sz, 0};
	
	t1.join(); t2.join(); t3.join(); t4.join();
	std::cout << f1.get() + f2.get() + f3.get() + f4.get();
}