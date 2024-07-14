function quicksort(array) {
    if (array.length <= 1) {
        return array;
    } else {
        const pivot = array[0];
        const left = array.slice(1).filter(x => x < pivot);
        const right = array.slice(1).filter(x => x >= pivot);
        return [...quicksort(left), pivot, ...quicksort(right)];
    }
}
