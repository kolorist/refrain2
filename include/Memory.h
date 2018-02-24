#ifndef __REFRAIN2_MEMORY_H__
#define __REFRAIN2_MEMORY_H__

#include <floral.h>
#include <helich.h>

namespace refrain2 {
	typedef helich::Allocator<helich::FreelistScheme, helich::NoTrackingPolicy>	FreelistAllocator;

	extern FreelistAllocator					g_TaskAllocator;
	extern FreelistAllocator					g_TaskDataAllocator;
}

#endif // __REFRAIN2_MEMORY_H__
