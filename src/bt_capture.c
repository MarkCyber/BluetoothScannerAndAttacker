#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

#define EIR_FLAGS                   0x01
#define EIR_NAME_SHORT              0x08
#define EIR_NAME_COMPLETE           0x09
#define EIR_MANUFACTURER_SPECIFIC   0xFF

void eir_parse_name(uint8_t *eir, size_t eir_len, char *buf, size_t buf_len) {
    size_t offset = 0;

    while (offset < eir_len) {
        uint8_t field_len = eir[0];
        size_t name_len;

        if (field_len == 0) break;

        if (eir[1] == EIR_NAME_SHORT || eir[1] == EIR_NAME_COMPLETE) {
            name_len = field_len - 1;
            if (name_len > buf_len) name_len = buf_len - 1;

            memcpy(buf, &eir[2], name_len);
            buf[name_len] = '\0';
            return;
        }

        offset += field_len + 1;
        eir += field_len + 1;
    }

    buf[0] = '\0';
}

void eir_parse_manufacturer(uint8_t *eir, size_t eir_len) {
    size_t offset = 0;

    while (offset < eir_len) {
        uint8_t field_len = eir[0];

        if (field_len == 0) break;

        if (eir[1] == EIR_MANUFACTURER_SPECIFIC) {
            printf("Manufacturer Specific Data: ");
            for (int i = 2; i < field_len + 1; i++) {
                printf("%02X ", eir[i]);
            }
            printf("\n");
        }

        offset += field_len + 1;
        eir += field_len + 1;
    }
}

int main(int argc, char **argv) {
    int dev_id, sock, len, flags;
    int max_rsp = 255;
    inquiry_info *ii = NULL;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    if (dev_id < 0) {
        perror("No Bluetooth Adapter Available");
        exit(1);
    }

    sock = hci_open_dev(dev_id);
    if (sock < 0) {
        perror("Opening socket");
        exit(1);
    }

    len = 8;  // Inquiry duration * 1.28s
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    int num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("hci_inquiry");
        free(ii);
        close(sock);
        exit(1);
    }

    for (int i = 0; i < num_rsp; i++) {
        ba2str(&(ii[i].bdaddr), addr);
        memset(name, 0, sizeof(name));

        if (hci_read_remote_name(sock, &(ii[i].bdaddr), sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");

        printf("Address: %s, Name: %s\n", addr, name);

        // Example of additional parsing for BLE data
        // Here we assume the additional data is in the form of an EIR array.
        uint8_t eir_data[255];
        int eir_len = hci_read_remote_name_with_clock_offset(sock, &(ii[i].bdaddr), eir_data, sizeof(eir_data), 0, 0);
        if (eir_len > 0) {
            eir_parse_manufacturer(eir_data, eir_len);
        }
    }

    free(ii);
    close(sock);
    return 0;
}
