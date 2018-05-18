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

// Gloabal variables
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


int main(void)
{
	printf("Simple TCP server. Version 1.0, Build: %s %s\n", __DATE__, __TIME__);
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
	return ServerRunner(22000);
}


