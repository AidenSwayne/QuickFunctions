
'''
This function should return the lowest index in list arr where target is located.
The list may contain duplicate strings.
'''

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