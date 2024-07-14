const fs = require('fs');
const path = require('path');
const { performance } = require('perf_hooks');

// Append export statement to function.js
const functionPath = path.join(__dirname, 'function.js');
fs.appendFileSync(functionPath, '\nmodule.exports = { jsonEncode };');

// Now we can require the function
const { jsonEncode } = require('./function');

function validateJsonEncode(inputData, expectedOutput) {
    try {
        const actualOutput = jsonEncode(inputData);
        if (typeof actualOutput !== 'string') {
            console.log(`Failed test case: input='${JSON.stringify(inputData)}'`);
            console.log(`Expected return type: str, Actual return type: ${typeof actualOutput}`);
            process.exit(1);
        }
        if (actualOutput !== expectedOutput) {
            console.log(`Failed test case: input='${JSON.stringify(inputData)}', expected=${expectedOutput}, actual=${actualOutput}`);
            process.exit(1);
        }
    } catch (e) {
        console.log(`Error occurred for test case: input='${JSON.stringify(inputData)}', error=${e.message}`);
        process.exit(1);
    }
}

const testCases = [
    [{"name": "John", "age": 30}, '{"name":"John","age":30}'],
    [[1, 2, 3], '[1,2,3]'],
    [{"set": new Set([1, 2, 3])}, '{"set":[1,2,3]}'],
    [{"invalid": Infinity}, '{"invalid":"Infinity"}'],
    [{"invalid": -Infinity}, '{"invalid":"-Infinity"}'],
    [{"invalid": NaN}, '{"invalid":"NaN"}'],
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([inputData, expectedOutput]) => {
    validateJsonEncode(inputData, expectedOutput);
    const start = performance.now();
    for (let i = 0; i < 1000; i++) {
        jsonEncode(inputData);
    }
    const end = performance.now();
    totalExecutionTime += (end - start);
});

// Run random test cases and measure execution time
const randomCaseCount = 100;
for (let i = 0; i < randomCaseCount; i++) {
    const inputData = (() => {
        const choice = Math.floor(Math.random() * 4);
        switch (choice) {
            case 0:
                return {"key": "value", "nested": {"key": "value"}};
            case 1:
                return Array.from({length: Math.floor(Math.random() * 10) + 1}, () => Math.floor(Math.random() * 100) + 1);
            case 2:
                return {"set": new Set(Array.from({length: Math.floor(Math.random() * 10) + 1}, () => Math.floor(Math.random() * 100) + 1))};
            case 3:
                return {"invalid": [Infinity, -Infinity, NaN][Math.floor(Math.random() * 3)]};
        }
    })();

    function customJsonEncoder(key, value) {
        if (value instanceof Set) {
            return Array.from(value);
        } else if (typeof value === 'number' && !isFinite(value)) {
            if (value === Infinity) return "Infinity";
            if (value === -Infinity) return "-Infinity";
            return "NaN";
        }
        return value;
    }

    const expectedOutput = JSON.stringify(inputData, customJsonEncoder);
    validateJsonEncode(inputData, expectedOutput);
    const start = performance.now();
    for (let j = 0; j < 1000; j++) {
        jsonEncode(inputData);
    }
    const end = performance.now();
    totalExecutionTime += (end - start);
}

const averageExecutionTimeMs = totalExecutionTime / (testCases.length + randomCaseCount) / 1000;
console.log(`Average execution time: ${averageExecutionTimeMs.toFixed(10)} ms`);

// Remove the export statement from function.js
let functionContent = fs.readFileSync(functionPath, 'utf8');
functionContent = functionContent.replace('\nmodule.exports = { jsonEncode };', '');
fs.writeFileSync(functionPath, functionContent);