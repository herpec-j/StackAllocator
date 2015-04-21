#pragma once

#include <limits>

#include "ThreadSafeStackAllocator.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			// Constructors
			template <class Type, std::size_t Capacity, class Enable>
			inline ThreadSafeStackAllocator<Type, Capacity, Enable>::ThreadSafeStackAllocator(void)
				: StackAllocator<Type, Capacity>(std::make_shared<StackType>())
			{
				return;
			}
			
			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline ThreadSafeStackAllocator<Type, Capacity, Enable>::ThreadSafeStackAllocator(ThreadSafeStackAllocator<OtherType, OtherSize> const &other) noexcept
				: StackAllocator<Type, Capacity>(other.stack)
			{
				return;
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline ThreadSafeStackAllocator<Type, Capacity, Enable>::ThreadSafeStackAllocator(ThreadSafeStackAllocator<OtherType, OtherSize> &&other) noexcept
				: StackAllocator<Type, Capacity>(std::move(other.stack))
			{
				return;
			}
			
			// Assignment Operators
			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline ThreadSafeStackAllocator<Type, Capacity, Enable> &ThreadSafeStackAllocator<Type, Capacity, Enable>::operator=(ThreadSafeStackAllocator<OtherType, OtherSize> const &other) noexcept
			{
				StackAllocator<Type, Capacity>::operator=(other);
				return *this;
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline ThreadSafeStackAllocator<Type, Capacity, Enable> &ThreadSafeStackAllocator<Type, Capacity, Enable>::operator=(ThreadSafeStackAllocator<OtherType, OtherSize> &&other) noexcept
			{
				StackAllocator<Type, Capacity>::operator=(std::move(other));
				return *this;
			}
		}
	}
}