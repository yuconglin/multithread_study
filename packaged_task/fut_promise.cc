#include <future>
#include <iostream>
#include <thread>

using namespace std;

void produce(promise<int>& px, int a, int b) {               // Producer function with promise
    this_thread::sleep_for(1s);
    px.set_value(a+b);				           // Set the result
}

void consume(future<int>& fx) {                // Consumer function with future
    int x = fx.get();				           // Get the result
	cout << "The answer is " << x << endl;
}

int main() {
    promise<int> p;
    future<int> f = p.get_future();
    
    thread fut{consume, std::ref(f)};
    thread prom{produce, std::ref(p), 6, 7};
    
    fut.join();
    prom.join();
}
