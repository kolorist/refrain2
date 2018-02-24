#include "MemorySystem.h"

#include <clover.h>
#include <refrain2.h>

namespace helich {
	MemoryManager								g_MemoryManager;
}

namespace clover {
	LinearAllocator								g_LinearAllocator;
}

namespace refrain2 {
	FreelistAllocator							g_TaskAllocator;
	FreelistAllocator							g_TaskDataAllocator;
}
