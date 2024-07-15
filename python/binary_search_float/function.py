
"""
    Parameters:
    arr: a list of valid floats, sorted in ascending order, with duplicates.
    target: the target float to search for, which is not guaranteed to be in the array.

    return value:
    the lowest index in arr where target is located if it is present, and -1 otherwise.
"""
def binary_search_float(arr: list[float], target: float) -> int:
    left = 0
    right = len(arr) - 1
    result = -1
    

    while left <= right:
        mid = (left + right) // 2

        if abs(arr[mid] - target) < 1e-6:
            result = mid
            right = mid - 1
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return result
