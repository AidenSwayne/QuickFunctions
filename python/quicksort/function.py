def quicksort(array: list[int]) -> list[int]:
    if len(array) <= 1:
        return array
    
    else:
        pivot = array[0]
        left = [x for x in array[1:] if x < pivot]
        right = [x for x in array[1:] if x >= pivot]
        return quicksort(left) + [pivot] + quicksort(right)
