const fs = require('fs');
const path = require('path');
const { performance } = require('perf_hooks');

// Append export statement to function.js
const functionPath = path.join(__dirname, 'function.js');
fs.appendFileSync(functionPath, '\nmodule.exports = { binarySearchInt };');

// Now we can require the function
const { binarySearchInt } = require('./function');

function createSortedArray(size, minVal = 0, maxVal = 100) {
    return Array.from({ length: size }, () => Math.floor(Math.random() * (maxVal - minVal + 1) + minVal)).sort((a, b) => a - b);
}

function validateSearchResult(array, target, expectedIndex) {
    const index = binarySearchInt(array, target);
    if (index !== expectedIndex) {
        console.log(`Failed test case: array=${JSON.stringify(array)}, target=${target}`);
        console.log(`Expected index: ${expectedIndex}`);
        console.log(`Actual index: ${index}`);
        process.exit(1);
    }
}

const testCases = [
    [[-2, 1, 1, 4, 7, 9, 10], 1, 1],
    [[1, 2, 3, 4, 5], 3, 2],
    [[1, 2, 3, 4, 5], 6, -1],
    [[1, 2, 2, 4, 4, 4, 5, 6], 4, 3],
    [[-9, -3, -1, 0, 2, 5], -3, 1],
    [[1, 1, 1, 2, 2, 3, 3], 1, 0],
    [[2, 2, 3, 3, 4, 4, 4], 4, 4]
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([array, target, expectedIndex]) => {
    try {
        validateSearchResult(array, target, expectedIndex);
        const start = performance.now();
        for (let i = 0; i < 1000; i++) {
            binarySearchInt(array, target);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for test case: array=${JSON.stringify(array)}, target=${target}, error=${e.message}`);
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
        for (let j = 0; j < 1000; j++) {
            binarySearchInt(array, target);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for test case: array=${JSON.stringify(array)}, target=${target}, error=${e.message}`);
        process.exit(1);
    }
}

// Run random test cases with large arrays and measure execution time
const randomCaseCountLarge = 100;
for (let i = 0; i < randomCaseCountLarge; i++) {
    try {
        const size = Math.floor(Math.random() * 9001) + 1000;
        const array = createSortedArray(size);
        const target = array[Math.floor(Math.random() * array.length)];
        const expectedIndex = array.indexOf(target);
        validateSearchResult(array, target, expectedIndex);
        const start = performance.now();
        for (let j = 0; j < 1000; j++) {
            binarySearchInt(array, target);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for test case: array=${JSON.stringify(array)}, target=${target}, error=${e.message}`);
        process.exit(1);
    }
}

const averageExecutionTimeMs = totalExecutionTime / (testCases.length + randomCaseCount + randomCaseCountLarge) / 1000; // 1000 runs per test case
console.log(`Average execution time: ${averageExecutionTimeMs.toFixed(10)} ms`);

// Remove the export statement from function.js
let functionContent = fs.readFileSync(functionPath, 'utf8');
functionContent = functionContent.replace('\nmodule.exports = { binarySearchInt };', '');
fs.writeFileSync(functionPath, functionContent);