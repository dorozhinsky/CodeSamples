/*
 * main.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: dorozhin
 */

#include <iostream>
#include <cstdlib>


int main(int argc, char* argv[])
{
    std::cout << "Test App was started \a" << std::endl;

    // App termination loop
    for (int i = 10; i >= 0; i--)
    {
        int status = system("sleep 1");

        if (0 != status)
        {
            std::cout << "ERROR: system call returned non-zero status = " << status << std::endl;
            return status;
        }

    std::cout << "Test App is terminating in " << i << " seconds      " << std::endl;
    }
    std::cout << "Test App has terminated\a" << std::endl;

    return EXIT_SUCCESS;
}


