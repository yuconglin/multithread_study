#include <future>
#include <iostream>

// Callable object - thread entry point
void hello() {
    std::cout << "Hello, Async!\n";
}

int main() {
    std::async(hello);        // Create an instance of std::async and initialize it with the task
}
