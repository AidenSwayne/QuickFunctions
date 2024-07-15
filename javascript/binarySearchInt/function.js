/*
    Parameters:
    arr: an array of integers, sorted in ascending order, with duplicates.
    target: the target integer to search for, which is not guaranteed to be in the array.

    return value:
    the lowest index in arr where target is located if it is present, and -1 otherwise.
*/

function binarySearchInt(arr, target) {
    let left = 0;
    let right = arr.length - 1;
    let result = -1;
    

    
    while (left <= right) {
        let mid = Math.floor((left + right) / 2);

        if (arr[mid] === target) {
            result = mid;
            right = mid - 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}
