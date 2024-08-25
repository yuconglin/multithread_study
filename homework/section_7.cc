#include <mutex>
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

class some_type { 
	//
public:
  some_type(int index) : index_(index) {}

  void do_it() {
    std::cout << "do it " << index_ << "\n";
  }
private:
  int index_{};
};

std::atomic<some_type*> ptr{nullptr};
std::mutex process_mutex;

void process(int index) {
    if (!ptr) {                     // First check of ptr
        std::lock_guard<std::mutex> lk(process_mutex);
        
        if (!ptr)                  // Second check of ptr
            ptr = new some_type(index);   // Initialize ptr
    }
    some_type *sp = ptr;
    sp->do_it();
}

int main(int argc, char** argv) {
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++ i) {
    threads.push_back(std::thread{process, i});
  }
  for (auto& t : threads) {
    t.join();
  }
}
