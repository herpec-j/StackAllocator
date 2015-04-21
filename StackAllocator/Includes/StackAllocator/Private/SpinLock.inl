#pragma once

#include "SpinLock.hpp"

namespace AO
{
	namespace Allocator
	{
		inline namespace Version_1
		{
			namespace Private
			{
				// Constructors
				inline SpinLock::SpinLock(void)
					: atomicLock(false)
				{
					return;
				}

				// Methods
				inline void SpinLock::lock(void)
				{
					while (atomicLock.exchange(true));
				}

				inline void SpinLock::unlock(void)
				{
					atomicLock = false;
				}

				inline bool SpinLock::isLocked(void) const
				{
					return atomicLock;
				}
			}
		}
	}
}