/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2013 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
// This code has been modified by Elm�

#ifndef _usb_desc_h_
#define _usb_desc_h_

// This header is NOT meant to be included when compiling
// user sketches in Arduino.  The low-level functions
// provided by usb_dev.c are meant to be called only by
// code which provides higher-level interfaces to the user.

#include <stdint.h>
#include <stddef.h>

#define ENDPOINT_UNUSED                0x00
#define ENDPOINT_TRANSIMIT_ONLY        0x15
#define ENDPOINT_RECEIVE_ONLY          0x19
#define ENDPOINT_TRANSMIT_AND_RECEIVE  0x1D

/*
Each group of #define lines below corresponds to one of the
settings in the Tools > USB Type menu.  This file defines what
type of USB device is actually created for each of those menu
options.

Each "interface" is a set of functionality your PC or Mac will
use and treat as if it is a unique device.  Within each interface,
the "endpoints" are the actual communication channels.  Most
interfaces use 1, 2 or 3 endpoints.  By editing only this file,
you can customize the USB Types to be any collection of interfaces.

To modify a USB Type, delete the XYZ_INTERFACE lines for any
interfaces you wish to remove, and copy them from another USB Type
for any you want to add.

Give each interface a unique number, and edit NUM_INTERFACE to
reflect the total number of interfaces.

Next, assign unique endpoint numbers to all the endpoints across
all the interfaces your device has.  You can reuse an endpoint
number for transmit and receive, but the same endpoint number must
not be used twice to transmit, or twice to receive.

Most endpoints also require their maximum size, and some also
need an interval specification (the number of milliseconds the
PC will check for data from that endpoint).  For existing
interfaces, usually these other settings should not be changed.

Edit NUM_ENDPOINTS to be at least the largest endpoint number used.

Edit NUM_USB_BUFFERS to control how much memory the USB stack will
allocate.  At least 2 should be used for each endpoint.  More
memory will allow higher throughput for user programs that have
high latency (eg, spending time doing things other than interacting
with the USB).

Edit the ENDPOINT*_CONFIG lines so each endpoint is configured
the proper way (transmit, receive, or both).

If you are using existing interfaces (making your own device with
a different set of interfaces) the code in all other files should
automatically adapt to the new endpoints you specify here.

If you need to create a new type of interface, you'll need to write
the code which sends and receives packets, and presents an API to
the user.  Usually, a pair of files are added for the actual code,
and code is also added in usb_dev.c for any control transfers,
interrupt-level code, or other very low-level stuff not possible
from the packet send/receive functons.  Code also is added in
usb_inst.c to create an instance of your C++ object.

You may edit the Vendor and Product ID numbers, and strings.  If
the numbers are changed, Teensyduino may not be able to automatically
find and reboot your board when you click the Upload button in
the Arduino IDE.  You will need to press the Program button on
Teensy to initiate programming.

Some operating systems, especially Windows, may cache USB device
info.  Changes to the device name may not update on the same
computer unless the vendor or product ID numbers change, or the
"bcdDevice" revision code is increased.

If these instructions are missing steps or could be improved, please
let me know?  http://forum.pjrc.com/forums/4-Suggestions-amp-Bug-Reports
*/


// Here you can chose one of three possible touch screen devices
// TOUCH_DEVICE == 0 -> use pointer device
// TOUCH_DEVICE == 1 -> use single touch device
// TOUCH_DEVICE == 2 -> use multi  touch device
#define TOUCH_DEVICE   0



// PJRC bought the device range 0x0474 - 0x04D7 from VOTI (http://www.voti.nl/pids)
// The following is not really necessary. But Windows does not read the device descriptor anew if it once has been stored in the Registry.
// So if you change TOUCH_DEVICE from 0 to 1 or vice versa, Windows will not recognize any change!
// Without the following lines you would have to erase the device in serveral places from the Registry after changing TOUCH_DEVICE.
// This is very difficult because only SYSTEM/NT AUTHORITY has the priviliges to delete these keys.
// Search the Registry (HKLM) for "Vid_16C0&Pid_048"
#define PRODUCT_ID_KMT   0x048A + 2 * TOUCH_DEVICE // Keyboard/Mouse/Touchscreen
#define PRODUCT_ID_SKMT  0x048B + 2 * TOUCH_DEVICE // Serial/Keyboard/Mouse/Touchscreen




#if defined(USB_SERIAL)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            0x0483
  #define DEVICE_CLASS          2    // 2 = Communication Class
  #define MANUFACTURER_NAME    {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME        {'U','S','B',' ','S','e','r','i','a','l'}
  #define PRODUCT_NAME_LEN      10
  #define EP0_SIZE              64
  #define NUM_ENDPOINTS         4
  #define NUM_USB_BUFFERS       12
  #define NUM_INTERFACE         2
  #define CDC_STATUS_INTERFACE  0
  #define CDC_DATA_INTERFACE    1
  #define CDC_ACM_ENDPOINT      2
  #define CDC_RX_ENDPOINT       3
  #define CDC_TX_ENDPOINT       4
  #define CDC_ACM_SIZE          16
  #define CDC_RX_SIZE           64
  #define CDC_TX_SIZE           64
  #define ENDPOINT2_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT3_CONFIG    ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT4_CONFIG    ENDPOINT_TRANSIMIT_ONLY

// ****** Modified by Elm� *******
#elif defined(USB_HID)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            PRODUCT_ID_KMT
  #define MANUFACTURER_NAME     {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME          {'K','e','y','b','o','a','r','d','/','M','o','u','s','e','/','T','o','u','c','h','s','c','r','e','e','n'}
  #define PRODUCT_NAME_LEN      26
  #define EP0_SIZE              64
  #define NUM_USB_BUFFERS       24  
  #define NUM_ENDPOINTS         3
  #define NUM_INTERFACE         3
  // Keyboard
  #define KEYBOARD_INTERFACE    0    
  #define KEYBOARD_ENDPOINT     1
  #define ENDPOINT1_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define KEYBOARD_SIZE         8
  #define KEYBOARD_INTERVAL     1
  // Mouse (relative coordinates)
  #define MOUSE_INTERFACE       1    
  #define MOUSE_ENDPOINT        2
  #define ENDPOINT2_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define MOUSE_SIZE            8
  #define MOUSE_INTERVAL        2
  // Touchscreen (absolute coordinates)
  #define TOUCH_INTERFACE       2    
  #define TOUCH_ENDPOINT        3
  #define ENDPOINT3_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define TOUCH_SIZE            8 
  #define TOUCH_INTERVAL        2
/***** To use the Joystick remove this comment and increment NUM_ENDPOINTS and NUM_INTERFACE
  // Joystick
  #define JOYSTICK_INTERFACE    3
  #define JOYSTICK_ENDPOINT     4
  #define ENDPOINT4_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define JOYSTICK_SIZE         16
  #define JOYSTICK_INTERVAL     2
*/
 

// ****** Modified by Elm� *******
#elif defined(USB_SERIAL_HID)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            PRODUCT_ID_SKMT
  #define DEVICE_CLASS          0xEF
  #define DEVICE_SUBCLASS       0x02
  #define DEVICE_PROTOCOL       0x01
  #define MANUFACTURER_NAME     {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME          {'S','e','r','i','a','l','/','K','e','y','b','o','a','r','d','/','M','o','u','s','e','/','T','o','u','c','h','s','c','r','e','e','n'}
  #define PRODUCT_NAME_LEN      33
  #define EP0_SIZE              64
  #define NUM_USB_BUFFERS       30  
  #define NUM_ENDPOINTS         6
  #define NUM_INTERFACE         5
  // Serial
  #define CDC_IAD_DESCRIPTOR    1
  #define CDC_STATUS_INTERFACE  0
  #define CDC_DATA_INTERFACE    1 
  #define CDC_ACM_ENDPOINT      2
  #define ENDPOINT2_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define CDC_RX_ENDPOINT       3
  #define ENDPOINT3_CONFIG      ENDPOINT_RECEIVE_ONLY  
  #define CDC_TX_ENDPOINT       4
  #define ENDPOINT4_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define CDC_ACM_SIZE          16
  #define CDC_RX_SIZE           64
  #define CDC_TX_SIZE           64
  // Keyboard
  #define KEYBOARD_INTERFACE    2    
  #define KEYBOARD_ENDPOINT     1
  #define ENDPOINT1_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define KEYBOARD_SIZE         8
  #define KEYBOARD_INTERVAL     1
  // Mouse (relative coordinates)
  #define MOUSE_INTERFACE       3    
  #define MOUSE_ENDPOINT        5
  #define ENDPOINT5_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define MOUSE_SIZE            8
  #define MOUSE_INTERVAL        2
  // Touchscreen (absolute coordinates)
  #define TOUCH_INTERFACE       4 
  #define TOUCH_ENDPOINT        6
  #define ENDPOINT6_CONFIG      ENDPOINT_TRANSIMIT_ONLY  
  #define TOUCH_SIZE            8
  #define TOUCH_INTERVAL        2
/***** To use the Joystick remove this comment and increment NUM_ENDPOINTS and NUM_INTERFACE
  // Joystick
  #define JOYSTICK_INTERFACE    5
  #define JOYSTICK_ENDPOINT     7
  #define ENDPOINT7_CONFIG      ENDPOINT_TRANSIMIT_ONLY    
  #define JOYSTICK_SIZE         16
  #define JOYSTICK_INTERVAL     2
*/
  
#elif defined(USB_MIDI)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            0x0485
  #define MANUFACTURER_NAME    {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN  11
  #define PRODUCT_NAME        {'T','e','e','n','s','y',' ','M','I','D','I'}
  #define PRODUCT_NAME_LEN      11
  #define EP0_SIZE              64
  #define NUM_ENDPOINTS         4
  #define NUM_USB_BUFFERS       16
  #define NUM_INTERFACE         2
  #define SEREMU_INTERFACE      1    // Serial emulation
  #define SEREMU_TX_ENDPOINT    1
  #define SEREMU_TX_SIZE        64
  #define SEREMU_TX_INTERVAL    1
  #define SEREMU_RX_ENDPOINT    2
  #define SEREMU_RX_SIZE        32
  #define SEREMU_RX_INTERVAL    2
  #define MIDI_INTERFACE        0    // MIDI
  #define MIDI_TX_ENDPOINT      3
  #define MIDI_TX_SIZE          64
  #define MIDI_RX_ENDPOINT      4
  #define MIDI_RX_SIZE          64
  #define ENDPOINT1_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT2_CONFIG    ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT3_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT4_CONFIG    ENDPOINT_RECEIVE_ONLY

#elif defined(USB_RAWHID)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            0x0486
  #define RAWHID_USAGE_PAGE     0xFFAB  // recommended: 0xFF00 to 0xFFFF
  #define RAWHID_USAGE          0x0200  // recommended: 0x0100 to 0xFFFF
  #define MANUFACTURER_NAME    {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN  11
  #define PRODUCT_NAME        {'T','e','e','n','s','y','d','u','i','n','o',' ','R','a','w','H','I','D'}
  #define PRODUCT_NAME_LEN      18
  #define EP0_SIZE              64
  #define NUM_ENDPOINTS         6
  #define NUM_USB_BUFFERS       12
  #define NUM_INTERFACE         2
  #define RAWHID_INTERFACE      0    // RawHID
  #define RAWHID_TX_ENDPOINT    3
  #define RAWHID_TX_SIZE        64
  #define RAWHID_TX_INTERVAL    1
  #define RAWHID_RX_ENDPOINT    4
  #define RAWHID_RX_SIZE        64
  #define RAWHID_RX_INTERVAL    1
  #define SEREMU_INTERFACE      1    // Serial emulation
  #define SEREMU_TX_ENDPOINT    1
  #define SEREMU_TX_SIZE        64
  #define SEREMU_TX_INTERVAL    1
  #define SEREMU_RX_ENDPOINT    2
  #define SEREMU_RX_SIZE        32
  #define SEREMU_RX_INTERVAL    2
  #define ENDPOINT1_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT2_CONFIG    ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT3_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT4_CONFIG    ENDPOINT_RECEIVE_ONLY

#elif defined(USB_FLIGHTSIM)
  #define VENDOR_ID             0x16C0
  #define PRODUCT_ID            0x0488
  #define MANUFACTURER_NAME    {'T','e','e','n','s','y','d','u','i','n','o'}
  #define MANUFACTURER_NAME_LEN 11
  #define PRODUCT_NAME        {'T','e','e','n','s','y',' ','F','l','i','g','h','t',' ','S','i','m',' ','C','o','n','t','r','o','l','s'}
  #define PRODUCT_NAME_LEN      26
  #define EP0_SIZE              64
  #define NUM_ENDPOINTS         4
  #define NUM_USB_BUFFERS       20
  #define NUM_INTERFACE         2
  #define FLIGHTSIM_INTERFACE   0    // Flight Sim Control
  #define FLIGHTSIM_TX_ENDPOINT 3
  #define FLIGHTSIM_TX_SIZE     64
  #define FLIGHTSIM_TX_INTERVAL 1
  #define FLIGHTSIM_RX_ENDPOINT 4
  #define FLIGHTSIM_RX_SIZE     64
  #define FLIGHTSIM_RX_INTERVAL 1
  #define SEREMU_INTERFACE      1    // Serial emulation
  #define SEREMU_TX_ENDPOINT    1
  #define SEREMU_TX_SIZE        64
  #define SEREMU_TX_INTERVAL    1
  #define SEREMU_RX_ENDPOINT    2
  #define SEREMU_RX_SIZE        32
  #define SEREMU_RX_INTERVAL    2
  #define ENDPOINT1_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT2_CONFIG    ENDPOINT_RECEIVE_ONLY
  #define ENDPOINT3_CONFIG    ENDPOINT_TRANSIMIT_ONLY
  #define ENDPOINT4_CONFIG    ENDPOINT_RECEIVE_ONLY

#endif

#ifdef USB_DESC_LIST_DEFINE
#if defined(NUM_ENDPOINTS) && NUM_ENDPOINTS > 0
// NUM_ENDPOINTS = number of non-zero endpoints (0 to 15)
extern const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS];

typedef struct {
    uint16_t    wValue;
    uint16_t    wIndex;
    const uint8_t    *addr;
    uint16_t    length;
} usb_descriptor_list_t;

extern const usb_descriptor_list_t usb_descriptor_list[];
#endif // NUM_ENDPOINTS
#endif // USB_DESC_LIST_DEFINE

#endif
