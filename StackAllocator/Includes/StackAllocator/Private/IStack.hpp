#pragma once

#include <cstddef>
#include <cstdint>

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			namespace Private
			{
				class IStack
				{
				public:
					// Constructors
					IStack(void) = default;

					IStack(IStack const &) = default;

					IStack(IStack &&) = default;

					// Assignment Operators
					IStack &operator=(IStack const &) = default;

					IStack &operator=(IStack &&) = default;

					// Destructor
					virtual ~IStack(void) = default;

					// Virtual Methods
					virtual bool empty(void) const noexcept = 0;

					virtual std::size_t size(void) const noexcept = 0;

					virtual std::size_t capacity(void) const noexcept = 0;

					virtual void reset(void) noexcept = 0;

					virtual void *allocate(std::size_t numberOfBytes, std::size_t alignment) = 0;

					virtual void deallocate(void const *memory, std::size_t numberOfBytes) = 0;
				};
			}
		}
	}
}