const fs = require('fs');
const path = require('path');
const { performance } = require('perf_hooks');

// Append export statement to function.js
const functionPath = path.join(__dirname, 'function.js');
fs.appendFileSync(functionPath, '\nmodule.exports = { matrixMultiply2d };');

// Now we can require the function
const { matrixMultiply2d } = require('./function');

function createMatrix(rows, cols, minVal = 0, maxVal = 10) {
    return Array(rows).fill().map(() => 
        Array(cols).fill().map(() => Math.floor(Math.random() * (maxVal - minVal + 1)) + minVal)
    );
}

function validateMatrixMultiplication(matrix1, matrix2, expectedResult) {
    try {
        const result = matrixMultiply2d(matrix1, matrix2);
        if (!Array.isArray(result) || !result.every(row => Array.isArray(row))) {
            console.log(`Failed test case: matrix1=${JSON.stringify(matrix1)}, matrix2=${JSON.stringify(matrix2)}`);
            console.log(`Expected return type: Array of Arrays, Actual return type: ${typeof result}`);
            process.exit(1);
        }
        if (JSON.stringify(result) !== JSON.stringify(expectedResult)) {
            console.log(`Failed test case: matrix1=${JSON.stringify(matrix1)}, matrix2=${JSON.stringify(matrix2)}`);
            process.exit(1);
        }
    } catch (e) {
        if (e.message !== "The number of columns in matrix1 must be equal to the number of rows in matrix2.") {
            console.log(`Error occurred for test case: matrix1=${JSON.stringify(matrix1)}, matrix2=${JSON.stringify(matrix2)}, error=${e.message}`);
            process.exit(1);
        }
    }
}

const testCases = [
    [[[1, 2], [3, 4]], [[5, 6], [7, 8]], [[19, 22], [43, 50]]],
    [[[1, 2, 3]], [[4], [5], [6]], [[32]]],
    [[[1, 2], [3, 4], [5, 6]], [[7, 8, 9], [10, 11, 12]], [[27, 30, 33], [61, 68, 75], [95, 106, 117]]]
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([matrix1, matrix2, expectedResult]) => {
    if (expectedResult === null) {
        validateMatrixMultiplication(matrix1, matrix2, expectedResult);
    } else {
        try {
            validateMatrixMultiplication(matrix1, matrix2, expectedResult);
            const start = performance.now();
            for (let i = 0; i < 100; i++) {
                matrixMultiply2d(matrix1, matrix2);
            }
            const end = performance.now();
            totalExecutionTime += (end - start);
        } catch (e) {
            console.log(`Error occurred for test case: matrix1=${JSON.stringify(matrix1)}, matrix2=${JSON.stringify(matrix2)}, error=${e.message}`);
            process.exit(1);
        }
    }
});

// Run random test cases with small matrices and measure execution time
const randomCaseCount = 100;
for (let i = 0; i < randomCaseCount; i++) {
    try {
        const rows1 = Math.floor(Math.random() * 10) + 1;
        const cols1 = Math.floor(Math.random() * 10) + 1;
        const rows2 = cols1;
        const cols2 = Math.floor(Math.random() * 10) + 1;
        const matrix1 = createMatrix(rows1, cols1);
        const matrix2 = createMatrix(rows2, cols2);

        const expectedResult = matrix1.map(row => 
            matrix2[0].map((_, j) => 
                row.reduce((sum, cell, i) => sum + cell * matrix2[i][j], 0)
            )
        );
        validateMatrixMultiplication(matrix1, matrix2, expectedResult);

        const start = performance.now();
        for (let j = 0; j < 100; j++) {
            matrixMultiply2d(matrix1, matrix2);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(`Error occurred for random test case: error=${e.message}`);
        process.exit(1);
    }
}

// Run random test cases with large matrices and measure execution time
const randomCaseCountLarge = 10;
for (let i = 0; i < randomCaseCountLarge; i++) {
    try {
        const rows1 = Math.floor(Math.random() * 41) + 10;
        const cols1 = Math.floor(Math.random() * 41) + 10;
        const rows2 = cols1;
        const cols2 = Math.floor(Math.random() * 41) + 10;
        const matrix1 = createMatrix(rows1, cols1);
        const matrix2 = createMatrix(rows2, cols2);

        const expectedResult = matrix1.map(row => 
            matrix2[0].map((_, j) => 
                row.reduce((sum, cell, i) => sum + cell * matrix2[i][j], 0)
            )
        );
        validateMatrixMultiplication(matrix1, matrix2, expectedResult);

        const start = performance.now();
        for (let j = 0; j < 100; j++) {
            matrixMultiply2d(matrix1, matrix2);
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
functionContent = functionContent.replace('\nmodule.exports = { matrixMultiply2d };', '');
fs.writeFileSync(functionPath, functionContent);