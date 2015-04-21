#pragma once

#include <limits>

#include "StackAllocator.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			// Constructors
			template <class Type, std::size_t Capacity, class Enable>
			inline StackAllocator<Type, Capacity, Enable>::StackAllocator(void)
				: StackAllocator<Type, Capacity>(std::make_shared<StackType>())
			{
				return;
			}

			template <class Type, std::size_t Capacity, class Enable>
			inline StackAllocator<Type, Capacity, Enable>::StackAllocator(std::shared_ptr<Private::IStack> stack) noexcept
				: stack(stack)
			{
				return;
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline StackAllocator<Type, Capacity, Enable>::StackAllocator(StackAllocator<OtherType, OtherSize> const &other) noexcept
				: stack(other.stack)
			{
				return;
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline StackAllocator<Type, Capacity, Enable>::StackAllocator(StackAllocator<OtherType, OtherSize> &&other) noexcept
				: stack(std::move(other.stack))
			{
				return;
			}

			// Assignment Operators
			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline StackAllocator<Type, Capacity, Enable> &StackAllocator<Type, Capacity, Enable>::operator=(StackAllocator<OtherType, OtherSize> const &other) noexcept
			{
				if (this != &other)
				{
					stack = other.stack;
				}
				return *this;
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherSize>
			inline StackAllocator<Type, Capacity, Enable> &StackAllocator<Type, Capacity, Enable>::operator=(StackAllocator<OtherType, OtherSize> &&other) noexcept
			{
				if (this != &other)
				{
					stack = std::move(other.stack);
				}
				return *this;
			}

			// Methods
			template <class Type, std::size_t Capacity, class Enable>
			inline typename StackAllocator<Type, Capacity, Enable>::pointer StackAllocator<Type, Capacity, Enable>::allocate(size_type numberOfObjects, void const *hint)
			{
				return static_cast<Type *>(stack->allocate(numberOfObjects * sizeof(Type), alignof(Type)));
			}

			template <class Type, std::size_t Capacity, class Enable>
			inline void StackAllocator<Type, Capacity, Enable>::deallocate(const_pointer instance, size_type numberOfObjects)
			{
				stack->deallocate(static_cast<void const *>(instance), numberOfObjects * sizeof(Type));
			}

			template <class Type, std::size_t Capacity, class Enable>
			inline typename StackAllocator<Type, Capacity, Enable>::pointer StackAllocator<Type, Capacity, Enable>::address(reference instance) const noexcept
			{
				return std::addressof(instance);
			}

			template <class Type, std::size_t Capacity, class Enable>
			inline typename StackAllocator<Type, Capacity, Enable>::const_pointer StackAllocator<Type, Capacity, Enable>::address(const_reference instance) const noexcept
			{
				return std::addressof(instance);
			}

			template <class Type, std::size_t Capacity, class Enable>
			inline typename StackAllocator<Type, Capacity, Enable>::size_type StackAllocator<Type, Capacity, Enable>::max_size(void) const noexcept
			{
				return std::numeric_limits<size_type>::max();
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, typename... Args>
			inline void StackAllocator<Type, Capacity, Enable>::construct(OtherType *address, Args &&...args)
			{
				new (static_cast<void *>(address)) OtherType(std::forward<Args>(args)...);
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType>
			inline void StackAllocator<Type, Capacity, Enable>::destroy(OtherType *instance)
			{
				instance->~OtherType();
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherCapacity>
			inline bool StackAllocator<Type, Capacity, Enable>::operator==(StackAllocator<OtherType, OtherCapacity> const &other) const noexcept
			{
				return stack == other.stack;
			}

			template <class Type, std::size_t Capacity, class Enable>
			template <class OtherType, std::size_t OtherCapacity>
			inline bool StackAllocator<Type, Capacity, Enable>::operator!=(StackAllocator<OtherType, OtherCapacity> const &other) const noexcept
			{
				return !(*this == other);
			}
		}
	}
}