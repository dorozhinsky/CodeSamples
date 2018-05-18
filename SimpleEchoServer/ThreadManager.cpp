/*
 * ThreadManager.cpp
 *
 *  Created on: Nov 15, 2017
 *      Author: dorozhin
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>

#include "ThreadManager.hpp"

static std::list<pthread_t> pthreadIDs;
static pthread_mutex_t pthreadIDsLock = PTHREAD_MUTEX_INITIALIZER;
static __thread pthread_t cachedThreadID;

static void ThreadCleanupFunction(void* args)
{
	// Remove from the list
	pthread_mutex_lock(&pthreadIDsLock);
	pthreadIDs.remove(cachedThreadID);
	pthread_mutex_unlock(&pthreadIDsLock);
	if (NULL != args)
	{
		ThreadExecuteParameters* params = (ThreadExecuteParameters*)args;

		if (NULL != params->cleanupHandler)
			params->cleanupHandler(params->params);

		free(args);
	}
}

static void* ThreadExecutorFunction(void* args)
{
	ThreadExecuteParameters *startParams = (ThreadExecuteParameters*)args;
	void* resultValue = NULL;

	pthread_mutex_lock(&pthreadIDsLock);
	cachedThreadID = pthread_self();
	pthreadIDs.push_back(cachedThreadID);
	pthread_mutex_unlock(&pthreadIDsLock);

	pthread_cleanup_push(ThreadCleanupFunction, args);

	resultValue = startParams->executor(startParams->params);

	pthread_cleanup_pop(1);

	return resultValue;
}

int CreateAndRunThread(ThreadExecuteParameters* startParams, pthread_t* startedThreadId)
{
	pthread_t newThreadId;
	int result = -1;

	if ((NULL == startParams)
			|| (NULL == startParams->executor))
	{
		return EINVAL;
	}

	result = pthread_create(&newThreadId, NULL, &ThreadExecutorFunction, startParams);

	if (NULL != startedThreadId)
		*startedThreadId = newThreadId;

	return result;
}

int CancelThread(const pthread_t threadId)
{
	return pthread_cancel(threadId);
}

void CancelAllRunningThreads(void* params)
{
	(void)params;
	std::list<pthread_t>::iterator itr;
	size_t runningThreadsCount = 0;

	do
	{
		pthread_mutex_lock(&pthreadIDsLock);

		for (runningThreadsCount = 0, itr = pthreadIDs.begin(); itr != pthreadIDs.end(); runningThreadsCount++, itr++)
			CancelThread(*itr);

		pthread_mutex_unlock(&pthreadIDsLock);
	}
	while (runningThreadsCount > 0);
}
