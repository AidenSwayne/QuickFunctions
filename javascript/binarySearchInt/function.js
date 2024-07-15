
// This function should return the lowest index in array arr where target is located.

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
