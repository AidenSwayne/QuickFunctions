
"""
    Parameters:
    arr: a list of possibly empty strings, sorted in ascending lexicographic order, with duplicates.
    target: the target string to search for, which is not guaranteed to be in the array.

    return value:
    the lowest index in arr where target is located if it is present, and -1 otherwise.
"""

def binary_search_string(arr: list[str], target: str) -> int:
    left = 0
    right = len(arr) - 1
    result = -1

    while left <= right:
        mid = (left + right) // 2
        if arr[mid] == target:
            result = mid
            right = mid - 1  # Continue searching towards the left
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return result