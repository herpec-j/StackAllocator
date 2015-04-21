#pragma once

#include "StackAllocator.hpp"
#include "ThreadSafeStack.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			template <class Type, std::size_t Capacity = 256, class Enable = typename std::enable_if<!std::is_same<Type, void>::value && Capacity != 0>::type>
			class ThreadSafeStackAllocator : public StackAllocator<Type, Capacity>
			{
				// Friendships
				template <class OtherType, std::size_t OtherCapacity, class OtherEnable>
				friend class ThreadSafeStackAllocator;

				template <class OtherType, std::size_t OtherCapacity, class OtherEnable>
				friend class StackAllocator;

			public:
				// Type Aliases
				using StackType = ThreadSafeStack<sizeof(Type) * Capacity>;

				using value_type = Type;

				using pointer = Type *;

				using const_pointer = Type const *;

				using reference = Type &;

				using const_reference = Type const &;

				using size_type = std::size_t;

				using difference_type = std::ptrdiff_t;

				template <class OtherType>
				struct rebind
				{
					using other = ThreadSafeStackAllocator<OtherType, Capacity>;
				};

				// Constructors
				ThreadSafeStackAllocator(void);

				ThreadSafeStackAllocator(ThreadSafeStackAllocator const &) noexcept = default;

				ThreadSafeStackAllocator(ThreadSafeStackAllocator &&) noexcept = default;

				template <class OtherType, std::size_t OtherSize>
				ThreadSafeStackAllocator(ThreadSafeStackAllocator<OtherType, OtherSize> const &other) noexcept;

				template <class OtherType, std::size_t OtherSize>
				ThreadSafeStackAllocator(ThreadSafeStackAllocator<OtherType, OtherSize> &&other) noexcept;

				// Assignment Operators
				ThreadSafeStackAllocator &operator=(ThreadSafeStackAllocator const &) noexcept = default;

				ThreadSafeStackAllocator &operator=(ThreadSafeStackAllocator &&) noexcept = default;

				template <class OtherType, std::size_t OtherSize>
				ThreadSafeStackAllocator &operator=(ThreadSafeStackAllocator<OtherType, OtherSize> const &other) noexcept;

				template <class OtherType, std::size_t OtherSize>
				ThreadSafeStackAllocator &operator=(ThreadSafeStackAllocator<OtherType, OtherSize> &&other) noexcept;

				// Destructor
				virtual ~ThreadSafeStackAllocator(void) = default;
			};
		}
	}
}

#include "ThreadSafeStackAllocator.inl"
