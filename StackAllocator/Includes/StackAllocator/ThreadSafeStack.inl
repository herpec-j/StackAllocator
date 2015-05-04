#pragma once

#include <mutex>

#include "ThreadSafeStack.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			// Inherited Methods
			template <std::size_t Capacity>
			inline void ThreadSafeStack<Capacity>::reset(void) noexcept
			{
				std::lock_guard<Private::SpinLock> guard(lock);
				Stack<Capacity>::reset();
			}

			template <std::size_t Capacity>
			inline void *ThreadSafeStack<Capacity>::allocate(std::size_t numberOfBytes, std::size_t alignment)
			{
				std::lock_guard<Private::SpinLock> guard(lock);
				return Stack<Capacity>::allocate(numberOfBytes, alignment);
			}

			template <std::size_t Capacity>
			inline void ThreadSafeStack<Capacity>::deallocate(void const *memory, std::size_t numberOfBytes)
			{
				std::lock_guard<Private::SpinLock> guard(lock);
				Stack<Capacity>::deallocate(memory, numberOfBytes);
			}
		}
	}
}