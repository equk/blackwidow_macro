/*
 * Razer BlackWidow Macro Keys Enabler
 * Copyright (C) 2016 B.Walden
 *
 * This program sends a init packet used by Razer proprietary drivers
 * to initialize the 'Macro' keys on Razer BlackWidow keyboards.
 *
 * Note: BlackWidow Keyboards do not have any onboard memory for macros
 *       Razer use software for macro functionality in windows
 *       You can however use 'xmacro' in linux
 *       You can also use xmodmap or your WM to allocate functions to the
 *       macro key keycodes.
 *
 * M Key Keycodes: M1 = 191, M2 = 192, M3 = 193, M4 = 194, M5 = 195
 *
 * License: MIT (LICENSE file should be included with source)
 */

#include <libusb-1.0/libusb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// vendor_id = Razer
const int vendorId = 0x1532;
// product_id of known blackwidow devices
const int keyboardIds[] = {0x011b, 0x010d, 0x010e, 0x0221, 0x0203, 0x011a};

// Version
const char *BWIDOW_VERSION = "2.0";

// CLI Options
int verbose = 0;
int send_init = 0;

// Blackwidow M Key Init Code
unsigned char Blackwidow_Init[90] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x00
};

// USB Device Handle
// (check usb device connected before attempting to open connection)
struct libusb_device_handle *usb_init(struct libusb_context *context, int venid, int devid)
{
    struct libusb_device **device_list;
    struct libusb_device_handle *handle = NULL;

    int deviceCount = libusb_get_device_list(context, &device_list);

    int i;
    for (i = 0; i < deviceCount; i++)
    {
        struct libusb_device *device = device_list[i];
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(device, &desc);
        if (desc.idVendor == venid && desc.idProduct == devid)
        {
            libusb_open(device, &handle);
            break;
        }
    }

    libusb_free_device_list(device_list, 1);

    return handle;
}

// Close USB Handle
void usb_close(struct libusb_device_handle *handle)
{
    if (handle != NULL)
    {
        libusb_close(handle);
    }
}

void printData(unsigned char *data)
{
    int k = 0;
    for (int i = 0; i < 90; i++)
    {
        printf(" %02x", data[i]);
        if (k++ == 7)
        {
            printf("\n");
            k = 0;
        }
    }
    printf("\n");
}

// Send data to USB
int sendcmd(unsigned char *data, struct libusb_device_handle *handle)
{
    if (verbose)
    {
        printf("Sending data:\n");
        printData(data);
    }

    int res;
    res = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_CLASS, 0x9, 0x300, 0, data, 90, 1000);

    if (verbose)
    {
        printf("Transmitted: %d\n", res);
    }

    return res;
}

// Console argument parsing
int scanArgs(char *arg, char *argv[], int argc)
{
    int i = 0;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(arg, argv[i]) == 0)
        {
            return i;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{

    printf("Razer BlackWidow Macro Keys v%s\n", BWIDOW_VERSION);

    if (argc < 2)
    {
        // Show Help Banner If No Args Given
        printf("\nUsage: %s <arg>\n", argv[0]);
        printf("\nArguments:\n \t-s\tsend init packet\n");
        printf("\t-v\tshow verbose output\n");
        printf("\nNote: This script requires root access for kernel driver\n");
        return 0;
    }
    else
    {

        // Verbose mode
        if (scanArgs("-v", argv, argc))
        {
            verbose = 1;
        }

        // Send Macro Init Command
        if (scanArgs("-s", argv, argc))
        {
            send_init = 1;
        }

        if (send_init)
        {

            struct libusb_context *context;
            struct libusb_device_handle *handle = NULL;

            libusb_init(&context);
            libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, 3);

            int vid = vendorId;

            int i;

            for (i = 0; i < 5; i++)
            {
                // DEBUG
                // printf("[%04x:%04x]\n", vid, keyboardIds[i]);
                int pid = keyboardIds[i];
                handle = usb_init(context, vid, pid);
                if (handle != NULL)
                {
                    printf("Sending Keyboard Macro Init Sequence for [%04x:%04x]\n\n", vid, pid);

                    sendcmd(Blackwidow_Init, handle);

                    usb_close(handle);
                    libusb_exit(NULL);
                }
            }
        }
    }

    return 0;
}
