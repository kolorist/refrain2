#include "TaskingThread.h"

#include <clover.h>

#include "Globals.h"
#include "TaskManager.h"

namespace refrain2 {

	void TaskingThreadEntryPoint(voidptr data) {
		TaskingThread* wThread = (TaskingThread*)data;
		clover::Initialize(wThread->pm_Name, clover::LogLevel::Verbose);
		clover::InitializeVSOutput(wThread->pm_Name, clover::LogLevel::Verbose);
		CLOVER_VERBOSE("%s started", wThread->pm_Name);
		//clover::InitializeTracer(0, wThread->pm_Name);
		//clover::TracePrint("begin tasking thread");
		floral::set_current_thread_name(wThread->pm_Name);
		//clover::TracePrint("thread name: %s", wThread->pm_Name);
		//clover::TracePrint("entering thread loop");
		wThread->ThreadLoop();
		//clover::TracePrint("end thread loop");
	}

	TaskingThread::TaskingThread()
		: pm_RefTaskQueue(nullptr)
		, pm_IsRestricted(false)
	{
		memset(pm_Name, 0, sizeof(pm_Name));
		pm_Thread.entry_point = &TaskingThreadEntryPoint;
		pm_Thread.ptr_data = this;
	}

	TaskingThread::~TaskingThread()
	{
	}

	void TaskingThread::ThreadLoop()
	{
		while (true)
		{
			//clover::TracePrint("waiting for task...");
			Task t = pm_RefTaskQueue->wait_and_pop();
			// do the task
			//clover::TracePrint("got the task '%s', doing work", t.pm_Name);
			Task nextTask = (*t.pm_Instruction)(t.pm_Data);
			//clover::TracePrint("task completed, decreasing atomic counter");
			if (t.pm_Counter) {
				t.pm_Counter->fetch_sub(1);
				//clover::TracePrint("atomic counter = %d", t.pm_Counter->load());
			}
			if (nextTask.pm_Instruction) {
				//clover::TracePrint("enqueuing sequence task");
				g_TaskManager->PushTask(nextTask);
			}
			//clover::TracePrint("all done.");
		}
	}
}
