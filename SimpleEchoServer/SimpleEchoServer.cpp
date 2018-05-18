//============================================================================
// Name        : SimpleEchoServer.cpp
// Author      : Dorozhinsky
// Version     :
// Copyright   : GNU/GPL
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include<netinet/in.h>
#include <arpa/inet.h>

#include "ThreadManager.hpp"

static volatile int shutdownFlag = 0;

void ServerWorkerCleanup(void* params)
{
	if (NULL != params)
	{
		const char* msg = "Good by!\n";
		int comm_fd = *((int*)params);

		write(comm_fd, msg, strlen(msg));
		close(comm_fd);
		free(params);
		printf("INF: comm_fd %d was closed\n", comm_fd);
	}
}

void* ServerWorker(void* params)
{
	char str[100];
	int comm_fd = 0;

	if (NULL == params)
	{
		perror("ERR: invalid input parameters");
		return NULL;
	}

	comm_fd = *((int*)params);

	snprintf(str, sizeof(str), "Welcome to TCP simple server, ID=%lu\n>", pthread_self());
	write(comm_fd, str, strlen(str)+1);

	while(1)
	{
		bzero( str, sizeof(str));
		if ((0 > read(comm_fd,str,sizeof(str))) || (0 == strncmp(str, "quit", 4)))
			break;

		printf("INF: Echoing back - %s\n",str);
		write(comm_fd, str, strlen(str)+1);
		write(comm_fd, (char*)"\n>", 2);
	}

	return NULL;
}


int ServerRunner(const unsigned short listenPort)
{
	int listen_fd = -1;
	struct sockaddr_in servaddr;

	printf("INF: Starting TCP server...\n");

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		perror("socket failed");
		return EXIT_FAILURE;
	}
	printf("INF: server listen socket #%d was created\n", listen_fd);

	bzero( &servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(listenPort);

	if (0 != bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)))
	{
		perror("bind failed");
		return EXIT_FAILURE;
	}
	printf("INF: bind socket 0x%x:%u\n", INADDR_ANY, listenPort);

	if (0 != listen(listen_fd, SOMAXCONN))
	{
		perror("listen failed");
		return EXIT_FAILURE;
	}
	printf("INF: listen socket 0x%x:%u\n", INADDR_ANY, listenPort);

	// Setup cleanup handlers
	pthread_cleanup_push(CancelAllRunningThreads, NULL);

	while (!shutdownFlag)
	{
		struct sockaddr_in client;
		socklen_t clientSize = sizeof(client);
		ThreadExecuteParameters *threadParams;
		int comm_fd = -1;

		bzero(&client, clientSize);
		comm_fd = accept(listen_fd, (struct sockaddr*)&client, &clientSize);

		if (comm_fd < 0)
		{
			perror("accept failed");
			continue;
		}
		printf("INF: accepted connection on the socket fd %d\n", comm_fd);
		printf("INF: Connected to %s:%u\n", inet_ntoa(client.sin_addr),
				ntohs(client.sin_port));

		threadParams = (ThreadExecuteParameters*)malloc(sizeof(ThreadExecuteParameters));
		if (NULL == threadParams)
		{
			const char* errMsg = "Server error!!!\n";

			perror("Unable to alloc thread memory");
			write(comm_fd, (void*)errMsg, strlen(errMsg));
			close(comm_fd);
			continue;
		}

		threadParams->cleanupHandler = ServerWorkerCleanup;
		threadParams->executor = ServerWorker;
		threadParams->params = malloc(sizeof(int));

		if (NULL == threadParams->params)
		{
			const char* errMsg = "Server error!!!\n";

			perror("Unable to alloc thread memory");
			write(comm_fd, (void*)errMsg, strlen(errMsg));
			close(comm_fd);
			free(threadParams);

			continue;
		}

		*((int*)threadParams->params) = comm_fd;
		if (0 != CreateAndRunThread(threadParams, NULL))
		{
			const char* errMsg = "Server error!!!\n";

			perror("Unable to start a worker thread\n");
			write(comm_fd, (void*)errMsg, strlen(errMsg));
			close(comm_fd);
			free(threadParams->params);
			free(threadParams);

			continue;
		}
		// Do something else here.
	}

	printf("INF: shutdown the server...\n");
	pthread_cleanup_pop(1);

	close(listen_fd);
	printf("INF: listen socket fd was closed\n");

	printf("INF: server exit\n");

	return EXIT_SUCCESS;
}

void ServerShutdown(void)
{
	shutdownFlag = 1;
}
