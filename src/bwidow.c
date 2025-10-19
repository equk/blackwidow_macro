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
#define DEV_VID 0x1532
// product_id of known blackwidow devices
#define DEV_PID_BW_2013 0x011b
#define DEV_PID_BW_ULT 0x010d
#define DEV_PID_BW_ULT_2014 0x011a
#define DEV_PID_BW 0x010e
#define DEV_PID_BW_CHROMA_V2 0x0221

// USB Device
#define DEV_INTF 2

// Version
const char * BWIDOW_VERSION = "1.2";

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

libusb_device_handle * handle;
int verbose = 0;
int send_init = 0;

// Initialize libusb
int init () {
    libusb_init(NULL);

// libusb_set_debug replaced with libusb_set_option in libusb 1.0.22
#if LIBUSB_API_VERSION >= 0x01000106
    libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, 3);
#else
    libusb_set_debug(NULL, 3);
#endif

    // Open device based on vender & pid
    handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID_BW_2013);
    if (handle == NULL) {
        // try next device
        handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID_BW_ULT);
        if (handle == NULL) {
            // try next device
            handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID_BW_ULT_2014);
            if (handle == NULL) {
                //try next device
                handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID_BW);
                if (handle == NULL) {
                    //try next device
                    handle = libusb_open_device_with_vid_pid(NULL, DEV_VID, DEV_PID_BW_CHROMA_V2);
                    if(handle == NULL){
                        //no devices found
                        printf("ERROR - No Known Razer BlackWidow Devices Found\n");
                        libusb_exit(NULL);
                        return 1;
                    } else {
                        printf("Razer BlackWidow Chroma V2 Devide Found\n");
                    }
                } else {
                    printf("Razer BlackWidow Device Found\n");
                }
            } else {
                printf("Razer BlackWidow Ultimate 2014 Device Found\n");
            }
        } else {
            printf("Razer BlackWidow Ultimate Device Found\n");
        }
    } else {
        printf("Razer BlackWidow 2013 Device Found\n");
    }

    // Detaching libusb kernel driver
    if (libusb_kernel_driver_active(handle,DEV_INTF))
        libusb_detach_kernel_driver(handle, DEV_INTF);

    // Give feedback & quit on error
    if (libusb_claim_interface(handle, DEV_INTF) < 0) {
        printf("IO Error Opening USB Device\n");
        libusb_close(handle);
        libusb_exit(NULL);
        return 1;
    }

    return 0;
}

void showBanner () {
    printf("Usage: ./bwidow -s -v \n");
}

void closeHandle () {
    libusb_release_interface(handle, DEV_INTF);
    libusb_close(handle);
    libusb_exit(NULL);
}

void printData (unsigned char * data) {
    int k = 0;
    for (int i = 0; i < 90; i++) {
        printf(" %02x", data[i]);
        if (k++ == 7) {
            printf("\n");
            k = 0;
        }
    }
    printf("\n");
}

// Send data to USB
int sendcmd (unsigned char * data) {
    if (verbose) {
        printf("Sending data:\n");
        printData(data);
    }

    int res;
    res = libusb_control_transfer(handle,
        LIBUSB_REQUEST_TYPE_CLASS,
        0x9, 0x300, 0, data, 90, 1000);

    if (verbose) {
        printf("Transmitted: %d\n", res);
    }
    return res;
}

// Console argument parsing
int scanArgs (char* arg, char* argv[], int argc) {
    int i = 0;
    for (i = 1; i < argc; i++) {
        if (strcmp(arg, argv[i]) == 0) {
            return i;
        }
    }
    return 0;
}

// Main
int main (int argc, char * argv[]) {
    printf("Razer BlackWidow Macro Keys v%s\n", BWIDOW_VERSION);

    if ( argc<2 )
    {
        // Show Help Banner If No Args Given
        printf("\nUsage: %s <arg>\n",argv[0]);
        printf("\nArguments:\n \t-s\tsend init packet\n");
        printf("\t-v\tshow verbose output\n");
        printf("\nNote: This script requires root access for kernel driver\n");
        return 0;
    }
    else
    {

        if (init() != 0) return 1;

        // Verbose mode
        if (scanArgs("-v", argv, argc)) {
            verbose = 1;
        }

        // Send Button Init Command
        if (scanArgs("-s", argv, argc)){
            send_init = 1;
        }

        if (send_init) {
        //Send Data
            sendcmd(Blackwidow_Init);
        }

        closeHandle();
    }
    return 0;
}
