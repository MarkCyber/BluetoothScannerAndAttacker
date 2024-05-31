#include <flipper.h>
#include <btstack.h>
#include "bt_scan.h"

// BTstack event handler function
static void btstack_event_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    if (packet_type == HCI_EVENT_PACKET) {
        switch (hci_event_packet_get_type(packet)) {
            case GAP_EVENT_ADVERTISING_REPORT: {
                char addr_str[18];
                bd_addr_to_str(packet + 3, addr_str);
                printf("Device found: %s\n", addr_str);
                // Display found device on Flipper Zero screen
                flipper_display_message("Device found: %s", addr_str);
                break;
            }
            default:
                break;
        }
    }
}

// Function to initialize BTstack and start scanning for devices
void scan_devices(void) {
    btstack_init();
    btstack_event_handler_register(btstack_event_handler);
    printf("Scanning for Bluetooth devices...\n");
    // Set scan parameters: scan interval and window
    gap_set_scan_parameters(0x30, 0x30, 0);
    // Start scanning
    gap_start_scan();
}
