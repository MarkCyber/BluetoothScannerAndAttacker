#include "bt_scan.h"
#include "bt_capture.h"
#include "bt_exploit.h"
#include "bt_command.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


// Flag to keep the application running
static volatile int keepRunning = 1;

// Signal handler to catch termination signals
void intHandler(int dummy) {
    keepRunning = 0;
}

int main() {
    // Initialize the Bluetooth scanning module
    if (bt_scan_init() != 0) {
        fprintf(stderr, "Failed to initialize Bluetooth scanning module.\n");
        return EXIT_FAILURE;
    }

    // Initialize the Bluetooth capture module
    if (bt_capture_init() != 0) {
        fprintf(stderr, "Failed to initialize Bluetooth capture module.\n");
        return EXIT_FAILURE;
    }

    // Initialize the Bluetooth exploit module
    if (bt_exploit_init() != 0) {
        fprintf(stderr, "Failed to initialize Bluetooth exploit module.\n");
        return EXIT_FAILURE;
    }

    // Initialize the Bluetooth command module
    if (bt_command_init() != 0) {
        fprintf(stderr, "Failed to initialize Bluetooth command module.\n");
        return EXIT_FAILURE;
    }

    // Set up signal handler for graceful termination
    signal(SIGINT, intHandler);

    // Main loop to continuously perform Bluetooth operations
    while (keepRunning) {
        bt_scan();
        bt_capture();
        bt_exploit();
        bt_command();
    }

    // Cleanup
    bt_scan_cleanup();
    // Similarly, add cleanup functions for other modules (if needed)
    printf("Shutting down gracefully...\n");

    return EXIT_SUCCESS;
}
