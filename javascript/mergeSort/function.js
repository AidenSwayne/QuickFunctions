function mergeSort(arr,start,stop,buffer) {

  //Adding strict mode actually significantly increases performance.
  "use strict";

  //An inner function
  function mergeSortInner(arr,start,stop,buffer) {

      //Find the midpoint. ~~ is a faster shortcut for Math.floor
      const mid = ~~((start + stop) / 2);

      //Sort both halves.
      //When the length is 2+, call recursively.
      //When exactly 2, sort in-place with a simple swap check.
      //When less than 2, no need to do anything.
      const dstart = mid - start, dend = stop - mid;
      if (dstart > 2) mergeSortInner(arr, start, mid, buffer);
      else if (dstart === 2 && arr[start] > arr[start+1]) { const swap = arr[start]; arr[start] = arr[start+1]; arr[start+1] = swap; }
      if (dend > 2) mergeSortInner(arr, mid, stop, buffer);
      else if (dend === 2 && arr[mid] > arr[mid+1]) { const swap = arr[mid]; arr[mid] = arr[mid+1]; arr[mid+1] = swap; }

      //Shortcut: while the first element of the left is less than the first element of the right,
      //we can just skip forward, which will bypass the whole copy-back-and-forth steps.
      let midval = arr[mid];
      for (; start < mid && arr[start] < midval; start++) /* empty loop! */;

      //Wait, did we find ALL of them were already in place?
      //If so, the array is ALREADY in the right order, we can skip the entire next section.
      if (start === mid) return;

      //Walk through the array, copy to the buffer
      let i = start, b = start, j = mid;
      for (; i < mid && j < stop; b++) buffer[b] = arr[i] <= arr[j] ? arr[i++] : arr[j++];

      //Finish copying the LEFT side buffer
      //We don't have to copy the right side - if there's stuff left in the right, it's ALREADY in the right place in the array!
      for (; i < mid; i++, b++) buffer[b] = arr[i];

      //Copy buffer segment back to main
      for (let x = start; x < b; x++) arr[x] = buffer[x];
  }

  //Call our inner function with a start, end, and a single buffer to use for temporary storage.
  mergeSortInner(arr,0,arr.length, new Array(arr.length));
  return arr;
}