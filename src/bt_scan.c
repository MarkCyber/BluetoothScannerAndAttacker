#include "bt_scan.h"
#include <stdio.h>
#include <stdlib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

static int dev_id;
static int sock;

int bt_scan_init() {
    // Get the resource ID of the first available Bluetooth adapter
    dev_id = hci_get_route(NULL);
    if (dev_id < 0) {
        perror("Failed to get Bluetooth adapter ID");
        return -1;
    }

    // Open a socket to the Bluetooth adapter
    sock = hci_open_dev(dev_id);
    if (sock < 0) {
        perror("Failed to open Bluetooth socket");
        return -1;
    }

    printf("Bluetooth scan initialized.\n");
    return 0; // Success
}

void bt_scan() {
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int len, flags;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    len = 8; // Duration of the inquiry
    max_rsp = 255; // Maximum number of devices to discover
    flags = IREQ_CACHE_FLUSH; // Flush cache of previously discovered devices
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("hci_inquiry");
        free(ii);
        return;
    }

    for (int i = 0; i < num_rsp; i++) {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }

    free(ii);
}

int bt_scan_cleanup() {
    if (close(sock) < 0) {
        perror("Failed to close Bluetooth socket");
        return -1;
    }
    printf("Bluetooth scan cleaned up.\n");
    return 0; // Success
}
