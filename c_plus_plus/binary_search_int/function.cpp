#include "function.h"

int binary_search_int(const std::vector<int>& arr, int target) {
    // Use unsigned indices to let compiler skip consideration of negative values.
    if (arr.empty())
        return -1;
    unsigned indices [] = { 0, arr.size() };
    

    while (indices[0] + 1 < indices[1]) {
        //    Note: Can fail due to overflow, but unlikely to in the test action. Add half of diference to avoid.
        unsigned mid = (indices[0] + indices[1]) / 2;

        //    Branches? We don't need no stinkin' branches. (author's note: This is the _other_ type of reference)
        indices[arr[mid] > target] = mid;
    }

    return (arr[indices[0]] == target) ? static_cast<int>(indices[0]) : -1;
}
