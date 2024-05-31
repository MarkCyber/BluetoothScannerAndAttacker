#include <flipper.h>
#include "bt_command.h"

void send_command(char *device_address, const char *command) {
    printf("Sending command to device: %s\n", device_address);
    flipper_display_message("Sending command: %s", command);
    // Implementation for sending commands to the device
    // This might involve writing to specific Bluetooth services or characteristics
}
