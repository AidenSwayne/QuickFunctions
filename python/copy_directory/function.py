def copy_directory(source_dir: str, destination_dir: str) -> bool:
    import os
    import shutil

    # Delete the destination directory if it already exists
    if os.path.exists(destination_dir):
        shutil.rmtree(destination_dir)

    # Copy the source directory to the destination directory
    shutil.copytree(source_dir, destination_dir)
    return True