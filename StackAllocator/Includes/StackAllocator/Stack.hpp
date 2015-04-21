#pragma once

#include "Private/IStack.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			template <std::size_t Capacity>
			class Stack : public Private::IStack
			{
			public:
				// Constructors
				Stack(void) = default;

				Stack(Stack const &) = delete;

				Stack(Stack &&) = delete;

				// Assignment Operators
				Stack &operator=(Stack const &) = delete;

				Stack &operator=(Stack &&) = delete;

				// Destructor
				virtual ~Stack(void) = default;

				// Methods
				template <typename Type, typename... Args>
				Type *create(Args &&...args);

				template <typename Type>
				void destroy(Type const *instance);

				// Inherited Methods
				bool empty(void) const noexcept override final;

				std::size_t size(void) const noexcept override final;

				std::size_t capacity(void) const noexcept override final;

				virtual void reset(void) noexcept override;

				virtual void *allocate(std::size_t numberOfBytes, std::size_t alignment) override;

				virtual void deallocate(void const *memory, std::size_t numberOfBytes) override;

			private:
				// Static Attributes
				static constexpr const std::size_t Alignment = alignof(std::max_align_t);

				// Attributes
				alignas(Stack<Capacity>::Alignment) std::uint8_t buffer[Capacity];

				void *bufferPointer = buffer;
			};
		}
	}
}

#include "Stack.inl"