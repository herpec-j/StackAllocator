#pragma once

#include <thread>

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
					while (true)
					{
						while (atomicLock);
						if (!atomicLock.exchange(true))
						{
							return;
						}
						std::this_thread::yield();
					}
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