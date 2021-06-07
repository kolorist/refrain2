#include "TaskManager.h"

#include <clover.h>
#include <floral/math/utils.h>

#include "Globals.h"
#include "Memory.h"

namespace refrain2 {

	TaskManager::TaskManager()
		: pm_RegularWorkersCount(0)
		, m_RegularTaskQueue(&g_TaskAllocator)
	{
		for (u32 i = 0; i < RESTRICTED_THREADS_COUNT; i++) {
			m_RestrictedTaskQueues[i].assign_allocator(&g_TaskAllocator);
		}
	}

	TaskManager::~TaskManager()
	{ }

	void TaskManager::Initialize(const u32 workersCount)
	{
		pm_RegularWorkersCount = floral::min(workersCount, MAX_REGULAR_WORKERS);
		g_TaskManager = this;
		for (u32 i = 0; i < pm_RegularWorkersCount; i++) {
			m_WorkerThreads[i].pm_RefTaskQueue = &m_RegularTaskQueue;
		}
		for (u32 i = 0; i < RESTRICTED_THREADS_COUNT; i++) {
			m_RestrictedThreads[i].pm_RefTaskQueue = &m_RestrictedTaskQueues[i];
		}
	}

	void TaskManager::StartAllTaskingThreads()
	{
		for (u32 i = 0; i < pm_RegularWorkersCount; i++) {
			m_WorkerThreads[i].pm_Thread.start();
			sprintf(m_WorkerThreads[i].pm_Name, "_refrain_WorkerThread_%d_", i);
		}

		for (u32 i = 0; i < RESTRICTED_THREADS_COUNT; i++) {
			m_RestrictedThreads[i].pm_Thread.start();
			sprintf(m_RestrictedThreads[i].pm_Name, "_refrain_RestrictedThread_%d_", i);
		}
	}

	void TaskManager::WaitForAllTaskingThreads()
	{
		for (u32 i = 0; i < pm_RegularWorkersCount; i++) {
			floral::wait_for_thread(m_WorkerThreads[i].pm_Thread);
		}

		for (u32 i = 0; i < RESTRICTED_THREADS_COUNT; i++) {
			floral::wait_for_thread(m_RestrictedThreads[i].pm_Thread);
		}
	}

	void TaskManager::PushTask(Task& newTask)
	{
        if (newTask.pm_RestrictedQueueIdx >= 0)
            m_RestrictedTaskQueues[newTask.pm_RestrictedQueueIdx].push(newTask);
        else m_RegularTaskQueue.push(newTask);
	}

	void TaskManager::PushTask(InstructionType taskInstruction, voidptr data,
			const s32 restrictedIdx /* = NO_RESTRICTED */, const_cstr taskName /* = nullptr */,
            std::atomic<u32>* counter /* = nullptr */)
	{
		Task newTask;
		newTask.pm_Instruction = taskInstruction;
		newTask.pm_Data = data;
		newTask.pm_Counter = counter;
        newTask.pm_RestrictedQueueIdx = restrictedIdx;
		if (taskName)
			strcpy(newTask.pm_Name, taskName);
		else strcpy(newTask.pm_Name, "<empty>");

        PushTask(newTask);
	}

	void TaskManager::PushTasks(InstructionType taskInstruction, voidptr data,
			const u32 tasksCount, const s32 restrictedIdx /* = NO_RESTRICTED */,
            const_cstr taskName /* = nullptr */, std::atomic<u32>* counter /* = nullptr */)
	{
		for (u32 i = 0; i < tasksCount; i++)
			PushTask(taskInstruction, data, restrictedIdx, taskName, counter);
	}

	// -----------------------------------------
	// wait for counter
	void BusyWaitForCounter(std::atomic<u32>& counter, const u32 value)
	{
		while (counter > value);
	}

	void FreeWaitForCounter(std::atomic<u32>& counter, const u32 value)
	{
		while (counter > value);
#if 0
        while (counter > value) {
            // get new job from main thread task queue, non-blocking
			Task t = g_TaskManager->pm_MainThreadTaskQueue.try_pop();
			// do the task
            if (t.pm_Instruction != nullptr) {
                //clover::TracePrint("got the task '%s', doing work in MainThread", t.pm_Name);
                Task nextTask = (*t.pm_Instruction)(t.pm_Data);
                if (t.pm_Counter) {
                    t.pm_Counter->fetch_sub(1);
                    //clover::TracePrint("atomic counter = %d", t.pm_Counter->load());
                }
                //clover::TracePrint("all done, in MainThread.");
            }
        }
#endif
	}

	const bool CheckForCounter(std::atomic<u32>& counter, const u32 value)
	{
		u32 expected = value;
		return (counter.compare_exchange_strong(expected, value));
	}

}
