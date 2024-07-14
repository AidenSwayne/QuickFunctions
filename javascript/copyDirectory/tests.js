const fs = require('fs');
const path = require('path');
const os = require('os');
const crypto = require('crypto');
const { performance } = require('perf_hooks');

// Append export statement to function.js
const functionPath = path.join(__dirname, 'function.js');
fs.appendFileSync(functionPath, '\nmodule.exports = { copyDirectory };');

// Now we can require the function
const { copyDirectory } = require('./function');

function createTestFiles(directory) {
    // Create a tree of files and directories
    fs.mkdirSync(path.join(directory, "subdir1", "subsubdir1"), { recursive: true });
    fs.mkdirSync(path.join(directory, "subdir2"), { recursive: true });

    // Create files in the directories
    fs.writeFileSync(path.join(directory, "file1.txt"), "This is file1.");
    fs.writeFileSync(path.join(directory, "subdir1", "file2.txt"), "This is file2.");
    fs.writeFileSync(path.join(directory, "subdir1", "subsubdir1", "file3.txt"), "This is file3.");
    fs.writeFileSync(path.join(directory, "subdir2", "file4.txt"), "This is file4.");

    // Create 1MB file with random data
    crypto.randomFillSync(Buffer.alloc(1024 * 1024)).toJSON().data.forEach(byte => {
        fs.appendFileSync(path.join(directory, "file_1MB.bin"), Buffer.from([byte]));
    });

    // Create 200MB file with random data
    for (let i = 0; i < 200; i++) {
        crypto.randomFillSync(Buffer.alloc(1024 * 1024)).toJSON().data.forEach(byte => {
            fs.appendFileSync(path.join(directory, "file_200MB.bin"), Buffer.from([byte]));
        });
    }
}

function validateCopyDirectory(sourceDir, destinationDir) {
    try {
        const result = copyDirectory(sourceDir, destinationDir);

        if (typeof result !== 'boolean') {
            console.log(`Failed test case: source_dir='${sourceDir}', destination_dir='${destinationDir}'`);
            console.log(`Expected return type: bool, Actual return type: ${typeof result}`);
            process.exit(1);
        }

        if (!result) {
            console.log(`Failed test case: source_dir='${sourceDir}', destination_dir='${destinationDir}'`);
            process.exit(1);
        }

        // Compare the contents of the source and destination directories
        const sourceFiles = fs.readdirSync(sourceDir);
        const destFiles = fs.readdirSync(destinationDir);

        if (JSON.stringify(sourceFiles) !== JSON.stringify(destFiles)) {
            console.log(`Failed test case: source_dir='${sourceDir}', destination_dir='${destinationDir}'`);
            process.exit(1);
        }
    } catch (e) {
        console.log(`Error occurred for test case: source_dir='${sourceDir}', destination_dir='${destinationDir}', error=${e.message}`);
        process.exit(1);
    }
}

// Create a temporary directory to store the test files
const tempTestDir = fs.mkdtempSync(path.join(os.tmpdir(), 'test_files_'));

// Create the test files in the temporary directory
createTestFiles(tempTestDir);

const testCases = [
    ["source_dir1", "destination_dir1"],
    ["source_dir2", "destination_dir2"],
    ["source_dir3", "destination_dir3"],
];

let totalExecutionTime = 0;

// Run the test cases and measure execution time
testCases.forEach(([sourceDir, destinationDir]) => {
    // Create a temporary destination directory
    const tempDestinationDir = fs.mkdtempSync(path.join(os.tmpdir(), 'dest_'));

    // Copy the test files to the source directory
    const tempSourceDir = fs.mkdtempSync(path.join(os.tmpdir(), 'source_'));
    fs.cpSync(tempTestDir, tempSourceDir, { recursive: true });

    validateCopyDirectory(tempSourceDir, tempDestinationDir);
    const start = performance.now();
    for (let i = 0; i < 100; i++) {
        copyDirectory(tempSourceDir, tempDestinationDir);
    }
    const end = performance.now();
    totalExecutionTime += (end - start);

    // Clean up the temporary directories
    fs.rmSync(tempSourceDir, { recursive: true, force: true });
    fs.rmSync(tempDestinationDir, { recursive: true, force: true });
});

// Run random test cases and measure execution time
const randomCaseCount = 10;
for (let i = 0; i < randomCaseCount; i++) {
    // Create a temporary destination directory
    const tempDestinationDir = fs.mkdtempSync(path.join(os.tmpdir(), 'dest_'));

    // Copy the test files to the source directory
    const tempSourceDir = fs.mkdtempSync(path.join(os.tmpdir(), 'source_'));
    fs.cpSync(tempTestDir, tempSourceDir, { recursive: true });

    try {
        validateCopyDirectory(tempSourceDir, tempDestinationDir);
        const start = performance.now();
        for (let j = 0; j < 100; j++) {
            copyDirectory(tempSourceDir, tempDestinationDir);
        }
        const end = performance.now();
        totalExecutionTime += (end - start);
    } catch (e) {
        console.log(e);
    }

    // Clean up the temporary directories
    fs.rmSync(tempSourceDir, { recursive: true, force: true });
    fs.rmSync(tempDestinationDir, { recursive: true, force: true });
}

// Clean up the temporary test files directory
fs.rmSync(tempTestDir, { recursive: true, force: true });

const averageExecutionTimeMs = totalExecutionTime / (testCases.length + randomCaseCount) / 100; // 100 iterations per test case
console.log(`Average execution time: ${averageExecutionTimeMs.toFixed(10)} ms`);

// Remove the export statement from function.js
let functionContent = fs.readFileSync(functionPath, 'utf8');
functionContent = functionContent.replace('\nmodule.exports = { copyDirectory };', '');
fs.writeFileSync(functionPath, functionContent);