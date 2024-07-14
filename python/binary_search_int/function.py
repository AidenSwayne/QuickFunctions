def binary_search_int(arr: list[int], target: int) -> int:
    left = 0
    right = len(arr) - 1
    result = -1
    

    while left <= right:
        mid = (left + right) // 2

        if arr[mid] == target:
            result = mid
            right = mid - 1
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return result
