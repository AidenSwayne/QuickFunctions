const fs = require('fs');
const path = require('path');
const { performance } = require('perf_hooks');

// Append export statement to function.js
const functionPath = path.join(__dirname, 'function.js');
fs.appendFileSync(functionPath, '\nmodule.exports = { validatePassword };');

// Now we can require the function
const { validatePassword } = require('./function');

function validatePasswordWrapper(password, expectedOutput) {
    try {
        const actualOutput = validatePassword(password);
        if (actualOutput !== expectedOutput) {
            console.log(`Failed test case: password='${password}', expected=${expectedOutput}, actual=${actualOutput}`);
            process.exit(1);
        }
    } catch (e) {
        console.log(`Error occurred for test case: password='${password}', error=${e.message}`);
        process.exit(1);
    }
}

const testCases = [
    ["Abcdefg1!", true],
    ["abcdefgh1", false],
    ["ABCDEFGH!", false],
    ["Abcdefgh!", false],
    ["Abcdefg1", false],
    ["Ab1!", false],
    ["abcdefgh", false],
    ["ABCDEFGH", false],
    ["12345678", false],
    ["!@#$%^&*", false],
    ["Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!", true],
    ["Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!", false],
    ["Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdef", true],
    ["abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*(),.?\":{}|<>", false],
    ["ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*(),.?\":{}|<>", false],
    ["abcdefgh1!", false],
    ["Abcdefgh1", false],
    ["Abcdefgh!", false],
    ["Abc123!@#", true],
    ["Abc123!", false],
    ["Abc123!@#$%^&*(),.?\":{}|<>", true],
    ["abcABC123!@#", true],
    ["abcABC123!", true],
    ["abcABC123!@#$%^&*(),.?\":{}|<>", true],
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([password, expectedOutput]) => {
    validatePasswordWrapper(password, expectedOutput);
    const start = performance.now();
    for (let i = 0; i < 100; i++) {
        validatePassword(password);
    }
    const end = performance.now();
    totalExecutionTime += (end - start);
});

// Run random test cases and measure execution time
const randomCaseCount = 1000;
const allChars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?';
for (let i = 0; i < randomCaseCount; i++) {
    const password = Array(Math.floor(Math.random() * 66) + 5).fill().map(() => allChars[Math.floor(Math.random() * allChars.length)]).join('');
    const expectedOutput = (
        password.length >= 8 && password.length <= 64 &&
        /[A-Z]/.test(password) &&
        /[a-z]/.test(password) &&
        /[0-9]/.test(password) &&
        /[!@#$%^&*()_+\-=\[\]{}|;:,.<>?]/.test(password)
    );
    validatePasswordWrapper(password, expectedOutput);
    const start = performance.now();
    for (let j = 0; j < 100; j++) {
        validatePassword(password);
    }
    const end = performance.now();
    totalExecutionTime += (end - start);
}

const averageExecutionTimeMs = totalExecutionTime / (testCases.length + randomCaseCount) / 100;
console.log(`Average execution time: ${averageExecutionTimeMs.toFixed(10)} ms`);

// Remove the export statement from function.js
let functionContent = fs.readFileSync(functionPath, 'utf8');
functionContent = functionContent.replace('\nmodule.exports = { validatePassword };', '');
fs.writeFileSync(functionPath, functionContent);