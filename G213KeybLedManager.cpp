/**
  *  The MIT License (MIT)
  *
  *  g231KeybLedManager.cpp v0.1 Copyright (c) 2017 Carlos Arana
  *  This software is a migration to c++ from G213Colors all the rights to SebiTimeWaster
  *  for his investigation on the commands.
  *
  *  Permission is hereby granted, free of charge, to any person obtaining a copy
  *  of this software and associated documentation files (the "Software"), to deal
  *  in the Software without restriction, including without limitation the rights
  *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  *  copies of the Software, and to permit persons to whom the Software is
  *  furnished to do so, subject to the following conditions:
  *
  *  The above copyright notice and this permission notice shall be included in all
  *  copies or substantial portions of the Software.
  *
  *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  *  SOFTWARE.
**/


#include <iostream>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include "G213KeybLedManager.h"

/* Convert a hex char digit to its integer value. */
int G213KeybLedManager::hexDigitToInt(char digit) {
    digit = tolower(digit);
    if ('0' <= digit && digit <= '9') //if it's decimal
        return (int)(digit - '0');
    else if ('a' <= digit && digit <= 'f') //if it's abcdef
        return (int)(digit - ('a' - 10));
    else
        return -1; //value not in [0-9][a-f] range
}

/* Decode a hex string. */
unsigned char *G213KeybLedManager::decodeHexString(const char *hexStr,unsigned char *hexBin) {
    char* decoded =(char *) malloc(strlen(hexStr)/2+1);
    char* hexStrPtr = (char *)hexStr;
    char* decodedPtr = decoded;

    while (*hexStrPtr != '\0') { /* Step through hexStr, two chars at a time. */
        *decodedPtr = 16 * hexDigitToInt(*hexStrPtr) + hexDigitToInt(*(hexStrPtr+1));
        hexStrPtr += 2;
        decodedPtr++;
    }

    memcpy(hexBin,decoded,strlen(hexStr)/2);
    free(decoded);

    return hexBin;
}

libusb_device_handle * G213KeybLedManager::g213GetDeviceHandle() {
    struct libusb_device **devs;
    struct libusb_device *g213dev = NULL;
    struct libusb_device *dev;
    struct libusb_device_handle *handle = NULL;

    size_t i = 0;
    int r;

    if (libusb_get_device_list(ctx, &devs) < 0)
        return NULL;

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            g213dev =  NULL;
        }
        if (desc.idVendor == LOGITECH_VENDOR_ID && desc.idProduct == LOGITECH_PRODUCT_ID) {
            g213dev = dev;
            break;
        }
    }

    if (g213dev == NULL) {
        std::cout<<"No se ha encontrado teclado G213"<<std::endl; //there was an error
        return NULL;
    }

    int openResult = libusb_open(g213dev,&handle);

    libusb_free_device_list(devs, 1);

    if (openResult < 0) {
        std::cout<<"Cant open device error = "<<openResult<<std::endl;
        return NULL;
    }

    return handle;
}


bool G213KeybLedManager::attach() {
    int retValue;

    if((retValue=libusb_init(&ctx)) < 0) {
        std::cout<<"Init Error "<<retValue<<std::endl; //there was an error
        return false;
    }


    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO); //set verbosity level to 3, as suggested in the documentation

    if ((g213Handle = g213GetDeviceHandle())  == NULL) {
        std::cout<<"Cant open device"<<std::endl; //there was an error
        return false;
    }

    // This attach and reattach kernel driver automatically on open/close.
    libusb_set_auto_detach_kernel_driver(g213Handle,1);

    if(libusb_claim_interface(g213Handle, 0) < 0) {
        std::cout<<"Cannot Claim Interface"<<std::endl;
        return false;
    }

    return true;
}

void G213KeybLedManager::detach() {
    libusb_release_interface(g213Handle,0);
    libusb_close(g213Handle); //close the device we opened

    libusb_exit(ctx); //needs to be called to end the
}

void G213KeybLedManager::g213SendData(unsigned char *data,int16_t len) {

    int retControl = libusb_control_transfer(g213Handle,0x21,
                     0x09,0x0211,0,data,len,0);
    if (retControl != 20) {
        printf("libusb_control_transfer error: %s", libusb_error_name(retControl));
    }
}

void G213KeybLedManager::g213SetColor(char *hexColor,short field) {
    char cmdBuffer[41];

    sprintf(cmdBuffer,"11ff0c3a0%d01%s0200000000000000000000",field,hexColor);
    unsigned char cmdToSend[20];
    decodeHexString(cmdBuffer,cmdToSend);

    g213SendData((unsigned char *)cmdToSend,20);
}

void G213KeybLedManager::g213SetBreathe(char *hexColor,long timeInMills) {
    char cmdBuffer[41];

    sprintf(cmdBuffer,"11ff0c3a0002%s%04ld006400000000000000",hexColor,timeInMills);
    unsigned char cmdToSend[20];
    decodeHexString(cmdBuffer,cmdToSend);

    g213SendData((unsigned char *)cmdToSend,20);
}

void G213KeybLedManager::g213SetCycle(long timeInMills) {
    char cmdBuffer[41];

    sprintf(cmdBuffer,"11ff0c3a0003ffffff0000%04ld64000000000000",timeInMills);
    unsigned char cmdToSend[20];
    decodeHexString(cmdBuffer,cmdToSend);

    g213SendData((unsigned char *)cmdToSend,20);

}

