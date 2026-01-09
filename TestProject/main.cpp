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
#include <memory>


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

    // std::weak_ptr example: non-owning observer of shared_ptr
    {
        std::cout << "\n--- std::weak_ptr demo ---" << '\n';
        auto sp = std::make_shared<int>(42);
        std::weak_ptr<int> wp = sp;  // observe, not own
        std::cout << "shared_ptr use_count = " << sp.use_count() << '\n';
        if (auto locked = wp.lock()) {
            std::cout << "Locked value = " << *locked << '\n';
            std::cout << "shared_ptr use_count = " << sp.use_count() << '\n';
        } else {
            std::cout << "Object expired" << '\n';
        }
        sp.reset(); // destroy the managed object
        std::cout << "After resetting shared_ptr, expired = " << std::boolalpha << wp.expired() << '\n';
        if (auto locked = wp.lock()) {
            std::cout << "Locked value = " << *locked << '\n';
            std::cout << "shared_ptr use_count = " << sp.use_count() << '\n';
        } else {
            std::cout << "shared_ptr use_count = " << sp.use_count() << '\n';
            std::cout << "Object expired now" << '\n';
        }
        std::cout << "--- end demo ---\n\n";
    }

    // App termination loop
    for (int i = 5; i >= 0; i--)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Test App is terminating in " << i << " seconds" << '\n';
    }
    std::cout << "Test App has terminated" << '\n';

    return EXIT_SUCCESS;
}


