/*
 * ThreadManager.hpp
 *
 *  Created on: Nov 15, 2017
 *      Author: dorozhin
 */

#ifndef THREADMANAGER_HPP_
#define THREADMANAGER_HPP_

#include <pthread.h>

typedef void* (*ThreadExecutor)(void* params);
typedef void (*ThreadCleanupHandler)(void* params);

typedef struct ThreadExecuteParameters
{
	void* params;
	ThreadExecutor executor;
	ThreadCleanupHandler cleanupHandler;
} ThreadExecuteParameters;

int CreateAndRunThread(ThreadExecuteParameters* startParams, pthread_t* startedThreadId);
int CancelThread(const pthread_t threadId);
void CancelAllRunningThreads(void* params);


#endif /* THREADMANAGER_HPP_ */
