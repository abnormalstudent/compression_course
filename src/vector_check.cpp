#include <vector> 
#include <iostream>

int main() {

    uint32_t size = 5;
    std::vector<int> arr;
    arr.reserve(size);

    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] != nullptr) {
            std::cout << i  <<  "'th doesn't exist in the array." << std::endl; 
        } else {
            std::cout << i << " arr[i] == " << arr[i] << std::endl; 
        }
    }
    return 0;
}