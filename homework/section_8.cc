#include <future>
#include <iostream>
#include <numeric>
#include <vector>

// g++ section_8.cc -lpthread  -o section_8

namespace {
constexpr int kLength = 16;  
constexpr int kGroup = 4;

double accum(double *beg, double *end, double init) {
  return std::accumulate(beg, end, init);
}

double SumPackagedTask(double *nums) {
  std::vector<std::future<double>> futures;
  std::vector<std::thread> threads;
  const int inc = (kLength + kGroup - 1) / kGroup;

  for (int k = 0; k < kGroup; ++ k) {
    std::packaged_task<double(double*, double*, double)> ptask(accum); 
    futures.push_back(ptask.get_future());
    std::thread t(std::move(ptask), nums + k * inc, nums + std::min((k + 1) * inc, kLength), 0.0);
    threads.push_back(std::move(t));
  }
  
  double sum = 0.0;
  for (int k = 0; k < kGroup; ++ k) {
    threads[k].join(); 
    sum += futures[k].get();
  }

  return sum;
}

double SumAsync(double *nums) {
  std::vector<std::future<double>> futures;
  const int inc = (kLength + kGroup - 1) / kGroup;

  for (int k = 0; k < kGroup; ++ k) {
    auto res = std::async(std::launch::async, accum, nums + k * inc, nums + std::min((k + 1) * inc, kLength), 0.0);  
    futures.push_back(std::move(res));
  }

  double sum = 0.0;
  for (auto& future : futures) {
    sum += future.get();
  }
  return sum;
}

}

int main(int argc, char** argv) {
  double nums[kLength];
  for (int i = 0; i < kLength; ++ i) {
    nums[i] = i + 1;
  }
  
  std::cout << "final sum from packaged_tasks " << SumPackagedTask(nums) << '\n';
  std::cout << "final sum from async functions " << SumAsync(nums) << '\n';
}

