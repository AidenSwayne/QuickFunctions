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
