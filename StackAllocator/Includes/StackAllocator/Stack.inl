#pragma once

#include <utility>
#include <cassert>
#include <mutex>

#include "Stack.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			// Methods
			template <std::size_t Capacity>
			template <typename Type, typename... Args>
			inline Type *Stack<Capacity>::create(Args &&...args)
			{
				return new (allocate(sizeof(Type), alignof(Type))) Type(std::forward<Args>(args)...);
			}

			template <std::size_t Capacity>
			template <typename Type>
			inline void Stack<Capacity>::destroy(Type const *instance)
			{
				if (instance)
				{
					instance->~Type();
					deallocate(static_cast<void const *>(instance), sizeof(Type));
				}
			}

			// Inherited Methods
			template <std::size_t Capacity>
			inline bool Stack<Capacity>::empty(void) const noexcept
			{
				return size() == 0;
			}

			template <std::size_t Capacity>
			inline std::size_t Stack<Capacity>::size(void) const noexcept
			{
				return static_cast<std::uint8_t *>(bufferPointer) - buffer;
			}

			template <std::size_t Capacity>
			inline std::size_t Stack<Capacity>::capacity(void) const noexcept
			{
				return Capacity;
			}

			template <std::size_t Capacity>
			inline void Stack<Capacity>::reset(void) noexcept
			{
				bufferPointer = static_cast<void *>(buffer);
			}

			template <std::size_t Capacity>
			inline void *Stack<Capacity>::allocate(std::size_t numberOfBytes, std::size_t alignment)
			{
				std::size_t space = capacity() - size();
				if (std::align(alignment, numberOfBytes, bufferPointer, space))
				{
					void *allocatedMemory = bufferPointer;
					bufferPointer = static_cast<void *>(static_cast<std::uint8_t *>(bufferPointer) + numberOfBytes);
					return allocatedMemory;
				}
				else
				{
					void *allocatedMemory = std::malloc(numberOfBytes + alignment + sizeof(void *));
					void **pointerToAlignedMemory = reinterpret_cast<void **>(reinterpret_cast<std::uintptr_t>(static_cast<std::uint8_t *>(allocatedMemory) + alignment + sizeof(void *)) & ~(alignment - 1));
					pointerToAlignedMemory[-1] = allocatedMemory;
					return static_cast<void *>(pointerToAlignedMemory);
				}
			}

			template <std::size_t Capacity>
			inline void Stack<Capacity>::deallocate(void const *memory, std::size_t numberOfBytes)
			{
				std::uint8_t const *endOfMemory = static_cast<std::uint8_t const *>(memory) + numberOfBytes;
				if (buffer <= memory && endOfMemory <= buffer + Capacity)
				{
					assert(endOfMemory == bufferPointer && "Invalid memory");
					if (endOfMemory == bufferPointer)
					{
						bufferPointer = const_cast<void *>(memory);
					}
				}
				else
				{
					std::free(static_cast<void **>(const_cast<void *>(memory))[-1]);
				}
			}
		}
	}
}
