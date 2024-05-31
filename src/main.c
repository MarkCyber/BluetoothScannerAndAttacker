#include <flipper.h>
#include "bt_scan.h"
#include "bt_capture.h"
#include "bt_exploit.h"
#include "bt_command.h"

// Main menu function
void main_menu(void) {
    while (1) {
        flipper_display_clear();
        flipper_display_message("1. Scan Devices\n2. Capture Data\n3. Exploit Device\n4. Send Command\n5. Exit\n");
        int choice = flipper_get_input();
        switch (choice) {
            case 1:
                scan_devices();
                break;
            case 2:
                capture_data();
                break;
            case 3:
                flipper_display_message("Enter device address:");
                char device_address[18];
                flipper_get_input_string(device_address, 18);
                exploit_device(device_address);
                break;
            case 4:
                flipper_display_message("Enter device address:");
                flipper_get_input_string(device_address, 18);
                flipper_display_message("Enter command:");
                char command[256];
                flipper_get_input_string(command, 256);
                send_command(device_address, command);
                break;
            case 5:
                return;
            default:
                flipper_display_message("Invalid choice");
                break;
        }
    }
}

int main(void) {
    flipper_init();
    btstack_event_handler_register();
    main_menu();
    return 0;
}
