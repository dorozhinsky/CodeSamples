#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

void unordered_map_examples() {
    std::cout << "=== std::unordered_map Examples ===" << std::endl;

    // Example 1: Basic insertion and access
    std::cout << "\n1. Basic Insertion and Access:" << std::endl;
    std::unordered_map<std::string, int> fruits;
    fruits["apple"] = 5;
    fruits["banana"] = 3;
    fruits["orange"] = 8;

    std::cout << "Apples: " << fruits["apple"] << std::endl;
    std::cout << "Bananas: " << fruits["banana"] << std::endl;

    // Example 2: Checking if key exists
    std::cout << "\n2. Checking if Key Exists:" << std::endl;
    if (fruits.find("apple") != fruits.end()) {
        std::cout << "Apple found in map" << std::endl;
    }

    if (fruits.find("grape") == fruits.end()) {
        std::cout << "Grape not found in map" << std::endl;
    }

    // Example 3: Iteration
    std::cout << "\n3. Iterating Through Map:" << std::endl;
    for (const auto& pair : fruits) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Example 4: Insert with insert()
    std::cout << "\n4. Using insert() Method:" << std::endl;
    auto result = fruits.insert({"grape", 7});
    if (result.second) {
        std::cout << "Grape inserted successfully" << std::endl;
    }

    // Try inserting duplicate
    result = fruits.insert({"apple", 10});
    if (!result.second) {
        std::cout << "Apple already exists, not updated" << std::endl;
    }

    // Example 5: Size and empty checks
    std::cout << "\n5. Size and Empty Checks:" << std::endl;
    std::cout << "Map size: " << fruits.size() << std::endl;
    std::cout << "Is empty: " << (fruits.empty() ? "yes" : "no") << std::endl;

    // Example 6: Erasing elements
    std::cout << "\n6. Erasing Elements:" << std::endl;
    fruits.erase("banana");
    std::cout << "After erasing banana, size: " << fruits.size() << std::endl;

    // Example 7: Integer keys
    std::cout << "\n7. Unordered Map with Integer Keys:" << std::endl;
    std::unordered_map<int, std::string> idToName;
    idToName[1] = "Alice";
    idToName[2] = "Bob";
    idToName[3] = "Charlie";

    for (const auto& pair : idToName) {
        std::cout << "ID " << pair.first << ": " << pair.second << std::endl;
    }

    // Example 8: Custom default values
    std::cout << "\n8. Default Values:" << std::endl;
    std::unordered_map<std::string, int> counter;
    counter["apple"]++;  // Creates entry with 0, then increments
    counter["apple"]++;
    counter["banana"]++;

    std::cout << "Apple count: " << counter["apple"] << std::endl;
    std::cout << "Banana count: " << counter["banana"] << std::endl;

    // Example 9: Count occurrences
    std::cout << "\n9. Using count() Method:" << std::endl;
    std::cout << "Count of 'apple': " << counter.count("apple") << std::endl;
    std::cout << "Count of 'grape': " << counter.count("grape") << std::endl;

    // Example 10: Clear the map
    std::cout << "\n10. Clearing Map:" << std::endl;
    counter.clear();
    std::cout << "Size after clear: " << counter.size() << std::endl;

    std::cout << "\n=== End of std::unordered_map Examples ===" << std::endl;
}
