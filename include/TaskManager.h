#ifndef __REFRAIN2_TASK_MANAGER_H__
#define __REFRAIN2_TASK_MANAGER_H__

#include <floral.h>
#include <atomic>

#include "Task.h"
#include "TaskingThread.h"

namespace refrain2 {

#define RESTRICTED_THREADS_COUNT				1
static constexpr u32 MAX_REGULAR_WORKERS = 8;

#define NO_RESTRICTED							-1

	class TaskManager {
		public:
			TaskManager();
			~TaskManager();

			void								Initialize(const u32 workersCount);
			void								StartAllTaskingThreads();
			void								WaitForAllTaskingThreads();

			void								PushTask(Task& newTask);
			void								PushTask(InstructionType taskIntruction, voidptr data,
													const s32 restrictedIdx = NO_RESTRICTED, const_cstr taskName = nullptr,
													std::atomic<u32>* counter = nullptr);
			void								PushTasks(InstructionType taskInstruction, voidptr data,
													const u32 tasksCount, const s32 restrictedIdx = NO_RESTRICTED,
													const_cstr taskName = nullptr, std::atomic<u32>* counter = nullptr);

		public:
			u32									pm_RegularWorkersCount;

		private:
			TaskingThread						m_RestrictedThreads[RESTRICTED_THREADS_COUNT];
			TaskingThread						m_WorkerThreads[MAX_REGULAR_WORKERS];

			TaskQueueType						m_RegularTaskQueue;
			TaskQueueType						m_RestrictedTaskQueues[RESTRICTED_THREADS_COUNT];
	};

	// -----------------------------------------
	// wait for counter
	// TODO: only Orchestral thread can use those function for now,
	// 		we have to make it usable for TaskingThread as well
	void										BusyWaitForCounter(std::atomic<u32>& counter, const u32 value);
	void										FreeWaitForCounter(std::atomic<u32>& counter, const u32 value);
	const bool									CheckForCounter(std::atomic<u32>& counter, const u32 value);
	// -----------------------------------------

}

#endif // __REFRAIN2_TASK_MANAGER_H__
