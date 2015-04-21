#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "StackAllocator/ThreadSafeStackAllocator.hpp"

constexpr std::size_t size = 10000000;

static float testClassic(void)
{
	auto const begin = std::chrono::high_resolution_clock::now();
	for (std::size_t i = 0; i < size; ++i)
	{
		double const *v1 = new double(1.0);
		int const *v2 = new int(2);
		std::string const *v3 = new std::string("3");
		delete v3;
		delete v2;
		delete v1;
	}
	auto const end = std::chrono::high_resolution_clock::now();
	float const duration = std::chrono::duration_cast<std::chrono::duration<float, std::chrono::seconds::period>>(end - begin).count();
	std::cout << "Classic:               " << duration << "s" << std::endl;
	return duration;
}

static float testThreadSafeAllocator(void)
{
	AO::Allocator::ThreadSafeStack<sizeof(double) + sizeof(int) + sizeof(std::string)> stack;
	auto const begin = std::chrono::high_resolution_clock::now();
	for (std::size_t i = 0; i < size; ++i)
	{
		double const *v1 = stack.create<double>(1.0);
		int const *v2 = stack.create<int>(2);
		std::string const *v3 = stack.create<std::string>("3");
		stack.destroy(v3);
		stack.destroy(v2);
		stack.destroy(v1);
	}
	auto const end = std::chrono::high_resolution_clock::now();
	float const duration = std::chrono::duration_cast<std::chrono::duration<float, std::chrono::seconds::period>>(end - begin).count();
	std::cout << "Thread-Safe Allocator: " << duration << "s" << std::endl;
	return duration;
}

static float testUnsafeAllocator(void)
{
	AO::Allocator::Stack<sizeof(double) + sizeof(int) + sizeof(std::string)> stack;
	auto const begin = std::chrono::high_resolution_clock::now();
	for (std::size_t i = 0; i < size; ++i)
	{
		double const *v1 = stack.create<double>(1.0);
		int const *v2 = stack.create<int>(2);
		std::string const *v3 = stack.create<std::string>("3");
		stack.destroy(v3);
		stack.destroy(v2);
		stack.destroy(v1);
	}
	auto const end = std::chrono::high_resolution_clock::now();
	float const duration = std::chrono::duration_cast<std::chrono::duration<float, std::chrono::seconds::period>>(end - begin).count();
	std::cout << "Unsafe Allocator:      " << duration << "s" << std::endl;
	return duration;
}

int main(void)
{
	std::vector<int, AO::Allocator::StackAllocator<int>> sv;
	std::vector<int, AO::Allocator::ThreadSafeStackAllocator<int>> tssv;
	float const classicDuration = testClassic();
	float const threadSafeDuration = testThreadSafeAllocator();
	float const unsafeDuration = testUnsafeAllocator();
	assert(unsafeDuration < threadSafeDuration && threadSafeDuration < classicDuration && "Allocator is too slow");
	AO::Allocator::Stack<2 * sizeof(float)> fa;
	assert(fa.empty() && fa.size() == 0 && "Allocator should be empty");
	float *const value1 = fa.create<float>(1.0f);
	assert(*value1 == 1.0f && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == 1 && "Allocator should contain 1 element");
	float *const value2 = fa.create<float>(2.0f);
	assert(*value2 == 2.0f && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == 2 && "Allocator should contain 2 elements");
	float *const value3 = fa.create<float>(3.0f);
	assert(*value3 == 3.0f && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == 2 && "Allocator should contain 2 elements (because the last float was out of stack bounds)");
	fa.destroy(value3);
	assert(!fa.empty() && fa.size() == 2 && "Allocator should contain 2 elements (because the last float was out of stack bounds)");
	fa.destroy(value2);
	//assert(*value2 == 0.0f && "Object not destructed properly");
	assert(!fa.empty() && fa.size() == 1 && "Allocator should contain 1 element");
	fa.destroy(value1);
	//assert(*value1 == 0.0f && "Object not destructed properly");
	assert(fa.empty() && fa.size() == 0 && "Allocator should be empty");
	std::cout << "Success!" << std::endl;
	return 0;
}
