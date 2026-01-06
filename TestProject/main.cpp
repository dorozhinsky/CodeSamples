/*
 * main.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: dorozhin
 */

#include <iostream>
#include <cstdlib>
#include <thread>
#include <future>
#include <chrono>


int main(int argc, char* argv[])
{
    // Example: producer (worker thread) sets a value via std::promise
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::cout << "Test App was started" << '\n';

    std::thread worker([p = std::move(prom)]() mutable {
        std::cout << "Worker thread is processing..." << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(3));
        p.set_value(123);
    });
    
    std::cout << "Waiting for worker result..." << '\n';
    int result = fut.get();
    std::cout << "Worker returned " << result << '\n';
    worker.join();

    // Lambda-return example: lambda returns the sum of two ints
    auto add = [](int a, int b) -> int { return a + b; };
    std::cout << "Lambda add(2, 3) = " << add(2, 3) << '\n';

    // App termination loop
    for (int i = 10; i >= 0; i--)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Test App is terminating in " << i << " seconds" << '\n';
    }
    std::cout << "Test App has terminated" << '\n';

    return EXIT_SUCCESS;
}


