/*
 * main.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: dorozhin
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "SimpleEchoServer.hpp"

// Global variables
static struct sigaction act;

// Signal handler
void sighandler(int signum, siginfo_t *info, void *ptr)
{
	printf("Received signal %d\n", signum);
	printf("Signal originates from process %lu\n",
			(unsigned long)info->si_pid);

	if ((SIGTERM == signum)
			|| (SIGINT == signum))
	{
		printf("Terminating the application...\n");
		ServerShutdown();
	}
}


int main(int argc, char* argv[])
{
	int opt = 0;
	unsigned short port = 22000;

	printf("Simple TCP server. Version 1.0, Build: %s %s\n", __DATE__, __TIME__);

	// Get parameters
	while ((opt = getopt(argc, argv, "hp:")) != -1)
	{
		switch (opt)
		{
		case 'h':
			printf("Usage:\n\t%s [-h] [-p <port_number>]\n", argv[0]);
			exit(EXIT_SUCCESS);
			break;

		case 'p':
			port = atoi(optarg);
			break;

		default: /* '?' */
			fprintf(stderr, "Usage:\n\t %s [-h] [-p <port_number>]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}


	// Configure signals
	memset(&act, 0, sizeof(act));

	act.sa_sigaction = sighandler;
	act.sa_flags = SA_SIGINFO;
	sigfillset(&act.sa_mask);

	// Register signals
	printf("Registering signals...\n");
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGINT, &act, NULL);

	// Wait for the signals and run the server
	return ServerRunner(port);
}


