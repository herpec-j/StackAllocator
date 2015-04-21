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
	AO::Allocator::Stack<2 * sizeof(short) + sizeof(int)> fa;
	assert(fa.empty() && fa.size() == 0 && "Allocator should be empty");
	int *const value1 = fa.create<int>(1);
	assert(*value1 == 1 && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == sizeof(int) && "Allocator should contain 1 element");
	short *const value2 = fa.create<short>(2);
	assert(*value2 == 2 && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == sizeof(int) + sizeof(short) && "Allocator should contain 2 elements");
	short *const value3 = fa.create<short>(3);
	assert(*value3 == 3 && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == fa.capacity() && "Allocator should be full");
	int *const value4 = fa.create<int>(4);
	assert(*value4 == 4 && "Object not constructed properly");
	assert(!fa.empty() && fa.size() == fa.capacity() && "Allocator should be full");
	fa.destroy(value4);
	assert(!fa.empty() && fa.size() == fa.capacity() && "Allocator should still be full (because the last element was out of stack bounds)");
	fa.destroy(value3);
	//assert(*value3 == 0 && "Object not destructed properly");
	assert(!fa.empty() && fa.size() == sizeof(int) + sizeof(short) && "Allocator should contain 2 elements");
	fa.destroy(value2);
	//assert(*value2 == 0 && "Object not destructed properly");
	assert(!fa.empty() && fa.size() == sizeof(int) && "Allocator should contain 1 element");
	fa.destroy(value1);
	//assert(*value1 == 0 && "Object not destructed properly");
	assert(fa.empty() && fa.size() == 0 && "Allocator should be empty");
	std::cout << "Success!" << std::endl;
	return 0;
}
