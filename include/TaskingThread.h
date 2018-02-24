#ifndef __REFRAIN2_TASKING_THREAD_H__
#define __REFRAIN2_TASKING_THREAD_H__

#include <floral.h>

#include "Task.h"
#include "Memory.h"

namespace refrain2 {

	class TaskingThread {
		public:
			TaskingThread();
			~TaskingThread();

			void								ThreadLoop();

		public:
			c8									pm_Name[128];
			floral::thread						pm_Thread;
			TaskQueueType*						pm_RefTaskQueue;
			bool								pm_IsRestricted;
	};

};

#endif // __REFRAIN2_TASKING_THREAD_H__
