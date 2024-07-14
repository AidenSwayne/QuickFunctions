function binarySearchString(arr, target) {
  /*
    The array may contain duplicate strings.
    You must find the earliest occurrence of the target string in the sorted array.
    */
  let left = 0;
  let right = arr.length - 1;
  let result = -1;

  while (left <= right) {
    let mid = Math.floor((left + right) / 2);
    if (arr[mid] === target) {
      result = mid;
      right = mid - 1; // Continue searching towards the left
    } else if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return result;
}