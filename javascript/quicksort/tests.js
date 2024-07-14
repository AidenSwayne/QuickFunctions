const fs = require('fs');
const path = require('path');
const { performance } = require('perf_hooks');

// Append export statement to function.js
const functionPath = path.join(__dirname, 'function.js');
fs.appendFileSync(functionPath, '\nmodule.exports = { quicksort };');

// Now we can require the function
const { quicksort } = require('./function');

function createArray(size, minVal = 0, maxVal = 100) {
    return Array(size).fill().map(() => Math.floor(Math.random() * (maxVal - minVal + 1)) + minVal);
}

function validateSortedArray(array, expectedResult) {
    if (JSON.stringify(array) !== JSON.stringify(expectedResult)) {
        console.log(`Failed test case: array=${JSON.stringify(array)}`);
        console.log(`Expected output: ${JSON.stringify(expectedResult)}`);
        console.log(`Actual output: ${JSON.stringify(array)}`);
        process.exit(1);
    }
}

const testCases = [
    [[1, 7, 4, 1, 10, 9, -2], [-2, 1, 1, 4, 7, 9, 10]],
    [[1, 2, 3, 4, 5], [1, 2, 3, 4, 5]],
    [[5, 4, 3, 2, 1], [1, 2, 3, 4, 5]],
    [[4, 2, 6, 4, 1, 4, 2, 5], [1, 2, 2, 4, 4, 4, 5, 6]],
    [[-3, 0, -9, 5, -1, 2], [-9, -3, -1, 0, 2, 5]]
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([array, expectedResult]) => {
    try {
        const sortedArray = quicksort([...array]);
        validateSortedArray(sortedArray, expectedResult);
        const start = performance.now();
        for (let i = 0; i < 100; i++) {
            quicksort([...array]);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for test case: array=${JSON.stringify(array)}, error=${e.message}`);
        process.exit(1);
    }
});

// Run random test cases with small arrays and measure execution time
const randomCaseCount = 100;
for (let i = 0; i < randomCaseCount; i++) {
    try {
        const size = Math.floor(Math.random() * 100) + 1;
        const array = createArray(size);
        const expectedResult = [...array].sort((a, b) => a - b);
        const sortedArray = quicksort([...array]);
        validateSortedArray(sortedArray, expectedResult);
        const start = performance.now();
        for (let j = 0; j < 100; j++) {
            quicksort([...array]);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for random test case: error=${e.message}`);
        process.exit(1);
    }
}

// Run random test cases with large arrays and measure execution time
const randomCaseCountLarge = 100;
for (let i = 0; i < randomCaseCountLarge; i++) {
    try {
        const size = Math.floor(Math.random() * 9001) + 1000;
        const array = createArray(size);
        const expectedResult = [...array].sort((a, b) => a - b);
        const sortedArray = quicksort([...array]);
        validateSortedArray(sortedArray, expectedResult);
        const start = performance.now();
        for (let j = 0; j < 100; j++) {
            quicksort([...array]);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for large random test case: error=${e.message}`);
        process.exit(1);
    }
}

const averageExecutionTimeMs = totalExecutionTime / (testCases.length + randomCaseCount + randomCaseCountLarge) / 100;
console.log(`Average execution time: ${averageExecutionTimeMs.toFixed(10)} ms`);

// Remove the export statement from function.js
let functionContent = fs.readFileSync(functionPath, 'utf8');
functionContent = functionContent.replace('\nmodule.exports = { quicksort };', '');
fs.writeFileSync(functionPath, functionContent);