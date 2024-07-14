function copyDirectory(sourceDir, destinationDir) {
    // Delete the destination directory if it already exists
    if (fs.existsSync(destinationDir)) {
        fs.rmSync(destinationDir, { recursive: true, force: true });
    }

    // Create the destination directory
    fs.mkdirSync(destinationDir, { recursive: true });

    // Copy the source directory to the destination directory
    const items = fs.readdirSync(sourceDir, { withFileTypes: true });
    for (const item of items) {
        const srcPath = path.join(sourceDir, item.name);
        const destPath = path.join(destinationDir, item.name);

        if (item.isDirectory()) {
            copyDirectory(srcPath, destPath);
        } else {
            fs.copyFileSync(srcPath, destPath);
        }
    }
    return true;
}
