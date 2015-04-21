#pragma once

#include <type_traits>

#include "Stack.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			template <class Type, std::size_t Capacity, class Enable>
			class ThreadSafeStackAllocator;

			template <class Type, std::size_t Capacity = 256, class Enable = typename std::enable_if<!std::is_same<Type, void>::value && Capacity != 0>::type>
			class StackAllocator
			{
				// Friendships
				template <class OtherType, std::size_t OtherCapacity, class Enable>
				friend class StackAllocator;

				template <class OtherType, std::size_t OtherCapacity, class Enable>
				friend class ThreadSafeStackAllocator;

			public:
				// Type Aliases
				using StackType = Stack<sizeof(Type) * Capacity>;

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
					using other = StackAllocator<OtherType, Capacity>;
				};

				// Constructors
				StackAllocator(void);

				StackAllocator(StackAllocator const &) noexcept = default;
				
				StackAllocator(StackAllocator &&) noexcept = default;

				template <class OtherType, std::size_t OtherSize>
				StackAllocator(StackAllocator<OtherType, OtherSize> const &other) noexcept;

				template <class OtherType, std::size_t OtherSize>
				StackAllocator(StackAllocator<OtherType, OtherSize> &&other) noexcept;

				// Assignment Operators
				StackAllocator &operator=(StackAllocator const &) noexcept = default;

				StackAllocator &operator=(StackAllocator &&) noexcept = default;

				template <class OtherType, std::size_t OtherSize>
				StackAllocator &operator=(StackAllocator<OtherType, OtherSize> const &other) noexcept;

				template <class OtherType, std::size_t OtherSize>
				StackAllocator &operator=(StackAllocator<OtherType, OtherSize> &&other) noexcept;

				// Destructor
				virtual ~StackAllocator(void) = default;

				// Methods
				pointer allocate(size_type numberOfObjects, void const *hint = nullptr);

				void deallocate(const_pointer instance, size_type numberOfObjects);

				pointer address(reference instance) const noexcept;

				const_pointer address(const_reference instance) const noexcept;

				size_type max_size(void) const noexcept;

				template <class OtherType, typename... Args>
				void construct(OtherType *address, Args &&...args);

				template <class OtherType>
				void destroy(OtherType *instance);

				template <class OtherType, std::size_t OtherCapacity>
				bool operator==(StackAllocator<OtherType, OtherCapacity> const &other) const noexcept;

				template <class OtherType, std::size_t OtherCapacity>
				bool operator!=(StackAllocator<OtherType, OtherCapacity> const &other) const noexcept;

			protected:
				// Constructors
				StackAllocator(std::shared_ptr<Private::IStack> stack) noexcept;

			private:
				// Attributes
				std::shared_ptr<Private::IStack> stack;
			};
		}
	}
}

#include "StackAllocator.inl"