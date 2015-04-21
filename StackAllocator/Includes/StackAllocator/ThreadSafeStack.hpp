#pragma once

#include "Stack.hpp"
#include "Private/SpinLock.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			template <std::size_t Capacity>
			class ThreadSafeStack final : public Stack<Capacity>
			{
			public:
				// Constructors
				ThreadSafeStack(void) = default;

				ThreadSafeStack(ThreadSafeStack const &) = delete;

				ThreadSafeStack(ThreadSafeStack &&) = delete;

				// Assignment Operators
				ThreadSafeStack &operator=(ThreadSafeStack const &) = delete;

				ThreadSafeStack &operator=(ThreadSafeStack &&) = delete;

				// Destructor
				~ThreadSafeStack(void) = default;

				// Inherited Methods
				void reset(void) noexcept override final;

				void *allocate(std::size_t numberOfBytes, std::size_t alignment) override final;

				void deallocate(void const *memory, std::size_t numberOfBytes) override final;

			private:
				// Attributes
				Private::SpinLock lock;
			};
		}
	}
}

#include "ThreadSafeStack.inl"