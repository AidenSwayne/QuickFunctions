def binary_search_string(arr: list[str], target: str) -> int:
    '''
    The array may contain duplicate strings.
    You must find the earliest occurrence of the target string in the sorted array.
    '''
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