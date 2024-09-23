#include "cbuild.h"

int main() {
    const char *source_files[] = { "main.c", };
    
    // Initialize with default configuration
    CBuildConfig config = cbuild_default_config(source_files, 2, "my_program");

    // Add additional flags if needed (e.g., for debugging or optimizations)
    cbuild_add_flags(&config, "-g");

    // Link libraries if needed (e.g., for math operations on Linux)
    #if CBUILD_OS_LINUX
    cbuild_add_libs(&config, "-lm");
    #endif

    // Run the build process
    cbuild_run(&config);

    // Clean up allocated memory
    //cbuild_cleanup(&config);

    return 0;
}
