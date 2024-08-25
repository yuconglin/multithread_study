#include <future>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex m;

void produce(promise<int>& px) {               // Producer function with promise
    int x{42};          
    this_thread::sleep_for(1s);
	cout << "Promise sets shared state to " << x << endl;
    px.set_value(x);				           // Set the result
}

void consume(shared_future<int>& fx) {         // Consumer function with future
	cout << "Thread " << this_thread::get_id() << " calling get()..." << endl;
    int x = fx.get();				           // Get the result
	lock_guard<mutex> lg{m};
    cout << "Thread " << this_thread::get_id() << " returns from calling get()" << endl;
	cout << "Thread " << this_thread::get_id() << " has answer " << x << endl;
}

int main() {
    promise<int> p;
    shared_future<int> sf1(p.get_future());   // Move p's future into a shared future
    shared_future<int> sf2 = sf1;                     // Create a separate instance of shared future

    thread fut1{consume, std::ref(sf1)};         // Start two consumer threads, each with
    thread fut2{consume, std::ref(sf2)};         // its own shared future instance
    thread prom{produce, std::ref(p)};           // Start the producer thread

    fut1.join();
    fut2.join();
    prom.join();
}