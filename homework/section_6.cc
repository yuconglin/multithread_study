#include <iostream>
#include <thread>
#include <mutex>
#include <string>

using namespace std::chrono_literals;

std::mutex mut;
std::string sdata{"Empty"};
bool ready{};

void reader() {
  std::cout << "Reading data ..... \n";
  while (true) {
    std::lock_guard<std::mutex> guard(mut);
    if (ready) {
      std::cout << "Data is: " << sdata << '\n';
      break;
    }
  }
  std::this_thread::sleep_for(20ms);
}

void writer() {
  std::cout << "Writing data ..... \n";
  {
    std::lock_guard<std::mutex> lg(mut);
    std::this_thread::sleep_for(2s);
    sdata = "Populated";
    ready = true;
  }
}

int main(int argc, char** argv) {
  std::thread read{reader};
  std::thread write{writer};
  write.join();
  read.join();
}
