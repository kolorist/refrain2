#ifndef __REFRAIN2_TASK_H__
#define __REFRAIN2_TASK_H__

#include <floral.h>
#include <atomic>

#include "Memory.h"

namespace refrain2 {

	struct Task;
	typedef Task								(*InstructionType)(voidptr data);

	// this struct is copyable
	// NOTE: it's fun that this copy operation operate just like memcpy()
	// 	so raw fixed sized arrays are also being copied normally
	struct Task {
		Task()
			: pm_Instruction(nullptr)
			, pm_Data(nullptr)
			, pm_Counter(nullptr)
			, pm_RestrictedQueueIdx(-1)
		{ }

		// FIXME: remove pm_Name in Release build
		c8										pm_Name[256];
		InstructionType							pm_Instruction;
		voidptr									pm_Data;
		std::atomic_uint32_t*					pm_Counter;
		s32										pm_RestrictedQueueIdx;
	};

	typedef floral::queue_mt_lockbased<Task, FreelistAllocator>	TaskQueueType;

}

#endif // __REFRAIN2_TASK_H__
