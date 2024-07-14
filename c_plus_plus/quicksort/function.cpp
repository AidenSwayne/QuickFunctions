#include "function.h"
#include <algorithm>

std::vector<int> quicksort(std::vector<int>& array) {
    if (array.size() <= 1) {
        return array;
    } else {
        int pivot = array[0];
        std::vector<int> left, right;
        for (size_t i = 1; i < array.size(); ++i) {
            if (array[i] < pivot) {
                left.push_back(array[i]);
            } else {
                right.push_back(array[i]);
            }
        }
        std::vector<int> sorted_left = quicksort(left);
        std::vector<int> sorted_right = quicksort(right);
        sorted_left.push_back(pivot);
        sorted_left.insert(sorted_left.end(), sorted_right.begin(), sorted_right.end());
        return sorted_left;
    }
}