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
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "unordered_map_example.hpp"


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

    // std::remove and std::erase example
    {
        std::cout << "\n--- std::remove and std::erase demo ---" << '\n';
        std::vector<std::string> names = {"Alice", "Bob", "Alice", "Carol"};
        std::cout << "Original names: ";
        for (const auto& name : names) {
            std::cout << name << " ";
        }
        std::cout << '\n';

        // std::remove shifts all elements not equal to "Alice" to the front,
        // then returns the new logical end of the remaining range.
        // It does not actually change container size, so we call erase()
        // to remove the trailing elements left behind.
        // Complexity:
        // - std::vector, std::deque: O(N) moves + O(N) erase shift => overall O(N)
        // - std::list, std::forward_list: O(N) element moves and O(N) erase => overall O(N)
        // - applicable to sequence containers with mutable element access (vector, deque, list, forward_list, string)
        names.erase(std::remove(names.begin(), names.end(), "Alice"), names.end());
        std::cout << "After std::remove + erase: ";
        for (const auto& name : names) {
            std::cout << name << " ";
        }
        std::cout << '\n';

        std::vector<int> values = {1, 2, 3, 2, 4};
        std::cout << "Original values: ";
        for (int value : values) {
            std::cout << value << " ";
        }
        std::cout << '\n';

        values.erase(std::remove(values.begin(), values.end(), 2), values.end());
        std::cout << "After erase-remove idiom: ";
        for (int value : values) {
            std::cout << value << " ";
        }
        std::cout << '\n';

        std::list<std::string> nameList = {"Alice", "Bob", "Alice", "Carol"};
        std::cout << "Original list values: ";
        for (const auto& name : nameList) {
            std::cout << name << " ";
        }
        std::cout << '\n';

        // std::list::remove removes elements equal to the given value directly.
        // It works in-place by unlinking matching nodes, without shifting elements.
        nameList.remove("Alice");
        std::cout << "After list.remove(\"Alice\"): ";
        for (const auto& name : nameList) {
            std::cout << name << " ";
        }
        std::cout << '\n';
        std::cout << "--- end demo ---\n";
    }

    // std::map and std::set example
    {
        std::cout << "\n--- std::map and std::set demo ---" << '\n';
        std::map<std::string, int> scores;
        scores["Alice"] = 90;
        scores["Bob"] = 85;
        scores["Carol"] = 95;

        std::cout << "Scores map:\n";
        for (const auto& entry : scores) {
            std::cout << "  " << entry.first << ": " << entry.second << '\n';
        }

        auto it = scores.find("Bob");
        if (it != scores.end()) {
            std::cout << "Found Bob with score " << it->second << '\n';
        }

        scores.erase("Alice");
        std::cout << "After erase(\"Alice\"):";
        for (const auto& entry : scores) {
            std::cout << " " << entry.first;
        }
        std::cout << '\n';

        std::set<std::string> names_set = {"Alice", "Bob", "Alice", "Carol"};
        std::cout << "Set contents: ";
        for (const auto& name : names_set) {
            std::cout << name << " ";
        }
        std::cout << '\n';

        names_set.insert("Dave");
        names_set.erase("Bob");
        std::cout << "After insert(\"Dave\") and erase(\"Bob\"): ";
        for (const auto& name : names_set) {
            std::cout << name << " ";
        }
        std::cout << '\n';
        std::cout << "Contains Carol: " << std::boolalpha << (names_set.count("Carol") > 0) << '\n';
        std::cout << "--- end demo ---\n";
    }

    // std::unordered_map example
    unordered_map_examples();

    // App termination loop
    for (int i = 5; i >= 0; i--)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Test App is terminating in " << i << " seconds" << '\n';
    }
    std::cout << "Test App has terminated" << '\n';

    return EXIT_SUCCESS;
}


