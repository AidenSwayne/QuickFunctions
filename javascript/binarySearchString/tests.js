const fs = require("fs");
const path = require("path");
const { performance } = require("perf_hooks");

// Append export statement to function.js
const functionPath = path.join(__dirname, "function.js");
fs.appendFileSync(functionPath, "\nmodule.exports = { binarySearchString };");

// Now we can require the function
const { binarySearchString } = require("./function");

function createSortedArray(size, minLength = 1, maxLength = 10) {
  const characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  return Array.from({ length: size }, () => {
    const length =
      Math.floor(Math.random() * (maxLength - minLength + 1)) + minLength;
    return Array.from(
      { length },
      () => characters[Math.floor(Math.random() * characters.length)],
    ).join("");
  }).sort();
}

function validateSearchResult(array, target, expectedIndex) {
  const index = binarySearchString(array, target);
  if (index !== expectedIndex) {
    console.log(
      `Failed test case: array=${JSON.stringify(array)}, target=${target}`,
    );
    console.log(`Expected index: ${expectedIndex}`);
    console.log(`Actual index: ${index}`);
    process.exit(1);
  }
}

const testCases = [
  [["apple", "banana", "cherry", "date", "elderberry"], "cherry", 2],
  [["cat", "dog", "elephant", "fox", "giraffe"], "horse", -1],
  [["alice", "bob", "charlie", "david", "eve"], "alice", 0],
  [["alpha", "beta", "beta", "gamma", "gamma", "gamma", "delta"], "gamma", 3],
  [["blue", "green", "indigo", "orange", "red", "yellow"], "orange", 3],
  [["a", "a", "a", "b", "b", "c", "c"], "a", 0],
  [["x", "x", "y", "y", "z", "z", "z"], "z", 4],
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([array, target, expectedIndex]) => {
  try {
    validateSearchResult(array, target, expectedIndex);
    const start = performance.now();
    for (let i = 0; i < 10000; i++) {
      binarySearchString(array, target);
    }
    const end = performance.now();
    totalExecutionTime += end - start;
  } catch (e) {
    console.log(
      `Error occurred for test case: array=${JSON.stringify(array)}, target=${target}, error=${e.message}`,
    );
    process.exit(1);
  }
});

// Run random test cases with small arrays and measure execution time
const randomCaseCount = 100;
for (let i = 0; i < randomCaseCount; i++) {
  try {
    const size = Math.floor(Math.random() * 100) + 1;
    const array = createSortedArray(size);
    const target = array[Math.floor(Math.random() * array.length)];
    const expectedIndex = array.indexOf(target);
    validateSearchResult(array, target, expectedIndex);
    const start = performance.now();
    for (let j = 0; j < 10000; j++) {
      binarySearchString(array, target);
    }
    const end = performance.now();
    totalExecutionTime += end - start;
  } catch (e) {
    console.log(
      `Error occurred for test case: array=${JSON.stringify(array)}, target=${target}, error=${e.message}`,
    );
    process.exit(1);
  }
}

// Run random test cases with large arrays and measure execution time
const randomCaseCountLarge = 10;
for (let i = 0; i < randomCaseCountLarge; i++) {
  try {
    const size = Math.floor(Math.random() * 9001) + 1000;
    const array = createSortedArray(size);
    const target = array[Math.floor(Math.random() * array.length)];
    const expectedIndex = array.indexOf(target);
    validateSearchResult(array, target, expectedIndex);
    const start = performance.now();
    for (let j = 0; j < 10000; j++) {
      binarySearchString(array, target);
    }
    const end = performance.now();
    totalExecutionTime += end - start;
  } catch (e) {
    console.log(
      `Error occurred for test case: array=${JSON.stringify(array)}, target=${target}, error=${e.message}`,
    );
    process.exit(1);
  }
}

const averageExecutionTimeMs = totalExecutionTime / (testCases.length + randomCaseCount + randomCaseCountLarge) / 10000; // 10000 iterations per test case
console.log(`Average execution time: ${averageExecutionTimeMs.toFixed(10)} ms`);

// Remove the export statement from function.js
let functionContent = fs.readFileSync(functionPath, "utf8");
functionContent = functionContent.replace(
  "\nmodule.exports = { binarySearchString };",
  "",
);
fs.writeFileSync(functionPath, functionContent);