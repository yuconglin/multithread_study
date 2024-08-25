#include <iostream>
#include <future>

unsigned long long fibonacci(unsigned long long n) {
	if (n <= 1)
		return 1;
	return fibonacci(n-1) + fibonacci(n-2);
}

int main() {
	std::cout << "Calling fibonacci(44)\n";
	auto result = std::async(fibonacci, 44);
	std::cout << "Calculating result...\n";
	std::cout << result.get() << std::endl;
}