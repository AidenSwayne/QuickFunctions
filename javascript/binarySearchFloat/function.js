function binarySearchFloat(arr, target) {    
    let left = 0;
    let right = arr.length - 1;
    let result = -1;
    while (left <= right) {
        let mid = Math.floor((left + right) / 2);

        if (Math.abs(arr[mid] - target) < 1e-6) {
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
