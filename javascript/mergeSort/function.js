function mergeSort(arr) {
    if (arr.length <= 1) {
        return arr;
    }
    
    const mid = Math.floor(arr.length / 2);
    const left = arr.slice(0, mid);
    const right = arr.slice(mid);

    const sortedLeft = mergeSort(left);
    const sortedRight = mergeSort(right);

    let result = [];
    let i = 0;
    let j = 0;

    while (i < sortedLeft.length && j < sortedRight.length) {
        if (sortedLeft[i] <= sortedRight[j]) {
            result.push(sortedLeft[i]);
            i++;
        } else {
            result.push(sortedRight[j]);
            j++;
        }
    }

    return result.concat(sortedLeft.slice(i)).concat(sortedRight.slice(j));
}
