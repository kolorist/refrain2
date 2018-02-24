A job scheduler using fiber or by the normal way.

> 2 types of task queue: RestrictedTaskQueue, RegularTaskQueue.

> Only 1 primary thread, controls everything and maybe become a worker thread: OrchestralThread
	>> Pushing tasks, control program flows.
	>> 2 kind of waitings:
		>>> BusyWaitForCounter (only waiting, wont do anything else)
		>>> FreeWaitForCounter (when waiting, OrchestralThread may do tasks from RegularTaskQueue)
> Restricted threads, doing special works, maybe become a worker thread: RestrictedThread
	>> Hold important contexts: OpenGL drawing context, OpenGL streaming context...
	>> Consuming tasks from RestrictedTaskQueue.
> Worker threads, doing normal works: WorkerThread
	>> Consuming tasks from RegularTaskQueue.