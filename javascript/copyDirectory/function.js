function copyDirectory(sourceDir, destinationDir) {
    const fs = require('fs');
    const path = require('path');

    // Delete the destination directory if it already exists
    if (fs.existsSync(destinationDir)) {
        fs.rmSync(destinationDir, { recursive: true, force: true });
    }

    // Copy the source directory to the destination directory
    fs.cpSync(sourceDir, destinationDir, { recursive: true });
    return true;
}
module.exports = { copyDirectory };
module.exports = { copyDirectory };