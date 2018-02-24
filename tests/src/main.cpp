#include <stdio.h>

#include "MemorySystem.h"
#include <clover.h>
#include <refrain2.h>

refrain2::Task Task_SingleTaskNoData(voidptr data)
{
	CLOVER_VERBOSE("Task_SingleTask no data");
	return refrain2::Task();
}

refrain2::Task Task_SequenceTaskNoDataSecond(voidptr data)
{
	CLOVER_VERBOSE("Task_SequenceTaskNoData second task");
	return refrain2::Task();
}

refrain2::Task Task_SequenceTaskNoData(voidptr data)
{
	CLOVER_VERBOSE("Task_SequenceTaskNoData first task");

	refrain2::Task newTask;
	newTask.pm_Instruction = &Task_SequenceTaskNoDataSecond;
	newTask.pm_RestrictedQueueIdx = NO_RESTRICTED;
	strcpy(newTask.pm_Name, "sequence_second");
	return newTask;
}

refrain2::Task Task_SequenceTaskNoDataRestrictedSecond(voidptr data)
{
	CLOVER_VERBOSE("Task_SequenceTaskNoDataRestrictedSecond second task");
	return refrain2::Task();
}

refrain2::Task Task_SequenceTaskNoDataRegular(voidptr data)
{
	CLOVER_VERBOSE("Task_SequenceTaskNoDataRegular first task");

	refrain2::Task newTask;
	newTask.pm_Instruction = &Task_SequenceTaskNoDataRestrictedSecond;
	newTask.pm_RestrictedQueueIdx = 0;
	strcpy(newTask.pm_Name, "sequence_second");
	return newTask;
}

int main(void)
{
	using namespace helich;
	g_MemoryManager.Initialize(
		MemoryRegion<clover::LinearAllocator> { "clover/allocator", SIZE_MB(16), &clover::g_LinearAllocator },
		MemoryRegion<refrain2::FreelistAllocator> { "refrain2/task", SIZE_MB(16), &refrain2::g_TaskAllocator },
		MemoryRegion<refrain2::FreelistAllocator> { "refrain2/taskdata", SIZE_MB(32), &refrain2::g_TaskDataAllocator }
		);

	clover::InitializeVSOutput("vs2017", clover::LogLevel::Verbose);

	CLOVER_VERBOSE("refrain2 unit testing");

	refrain2::TaskManager taskMgr;

	taskMgr.Initialize(2);
	taskMgr.StartAllTaskingThreads();

	Sleep(100);

	taskMgr.PushTask(&Task_SingleTaskNoData, nullptr, NO_RESTRICTED, "single_nodata");
	taskMgr.PushTask(&Task_SequenceTaskNoData, nullptr, NO_RESTRICTED, "sequence_first");
	taskMgr.PushTask(&Task_SequenceTaskNoDataRegular, nullptr, NO_RESTRICTED, "sequence_first_res");
	taskMgr.PushTask(&Task_SequenceTaskNoDataRestrictedSecond, nullptr, 0, "single_res");

	Sleep(100);

	clover::PrintAllTraces();

	taskMgr.WaitForAllTaskingThreads();

	return 0;
}
