#pragma once

#include <atomic>

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			namespace Private
			{
				class SpinLock final
				{
				public:
					// Constructors
					SpinLock(void);

					SpinLock(SpinLock const &) = default;

					SpinLock(SpinLock &&) = default;

					// Assignment Operators
					SpinLock &operator=(SpinLock const &) = default;

					SpinLock &operator=(SpinLock &&) = default;

					// Destructor
					~SpinLock(void) = default;

					// Methods
					void lock(void);

					void unlock(void);
					
					bool isLocked(void) const;

				private:
					// Attributes
					std::atomic_bool atomicLock;
				};
			}
		}
	}
}

#include "SpinLock.inl"