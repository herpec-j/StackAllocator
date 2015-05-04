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
				return bufferSize;
			}

			template <std::size_t Capacity>
			inline std::size_t Stack<Capacity>::capacity(void) const noexcept
			{
				return Capacity;
			}

			template <std::size_t Capacity>
			inline void Stack<Capacity>::reset(void) noexcept
			{
				bufferSize = 0;
			}

			template <std::size_t Capacity>
			inline void *Stack<Capacity>::allocate(std::size_t numberOfBytes, std::size_t alignment)
			{
				std::size_t space = capacity() - size();
				void *bufferPointer = &buffer[bufferSize];
				if (std::align(alignment, numberOfBytes, bufferPointer, space))
				{
					bufferSize = static_cast<std::size_t>(static_cast<StorageType const *>(bufferPointer) - &buffer[0]) + numberOfBytes;
					return bufferPointer;
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
				StorageType const *beginOfMemory = static_cast<StorageType const *>(memory);
				StorageType const *endOfMemory = beginOfMemory + numberOfBytes;
				if (&buffer[0] <= memory && endOfMemory <= &buffer[0] + Capacity)
				{
					assert(static_cast<std::size_t>(endOfMemory - &buffer[0]) == bufferSize && "Invalid memory");
					if (static_cast<std::size_t>(endOfMemory - &buffer[0]) == bufferSize)
					{
						bufferSize = static_cast<std::size_t>(beginOfMemory - &buffer[0]);
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
