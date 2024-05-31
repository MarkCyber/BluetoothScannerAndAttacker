#include <flipper.h>
#include <btstack.h>
#include "bt_scan.h"

void scan_devices(void) {
    btstack_init();
    printf("Scanning for Bluetooth devices...\n");
    gap_set_scan_parameters(0x30, 0x30, 0);
    gap_start_scan();
}

void btstack_event_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    if (packet_type == HCI_EVENT_PACKET) {
        switch (hci_event_packet_get_type(packet)) {
            case GAP_EVENT_ADVERTISING_REPORT:
                printf("Device found: %s\n", bd_addr_to_str(packet + 3));
                flipper_display_message("Device found: %s", bd_addr_to_str(packet + 3));
                break;
            default:
                break;
        }
    }
}

void btstack_event_handler_register(void) {
    btstack_event_handler_register(btstack_event_handler);
}
