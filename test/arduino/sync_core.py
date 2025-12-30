import shutil
import os
from os.path import join, isfile

Import("env")

# Paths
# Project root is x:\_DEV\szoveges-ora\test\arduino
# Core src is x:\_DEV\szoveges-ora\src
# Target lib is x:\_DEV\szoveges-ora\test\arduino\lib\ProjectCore

project_dir = env.get("PROJECT_DIR")
core_src_dir = os.path.abspath(join(project_dir, "..", "..", "src"))
target_lib_dir = join(project_dir, "lib", "ProjectCore")

files_to_sync = [
    "Types.h",
    "IDrawer.h",
    "TextClock.h",
    "TextClock.cpp",
    "DisplayManager.h",
    "DisplayManager.cpp",
    "ClockDrawer.h",
    "ClockDrawer.cpp",
    "RingDrawer.h",
    "RingDrawer.cpp",
    "Renderer.h",
    "Renderer.cpp"
]

def sync_files():
    print("\n[SyncCore] Synchronizing core logic files...")
    
    if not os.path.exists(target_lib_dir):
        os.makedirs(target_lib_dir)
        print(f"[SyncCore] Created directory: {target_lib_dir}")

    for filename in files_to_sync:
        src_path = join(core_src_dir, filename)
        dst_path = join(target_lib_dir, filename)
        
        if isfile(src_path):
            # Only copy if source is newer or destination doesn't exist
            if not isfile(dst_path) or os.path.getmtime(src_path) > os.path.getmtime(dst_path):
                shutil.copy2(src_path, dst_path)
                print(f"[SyncCore] Copied: {filename}")
            else:
                print(f"[SyncCore] Skipping (up to date): {filename}")
        else:
            print(f"[SyncCore] WARNING: Source file not found: {src_path}")

# Hook: Sync before build
sync_files()

# Note: We don't delete on success because it breaks incremental builds 
# and IDE intellisense. If you want to delete them after upload, 
# you can add a post-action, but it's usually counter-productive for developement.
