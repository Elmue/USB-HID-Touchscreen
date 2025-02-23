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

#if F_CPU >= 20000000

#define USB_DESC_LIST_DEFINE
#include "usb_desc.h"
#ifdef NUM_ENDPOINTS
#include "usb_names.h"
#include "kinetis.h"
#include "avr_functions.h"

// USB Descriptors are binary data which the USB host reads to
// automatically detect a USB device's capabilities.  The format
// and meaning of every field is documented in numerous USB
// standards.  When working with USB descriptors, despite the
// complexity of the standards and poor writing quality in many
// of those documents, remember descriptors are nothing more
// than constant binary data that tells the USB host what the
// device can do.  Computers will load drivers based on this data.
// Those drivers then communicate on the endpoints specified by
// the descriptors.

// To configure a new combination of interfaces or make minor
// changes to existing configuration (eg, change the name or ID
// numbers), usually you would edit "usb_desc.h".  This file
// is meant to be configured by the header, so generally it is
// only edited to add completely new USB interfaces or features.



// **************************************************************
//   USB Device
// **************************************************************

#define LSB(n) ((n) & 255)
#define MSB(n) (((n) >> 8) & 255)

// USB Device Descriptor.  The USB host reads this first, to learn
// what type of device is connected.
static uint8_t device_descriptor[] = {
        18,                                     // bLength
        1,                                      // bDescriptorType
        0x00, 0x02,                             // bcdUSB
#ifdef DEVICE_CLASS
        DEVICE_CLASS,                           // bDeviceClass
#else
    0,
#endif
#ifdef DEVICE_SUBCLASS
        DEVICE_SUBCLASS,                        // bDeviceSubClass
#else
    0,
#endif
#ifdef DEVICE_PROTOCOL
        DEVICE_PROTOCOL,                        // bDeviceProtocol
#else
    0,
#endif
        EP0_SIZE,                               // bMaxPacketSize0
        LSB(VENDOR_ID), MSB(VENDOR_ID),         // idVendor
        LSB(PRODUCT_ID), MSB(PRODUCT_ID),       // idProduct
        0x00, 0x01,                             // bcdDevice
        1,                                      // iManufacturer
        2,                                      // iProduct
        3,                                      // iSerialNumber
        1                                       // bNumConfigurations
};

// These descriptors must NOT be "const", because the USB DMA
// has trouble accessing flash memory with enough bandwidth
// while the processor is executing from flash.



// **************************************************************
//   HID Report Descriptors
// **************************************************************

// Each HID interface needs a special report descriptor that tells
// the meaning and format of the data.

#ifdef KEYBOARD_INTERFACE
// Keyboard Protocol 1, HID 1.11 spec, Appendix B, page 59-60
static uint8_t keyboard_report_desc[] = {
        0x05, 0x01,             // Usage Page (Generic Desktop),
        0x09, 0x06,             // Usage (Keyboard),
        0xA1, 0x01,             // Collection (Application),
        0x75, 0x01,             //    Report Size (1),
        0x95, 0x08,             //    Report Count (8),
        0x05, 0x07,             //    Usage Page (Key Codes),
        0x19, 0xE0,             //    Usage Minimum (224),
        0x29, 0xE7,             //    Usage Maximum (231),
        0x15, 0x00,             //    Logical Minimum (0),
        0x25, 0x01,             //    Logical Maximum (1),
        0x81, 0x02,             //    Input (Data, Variable, Absolute), ;Modifier byte
        0x95, 0x08,             //    Report Count (8),
        0x75, 0x01,             //    Report Size (1),
        0x15, 0x00,             //    Logical Minimum (0),
        0x25, 0x01,             //    Logical Maximum (1),
        0x05, 0x0C,             //    Usage Page (Consumer),
        0x09, 0xE9,             //    Usage (Volume Increment),
        0x09, 0xEA,             //    Usage (Volume Decrement),
        0x09, 0xE2,             //    Usage (Mute),
        0x09, 0xCD,             //    Usage (Play/Pause),
        0x09, 0xB5,             //    Usage (Scan Next Track),
        0x09, 0xB6,             //    Usage (Scan Previous Track),
        0x09, 0xB7,             //    Usage (Stop),
        0x09, 0xB8,             //    Usage (Eject),
        0x81, 0x02,             //    Input (Data, Variable, Absolute), ;Media keys
        0x95, 0x05,             //    Report Count (5),
        0x75, 0x01,             //    Report Size (1),
        0x05, 0x08,             //    Usage Page (LEDs),
        0x19, 0x01,             //    Usage Minimum (1),
        0x29, 0x05,             //    Usage Maximum (5),
        0x91, 0x02,             //    Output (Data, Variable, Absolute), ;LED report
        0x95, 0x01,             //    Report Count (1),
        0x75, 0x03,             //    Report Size (3),
        0x91, 0x03,             //    Output (Constant),                 ;LED report padding
        0x95, 0x06,             //    Report Count (6),
        0x75, 0x08,             //    Report Size (8),
        0x15, 0x00,             //    Logical Minimum (0),
        0x25, 0x7F,             //    Logical Maximum(104),
        0x05, 0x07,             //    Usage Page (Key Codes),
        0x19, 0x00,             //    Usage Minimum (0),
        0x29, 0x7F,             //    Usage Maximum (104),
        0x81, 0x00,             //    Input (Data, Array),                ;Normal keys
        0xc0                    // End Collection
};
#endif

// Modified by Elm�
#ifdef MOUSE_INTERFACE

// Mouse Protocol 1, HID 1.11 spec, Appendix B, page 59-60, with wheel extension
// See:
// http://www.beyondlogic.org/usbnutshell/usb5.shtml
// http://eleccelerator.com/tutorial-about-usb-hid-report-descriptors
// To analyze USB packets life use USBlyzer (33 days fully functional trial period for free)

static uint8_t mouse_report_desc[] =
{
        // This descriptor is a copy of a real mouse: Genius optical mouse with the coordinates X,Y extended to 16 bit.

        0x05, 0x01,                     // Usage Page (Generic Desktop)
        0x09, 0x02,                     // Usage (Mouse)
        0xA1, 0x01,                     // Collection (Application)

        // Define the 3 mouse buttons: bit 0 = LEFT, bit 1 = RIGHT, bit 2 = MIDDLE button
        0x05, 0x09,                     //   Usage Page (Button)
        0x19, 0x01,                     //   Usage Minimum (Button #1)
        0x29, 0x03,                     //   Usage Maximum (Button #3)
        0x15, 0x00,                     //   Logical Minimum (0)
        0x25, 0x01,                     //   Logical Maximum (1)
        0x95, 0x03,                     //   Report Count (3)
        0x75, 0x01,                     //   Report Size (1)
        0x81, 0x02,                     //   Input (Data, Variable, Absolute)

        // Declare the remaining 5 bits of the first data byte as constant -> the driver will ignore them
        0x95, 0x01,                     //   Report Count (1)
        0x75, 0x05,                     //   Report Size (5)
        0x81, 0x03,                     //   Input (Constant)

        // Define relative X and Y coordinates of 16 bit each
        0x05, 0x01,                     //   Usage Page (Generic Desktop)
        0x09, 0x01,                     //   Usage (Pointer)
        0xA1, 0x00,                     //   Collection (Physical)
        0x09, 0x30,                     //     Usage (X)
        0x09, 0x31,                     //     Usage (Y)
        0x16, 0x01, 0x80,               //     Logical Minimum (-32767)
        0x26, 0xFF, 0x7F,               //     Logical Maximum (32767)
        0x75, 0x10,                     //     Report Size (16),
        0x95, 0x02,                     //     Report Count (2),
        0x81, 0x06,                     //     Input (Data, Variable, Relative)
        0xC0,                           //   End Collection

        // Define relative wheel movement of 8 bit
        0x09, 0x38,                     //   Usage (Wheel)
        0x15, 0x81,                     //   Logical Minimum (-127)
        0x25, 0x7F,                     //   Logical Maximum (127)
        0x75, 0x08,                     //   Report Size (8),
        0x95, 0x01,                     //   Report Count (1),
        0x81, 0x06,                     //   Input (Data, Variable, Relative)

        0xC0                            // End Collection

        // With this declaration we will have to send a data packet as:
        // byte 1   -> mouse buttons          (bit 0 = LEFT, bit 1 = RIGHT, bit 2 = MIDDLE button)
        // byte 2,3 -> relative X coordinate  (-32767...32767)
        // byte 4,5 -> relative Y coordinate  (-32767...32767)
        // byte 6   -> wheel                  (-127...127)
};
#endif

// Added by Elm�
#ifdef TOUCH_INTERFACE

// See:
// http://www.beyondlogic.org/usbnutshell/usb5.shtml
// http://eleccelerator.com/tutorial-about-usb-hid-report-descriptors
// To analyze USB packets life use USBlyzer (33 days fully functional trial period for free)
//
// About single touch device descriptors:
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff553737%28v=vs.85%29.aspx
// About multi touch device descriptors:
// https://msdn.microsoft.com/en-us/library/windows/hardware/ff553732%28v=vs.85%29.aspx

static uint8_t touch_report_desc[] =
{
	// ================================================================================
    #if TOUCH_DEVICE == 2 // multi touch device (product ID: 0x048E + 0x048F)
	// ================================================================================
	
        0x05, 0x0D,                    // USAGE_PAGE(Digitizers)
        0x09, 0x04,                    // USAGE     (Touch Screen)
        0xA1, 0x01,                    // COLLECTION(Application)
		
        // define the maximum amount of fingers that the device supports
        0x09, 0x55,                    //   USAGE(Contact Count Maximum)
        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
        0xB1, 0x02,                    //   FEATURE (Data,Var,Abs)

        // define the actual amount of fingers that are concurrently touching the screen
        0x09, 0x54,                    //   USAGE (Contact count)
        0x95, 0x01,                    //   REPORT_COUNT(1)
        0x75, 0x08,                    //   REPORT_SIZE (8)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		
		// declare a finger collection
        0x09, 0x22,                    //   USAGE (Finger)
        0xA1, 0x02,                    //   COLLECTION (Logical)

        // declare an identifier for the finger
        0x09, 0x51,                    //     USAGE (Contact Identifier)
        0x75, 0x08,                    //     REPORT_SIZE (8)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)

        // declare Tip Switch and In Range
        0x09, 0x42,                    //     USAGE (Tip Switch)
        0x09, 0x32,                    //     USAGE (In Range)
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x02,                    //     REPORT_COUNT(2)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)

        // declare the remaining 6 bits of the first data byte as constant -> the driver will ignore them
        0x95, 0x06,                    //     REPORT_COUNT (6)
        0x81, 0x03,                    //     INPUT (Cnst,Ary,Abs)

        // define absolute X and Y coordinates of 16 bit each (percent values multiplied with 100)
        0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //     Usage (X)
        0x09, 0x31,                    //     Usage (Y)
        0x16, 0x00, 0x00,              //     Logical Minimum (0)
        0x26, 0x10, 0x27,              //     Logical Maximum (10000)
        0x36, 0x00, 0x00,              //     Physical Minimum (0)
        0x46, 0x10, 0x27,              //     Physical Maximum (10000)
        0x66, 0x00, 0x00,              //     UNIT (None)
        0x75, 0x10,                    //     Report Size (16),
        0x95, 0x02,                    //     Report Count (2),
        0x81, 0x02,                    //     Input (Data,Var,Abs)
        0xC0,                          //   END_COLLECTION
        0xC0                           // END_COLLECTION
		
        // With this declaration a data packet must be sent as:
		// byte 1   -> "contact count"        (always == 1)		
		// byte 2   -> "contact identifier"   (any value)
        // byte 3   -> "Tip Switch" state     (bit 0 = Tip Switch up/down, bit 1 = In Range)
        // byte 4,5 -> absolute X coordinate  (0...10000)
        // byte 6,7 -> absolute Y coordinate  (0...10000)

	// ================================================================================
    #elif TOUCH_DEVICE == 1 // single touch device (product ID: 0x048C + 0x048D)
	// ================================================================================

        0x05, 0x0d,                    // USAGE_PAGE (Digitizer)
        0x09, 0x02,                    // USAGE (Pen)
        0xa1, 0x01,                    // COLLECTION (Application)
		
		// declare a finger collection
        0x09, 0x20,                    //   Usage (Stylus)
        0xA1, 0x00,                    //   Collection (Physical)

        // Declare a finger touch (finger up/down)
        0x09, 0x42,                    //     Usage (Tip Switch)
		0x09, 0x32,                    //     USAGE (In Range)
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x02,                    //     REPORT_COUNT (2)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)

        // Declare the remaining 6 bits of the first data byte as constant -> the driver will ignore them
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x06,                    //     REPORT_COUNT (6)
        0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)

        // Define absolute X and Y coordinates of 16 bit each (percent values multiplied with 100)
        // http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
        // Chapter 16.2 says: "In the Stylus collection a Pointer physical collection will contain the axes reported by the stylus."
        0x05, 0x01,                    //     Usage Page (Generic Desktop)
        0x09, 0x01,                    //     Usage (Pointer)
        0xA1, 0x00,                    //     Collection (Physical)
        0x09, 0x30,                    //        Usage (X)
        0x09, 0x31,                    //        Usage (Y)
        0x16, 0x00, 0x00,              //        Logical Minimum (0)
        0x26, 0x10, 0x27,              //        Logical Maximum (10000)
        0x36, 0x00, 0x00,              //        Physical Minimum (0)
        0x46, 0x10, 0x27,              //        Physical Maximum (10000)
        0x66, 0x00, 0x00,              //        UNIT (None)
        0x75, 0x10,                    //        Report Size (16),
        0x95, 0x02,                    //        Report Count (2),
        0x81, 0x02,                    //        Input (Data,Var,Abs)
        0xc0,                          //     END_COLLECTION

        0xc0,                          //   END_COLLECTION
        0xc0                           // END_COLLECTION

        // With this declaration a data packet must be sent as:
        // byte 1   -> "touch" state          (bit 0 = pen up/down, bit 1 = In Range)
        // byte 2,3 -> absolute X coordinate  (0...10000)
        // byte 4,5 -> absolute Y coordinate  (0...10000)

	// ================================================================================
    #elif TOUCH_DEVICE == 0 // pointer device (product ID: 0x048A + 0x048B)
	// This descriptor is a copy of a real touchscreen: ELO TouchSystems CarrollTouch 4500U
	// ================================================================================

        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x01,                    // USAGE (Pointer)
        0xa1, 0x01,                    // COLLECTION (Application)

		// declare a pointer collection
		0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
        0x09, 0x01,                    //   USAGE (Pointer)
        0xa1, 0x00,                    //   COLLECTION (Physical)

        // Declare a button
        0x05, 0x09,                    //     USAGE_PAGE (Button)
        0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
        0x29, 0x01,                    //     USAGE_MAXIMUM (Button 1)
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
        0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
        0x66, 0x00, 0x00,              //     UNIT (None)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x81, 0x62,                    //     INPUT (Data,Var,Abs)

        // Declare the remaining 7 bits of the first data byte as constant -> the driver will ignore them
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x95, 0x07,                    //     REPORT_COUNT (7)
        0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)

        // Define absolute X and Y coordinates of 16 bit each (percent values multiplied with 100)
        0x05, 0x01,                    //     Usage Page (Generic Desktop)
        0x09, 0x30,                    //     Usage (X)
        0x09, 0x31,                    //     Usage (Y)
        0x16, 0x00, 0x00,              //     Logical Minimum (0)
        0x26, 0x10, 0x27,              //     Logical Maximum (10000)
        0x36, 0x00, 0x00,              //     Physical Minimum (0)
        0x46, 0x10, 0x27,              //     Physical Maximum (10000)
        0x66, 0x00, 0x00,              //     UNIT (None)
        0x75, 0x10,                    //     Report Size (16),
        0x95, 0x02,                    //     Report Count (2),
        0x81, 0x62,                    //     Input (Data,Var,Abs)

        0xc0,                          //   END_COLLECTION
        0xc0                           // END_COLLECTION

        // With this declaration a data packet must be sent as:
        // byte 1   -> "button" state         (bit 0 = Button up/down)
        // byte 2,3 -> absolute X coordinate  (0...10000)
        // byte 4,5 -> absolute Y coordinate  (0...10000)
		
	#else
		#error "Invalid value for TOUCH_DEVICE"
    #endif
};
#endif

#ifdef JOYSTICK_INTERFACE
static uint8_t joystick_report_desc[] = {
        0x05, 0x01,                     // Usage Page (Generic Desktop)
        0x09, 0x04,                     // Usage (Joystick)
        0xA1, 0x01,                     // Collection (Application)
        0x15, 0x00,                     // Logical Minimum (0)
        0x25, 0x01,                     // Logical Maximum (1)
        0x75, 0x01,                     // Report Size (1)
        0x95, 0x20,                     // Report Count (32)
        0x05, 0x09,                     // Usage Page (Button)
        0x19, 0x01,                     // Usage Minimum (Button #1)
        0x29, 0x20,                     // Usage Maximum (Button #32)
        0x81, 0x02,                     // Input (variable,absolute)
        0x15, 0x00,                     // Logical Minimum (0)
        0x25, 0x07,                     // Logical Maximum (7)
        0x35, 0x00,                     // Physical Minimum (0)
        0x46, 0x3B, 0x01,               // Physical Maximum (315)
        0x75, 0x04,                     // Report Size (4)
        0x95, 0x01,                     // Report Count (1)
        0x65, 0x14,                     // Unit (20)
        0x05, 0x01,                     // Usage Page (Generic Desktop)
        0x09, 0x39,                     // Usage (Hat switch)
        0x81, 0x42,                     // Input (variable,absolute,null_state)
        0x05, 0x01,                     // Usage Page (Generic Desktop)
        0x09, 0x01,                     // Usage (Pointer)
        0xA1, 0x00,                     // Collection ()
        0x15, 0x00,                     //   Logical Minimum (0)
        0x26, 0xFF, 0x03,               //   Logical Maximum (1023)
        0x75, 0x0A,                     //   Report Size (10)
        0x95, 0x04,                     //   Report Count (4)
        0x09, 0x30,                     //   Usage (X)
        0x09, 0x31,                     //   Usage (Y)
        0x09, 0x32,                     //   Usage (Z)
        0x09, 0x35,                     //   Usage (Rz)
        0x81, 0x02,                     //   Input (variable,absolute)
        0xC0,                           // End Collection
        0x15, 0x00,                     // Logical Minimum (0)
        0x26, 0xFF, 0x03,               // Logical Maximum (1023)
        0x75, 0x0A,                     // Report Size (10)
        0x95, 0x02,                     // Report Count (2)
        0x09, 0x36,                     // Usage (Slider)
        0x09, 0x36,                     // Usage (Slider)
        0x81, 0x02,                     // Input (variable,absolute)
        0xC0                            // End Collection
};
#endif

#ifdef SEREMU_INTERFACE
static uint8_t seremu_report_desc[] = {
        0x06, 0xC9, 0xFF,                       // Usage Page 0xFFC9 (vendor defined)
        0x09, 0x04,                             // Usage 0x04
        0xA1, 0x5C,                             // Collection 0x5C
        0x75, 0x08,                             // report size = 8 bits (global)
        0x15, 0x00,                             // logical minimum = 0 (global)
        0x26, 0xFF, 0x00,                       // logical maximum = 255 (global)
        0x95, SEREMU_TX_SIZE,                   // report count (global)
        0x09, 0x75,                             // usage (local)
        0x81, 0x02,                             // Input
        0x95, SEREMU_RX_SIZE,                   // report count (global)
        0x09, 0x76,                             // usage (local)
        0x91, 0x02,                             // Output
        0x95, 0x04,                             // report count (global)
        0x09, 0x76,                             // usage (local)
        0xB1, 0x02,                             // Feature
        0xC0                                    // end collection
};
#endif

#ifdef RAWHID_INTERFACE
static uint8_t rawhid_report_desc[] = {
        0x06, LSB(RAWHID_USAGE_PAGE), MSB(RAWHID_USAGE_PAGE),
        0x0A, LSB(RAWHID_USAGE), MSB(RAWHID_USAGE),
        0xA1, 0x01,                             // Collection 0x01
        0x75, 0x08,                             // report size = 8 bits
        0x15, 0x00,                             // logical minimum = 0
        0x26, 0xFF, 0x00,                       // logical maximum = 255
        0x95, RAWHID_TX_SIZE,                   // report count
        0x09, 0x01,                             // usage
        0x81, 0x02,                             // Input (array)
        0x95, RAWHID_RX_SIZE,                   // report count
        0x09, 0x02,                             // usage
        0x91, 0x02,                             // Output (array)
        0xC0                                    // end collection
};
#endif

#ifdef FLIGHTSIM_INTERFACE
static uint8_t flightsim_report_desc[] = {
        0x06, 0x1C, 0xFF,                       // Usage page = 0xFF1C
        0x0A, 0x39, 0xA7,                       // Usage = 0xA739
        0xA1, 0x01,                             // Collection 0x01
        0x75, 0x08,                             // report size = 8 bits
        0x15, 0x00,                             // logical minimum = 0
        0x26, 0xFF, 0x00,                       // logical maximum = 255
        0x95, FLIGHTSIM_TX_SIZE,                // report count
        0x09, 0x01,                             // usage
        0x81, 0x02,                             // Input (array)
        0x95, FLIGHTSIM_RX_SIZE,                // report count
        0x09, 0x02,                             // usage
        0x91, 0x02,                             // Output (array)
        0xC0                                    // end collection
};
#endif


// **************************************************************
//   USB Descriptor Sizes
// **************************************************************

// pre-compute the size and position of everything in the config descriptor
//
#define CONFIG_HEADER_DESCRIPTOR_SIZE    9

#define CDC_IAD_DESCRIPTOR_POS        CONFIG_HEADER_DESCRIPTOR_SIZE
#ifdef  CDC_IAD_DESCRIPTOR
    #define CDC_IAD_DESCRIPTOR_SIZE        8
#else
    #define CDC_IAD_DESCRIPTOR_SIZE        0
#endif

#define CDC_DATA_INTERFACE_DESC_POS    CDC_IAD_DESCRIPTOR_POS+CDC_IAD_DESCRIPTOR_SIZE
#ifdef  CDC_DATA_INTERFACE
    #define CDC_DATA_INTERFACE_DESC_SIZE    9+5+5+4+5+7+9+7+7
#else
    #define CDC_DATA_INTERFACE_DESC_SIZE    0
#endif

#define MIDI_INTERFACE_DESC_POS        CDC_DATA_INTERFACE_DESC_POS+CDC_DATA_INTERFACE_DESC_SIZE
#ifdef  MIDI_INTERFACE
    #define MIDI_INTERFACE_DESC_SIZE    9+7+6+6+9+9+9+5+9+5
#else
    #define MIDI_INTERFACE_DESC_SIZE    0
#endif

#define KEYBOARD_INTERFACE_DESC_POS    MIDI_INTERFACE_DESC_POS+MIDI_INTERFACE_DESC_SIZE
#ifdef  KEYBOARD_INTERFACE
    #define KEYBOARD_INTERFACE_DESC_SIZE    9+9+7
    #define KEYBOARD_HID_DESC_OFFSET    KEYBOARD_INTERFACE_DESC_POS+9
#else
    #define KEYBOARD_INTERFACE_DESC_SIZE    0
#endif

#define MOUSE_INTERFACE_DESC_POS    KEYBOARD_INTERFACE_DESC_POS+KEYBOARD_INTERFACE_DESC_SIZE
#ifdef  MOUSE_INTERFACE
    #define MOUSE_INTERFACE_DESC_SIZE    9+9+7
    #define MOUSE_HID_DESC_OFFSET        MOUSE_INTERFACE_DESC_POS+9
#else
    #define MOUSE_INTERFACE_DESC_SIZE    0
#endif

#define RAWHID_INTERFACE_DESC_POS    MOUSE_INTERFACE_DESC_POS+MOUSE_INTERFACE_DESC_SIZE
#ifdef  RAWHID_INTERFACE
    #define RAWHID_INTERFACE_DESC_SIZE    9+9+7+7
    #define RAWHID_HID_DESC_OFFSET        RAWHID_INTERFACE_DESC_POS+9
#else
    #define RAWHID_INTERFACE_DESC_SIZE    0
#endif

#define FLIGHTSIM_INTERFACE_DESC_POS    RAWHID_INTERFACE_DESC_POS+RAWHID_INTERFACE_DESC_SIZE
#ifdef  FLIGHTSIM_INTERFACE
    #define FLIGHTSIM_INTERFACE_DESC_SIZE    9+9+7+7
    #define FLIGHTSIM_HID_DESC_OFFSET    FLIGHTSIM_INTERFACE_DESC_POS+9
#else
    #define FLIGHTSIM_INTERFACE_DESC_SIZE    0
#endif

#define SEREMU_INTERFACE_DESC_POS    FLIGHTSIM_INTERFACE_DESC_POS+FLIGHTSIM_INTERFACE_DESC_SIZE
#ifdef  SEREMU_INTERFACE
    #define SEREMU_INTERFACE_DESC_SIZE    9+9+7+7
    #define SEREMU_HID_DESC_OFFSET        SEREMU_INTERFACE_DESC_POS+9
#else
    #define SEREMU_INTERFACE_DESC_SIZE    0
#endif

#define JOYSTICK_INTERFACE_DESC_POS    SEREMU_INTERFACE_DESC_POS+SEREMU_INTERFACE_DESC_SIZE
#ifdef  JOYSTICK_INTERFACE
    #define JOYSTICK_INTERFACE_DESC_SIZE    9+9+7
    #define JOYSTICK_HID_DESC_OFFSET    JOYSTICK_INTERFACE_DESC_POS+9
#else
    #define JOYSTICK_INTERFACE_DESC_SIZE    0
#endif

// Added by Elm�
#define TOUCH_INTERFACE_DESC_POS    JOYSTICK_INTERFACE_DESC_POS+JOYSTICK_INTERFACE_DESC_SIZE
#ifdef  TOUCH_INTERFACE
    #define TOUCH_INTERFACE_DESC_SIZE    9+9+7
    #define TOUCH_HID_DESC_OFFSET    TOUCH_INTERFACE_DESC_POS+9
#else
    #define TOUCH_INTERFACE_DESC_SIZE    0
#endif

#define CONFIG_DESC_SIZE        TOUCH_INTERFACE_DESC_POS+TOUCH_INTERFACE_DESC_SIZE



// **************************************************************
//   USB Configuration
// **************************************************************

// USB Configuration Descriptor.  This huge descriptor tells all
// of the devices capbilities.
static uint8_t config_descriptor[CONFIG_DESC_SIZE] = {
        // configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
        9,                                      // bLength;
        2,                                      // bDescriptorType;
        LSB(CONFIG_DESC_SIZE),                 // wTotalLength
        MSB(CONFIG_DESC_SIZE),
        NUM_INTERFACE,                          // bNumInterfaces
        1,                                      // bConfigurationValue
        0,                                      // iConfiguration
        0x80,                                   // bmAttributes  (NOT self powered, no wake up)
        50,                                     // bMaxPower

#ifdef CDC_IAD_DESCRIPTOR
        // interface association descriptor, USB ECN, Table 9-Z
        8,                                      // bLength
        11,                                     // bDescriptorType
        CDC_STATUS_INTERFACE,                   // bFirstInterface
        2,                                      // bInterfaceCount
        0x02,                                   // bFunctionClass
        0x02,                                   // bFunctionSubClass
        0x01,                                   // bFunctionProtocol
        4,                                      // iFunction
#endif

#ifdef CDC_DATA_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        CDC_STATUS_INTERFACE,            // bInterfaceNumber
        0,                                      // bAlternateSetting
        1,                                      // bNumEndpoints
        0x02,                                   // bInterfaceClass
        0x02,                                   // bInterfaceSubClass
        0x01,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // CDC Header Functional Descriptor, CDC Spec 5.2.3.1, Table 26
        5,                                      // bFunctionLength
        0x24,                                   // bDescriptorType
        0x00,                                   // bDescriptorSubtype
        0x10, 0x01,                             // bcdCDC
        // Call Management Functional Descriptor, CDC Spec 5.2.3.2, Table 27
        5,                                      // bFunctionLength
        0x24,                                   // bDescriptorType
        0x01,                                   // bDescriptorSubtype
        0x01,                                   // bmCapabilities
        1,                                      // bDataInterface
        // Abstract Control Management Functional Descriptor, CDC Spec 5.2.3.3, Table 28
        4,                                      // bFunctionLength
        0x24,                                   // bDescriptorType
        0x02,                                   // bDescriptorSubtype
        0x06,                                   // bmCapabilities
        // Union Functional Descriptor, CDC Spec 5.2.3.8, Table 33
        5,                                      // bFunctionLength
        0x24,                                   // bDescriptorType
        0x06,                                   // bDescriptorSubtype
        CDC_STATUS_INTERFACE,                   // bMasterInterface
        CDC_DATA_INTERFACE,                     // bSlaveInterface0
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        CDC_ACM_ENDPOINT | 0x80,                // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        CDC_ACM_SIZE, 0,                        // wMaxPacketSize
        64,                                     // bInterval
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        CDC_DATA_INTERFACE,                     // bInterfaceNumber
        0,                                      // bAlternateSetting
        2,                                      // bNumEndpoints
        0x0A,                                   // bInterfaceClass
        0x00,                                   // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        CDC_RX_ENDPOINT,                        // bEndpointAddress
        0x02,                                   // bmAttributes (0x02=bulk)
        CDC_RX_SIZE, 0,                         // wMaxPacketSize
        0,                                      // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        CDC_TX_ENDPOINT | 0x80,                 // bEndpointAddress
        0x02,                                   // bmAttributes (0x02=bulk)
        CDC_TX_SIZE, 0,                         // wMaxPacketSize
        0,                                      // bInterval
#endif // CDC_DATA_INTERFACE

#ifdef MIDI_INTERFACE
        // Standard MS Interface Descriptor,
        9,                                      // bLength
        4,                                      // bDescriptorType
        MIDI_INTERFACE,                         // bInterfaceNumber
        0,                                      // bAlternateSetting
        2,                                      // bNumEndpoints
        0x01,                                   // bInterfaceClass (0x01 = Audio)
        0x03,                                   // bInterfaceSubClass (0x03 = MIDI)
        0x00,                                   // bInterfaceProtocol (unused for MIDI)
        0,                                      // iInterface
        // MIDI MS Interface Header, USB MIDI 6.1.2.1, page 21, Table 6-2
        7,                                      // bLength
        0x24,                                   // bDescriptorType = CS_INTERFACE
        0x01,                                   // bDescriptorSubtype = MS_HEADER
        0x00, 0x01,                             // bcdMSC = revision 01.00
        0x41, 0x00,                             // wTotalLength
        // MIDI IN Jack Descriptor, B.4.3, Table B-7 (embedded), page 40
        6,                                      // bLength
        0x24,                                   // bDescriptorType = CS_INTERFACE
        0x02,                                   // bDescriptorSubtype = MIDI_IN_JACK
        0x01,                                   // bJackType = EMBEDDED
        1,                                      // bJackID, ID = 1
        0,                                      // iJack
        // MIDI IN Jack Descriptor, B.4.3, Table B-8 (external), page 40
        6,                                      // bLength
        0x24,                                   // bDescriptorType = CS_INTERFACE
        0x02,                                   // bDescriptorSubtype = MIDI_IN_JACK
        0x02,                                   // bJackType = EXTERNAL
        2,                                      // bJackID, ID = 2
        0,                                      // iJack
        // MIDI OUT Jack Descriptor, B.4.4, Table B-9, page 41
        9,
        0x24,                                   // bDescriptorType = CS_INTERFACE
        0x03,                                   // bDescriptorSubtype = MIDI_OUT_JACK
        0x01,                                   // bJackType = EMBEDDED
        3,                                      // bJackID, ID = 3
        1,                                      // bNrInputPins = 1 pin
        2,                                      // BaSourceID(1) = 2
        1,                                      // BaSourcePin(1) = first pin
        0,                                      // iJack
        // MIDI OUT Jack Descriptor, B.4.4, Table B-10, page 41
        9,
        0x24,                                   // bDescriptorType = CS_INTERFACE
        0x03,                                   // bDescriptorSubtype = MIDI_OUT_JACK
        0x02,                                   // bJackType = EXTERNAL
        4,                                      // bJackID, ID = 4
        1,                                      // bNrInputPins = 1 pin
        1,                                      // BaSourceID(1) = 1
        1,                                      // BaSourcePin(1) = first pin
        0,                                      // iJack
        // Standard Bulk OUT Endpoint Descriptor, B.5.1, Table B-11, pae 42
        9,                                      // bLength
        5,                                      // bDescriptorType = ENDPOINT
        MIDI_RX_ENDPOINT,                       // bEndpointAddress
        0x02,                                   // bmAttributes (0x02=bulk)
        MIDI_RX_SIZE, 0,                        // wMaxPacketSize
        0,                                      // bInterval
        0,                                      // bRefresh
        0,                                      // bSynchAddress
        // Class-specific MS Bulk OUT Endpoint Descriptor, B.5.2, Table B-12, page 42
        5,                                      // bLength
        0x25,                                   // bDescriptorSubtype = CS_ENDPOINT
        0x01,                                   // bJackType = MS_GENERAL
        1,                                      // bNumEmbMIDIJack = 1 jack
        1,                                      // BaAssocJackID(1) = jack ID #1
        // Standard Bulk IN Endpoint Descriptor, B.5.1, Table B-11, pae 42
        9,                                      // bLength
        5,                                      // bDescriptorType = ENDPOINT
        MIDI_TX_ENDPOINT | 0x80,                // bEndpointAddress
        0x02,                                   // bmAttributes (0x02=bulk)
        MIDI_TX_SIZE, 0,                        // wMaxPacketSize
        0,                                      // bInterval
        0,                                      // bRefresh
        0,                                      // bSynchAddress
        // Class-specific MS Bulk IN Endpoint Descriptor, B.5.2, Table B-12, page 42
        5,                                      // bLength
        0x25,                                   // bDescriptorSubtype = CS_ENDPOINT
        0x01,                                   // bJackType = MS_GENERAL
        1,                                      // bNumEmbMIDIJack = 1 jack
        3,                                      // BaAssocJackID(1) = jack ID #3
#endif // MIDI_INTERFACE

#ifdef KEYBOARD_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        KEYBOARD_INTERFACE,                     // bInterfaceNumber
        0,                                      // bAlternateSetting
        1,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x01,                                   // bInterfaceSubClass (0x01 = Boot)
        0x01,                                   // bInterfaceProtocol (0x01 = Keyboard)
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(keyboard_report_desc)),      // wDescriptorLength
        MSB(sizeof(keyboard_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        KEYBOARD_ENDPOINT | 0x80,               // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        KEYBOARD_SIZE, 0,                       // wMaxPacketSize
        KEYBOARD_INTERVAL,                      // bInterval
#endif // KEYBOARD_INTERFACE

// Modified by Elm�
#ifdef MOUSE_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        MOUSE_INTERFACE,                        // bInterfaceNumber
        0,                                      // bAlternateSetting
        1,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x01,                                   // bInterfaceSubClass (0x01 = Boot)
        0x02,                                   // bInterfaceProtocol (0x02 = Mouse)
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(mouse_report_desc)),         // wDescriptorLength
        MSB(sizeof(mouse_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        MOUSE_ENDPOINT | 0x80,                  // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        MOUSE_SIZE, 0,                          // wMaxPacketSize
        MOUSE_INTERVAL,                         // bInterval
#endif // MOUSE_INTERFACE

#ifdef RAWHID_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        RAWHID_INTERFACE,                       // bInterfaceNumber
        0,                                      // bAlternateSetting
        2,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x00,                                   // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(rawhid_report_desc)),        // wDescriptorLength
        MSB(sizeof(rawhid_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        RAWHID_TX_ENDPOINT | 0x80,              // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        RAWHID_TX_SIZE, 0,                      // wMaxPacketSize
        RAWHID_TX_INTERVAL,                     // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        RAWHID_RX_ENDPOINT,                     // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        RAWHID_RX_SIZE, 0,                      // wMaxPacketSize
        RAWHID_RX_INTERVAL,            // bInterval
#endif // RAWHID_INTERFACE

#ifdef FLIGHTSIM_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        FLIGHTSIM_INTERFACE,                    // bInterfaceNumber
        0,                                      // bAlternateSetting
        2,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x00,                                   // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(flightsim_report_desc)),     // wDescriptorLength
        MSB(sizeof(flightsim_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        FLIGHTSIM_TX_ENDPOINT | 0x80,           // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        FLIGHTSIM_TX_SIZE, 0,                   // wMaxPacketSize
        FLIGHTSIM_TX_INTERVAL,                  // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        FLIGHTSIM_RX_ENDPOINT,                  // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        FLIGHTSIM_RX_SIZE, 0,                   // wMaxPacketSize
        FLIGHTSIM_RX_INTERVAL,            // bInterval
#endif // FLIGHTSIM_INTERFACE

#ifdef SEREMU_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        SEREMU_INTERFACE,                       // bInterfaceNumber
        0,                                      // bAlternateSetting
        2,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x00,                                   // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(seremu_report_desc)),        // wDescriptorLength
        MSB(sizeof(seremu_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        SEREMU_TX_ENDPOINT | 0x80,              // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        SEREMU_TX_SIZE, 0,                      // wMaxPacketSize
        SEREMU_TX_INTERVAL,                     // bInterval
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        SEREMU_RX_ENDPOINT,                     // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        SEREMU_RX_SIZE, 0,                      // wMaxPacketSize
        SEREMU_RX_INTERVAL,            // bInterval
#endif // SEREMU_INTERFACE

#ifdef JOYSTICK_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        JOYSTICK_INTERFACE,                     // bInterfaceNumber
        0,                                      // bAlternateSetting
        1,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x00,                                   // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x11, 0x01,                             // bcdHID
        0,                                      // bCountryCode
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(joystick_report_desc)),      // wDescriptorLength
        MSB(sizeof(joystick_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        JOYSTICK_ENDPOINT | 0x80,               // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        JOYSTICK_SIZE, 0,                       // wMaxPacketSize
        JOYSTICK_INTERVAL,                      // bInterval
#endif // JOYSTICK_INTERFACE

// Added by Elm�
#ifdef TOUCH_INTERFACE
        // interface descriptor, USB spec 9.6.5, page 267-269, Table 9-12
        9,                                      // bLength
        4,                                      // bDescriptorType
        TOUCH_INTERFACE,                        // bInterfaceNumber
        0,                                      // bAlternateSetting
        1,                                      // bNumEndpoints
        0x03,                                   // bInterfaceClass (0x03 = HID)
        0x00,                                   // bInterfaceSubClass
        0x00,                                   // bInterfaceProtocol
        0,                                      // iInterface
        // HID interface descriptor, HID 1.11 spec, section 6.2.1
        9,                                      // bLength
        0x21,                                   // bDescriptorType
        0x00, 0x01,                             // bcdHID (version 1.00)
        0x21,                                   // bCountryCode  (US)
        1,                                      // bNumDescriptors
        0x22,                                   // bDescriptorType
        LSB(sizeof(touch_report_desc)),         // wDescriptorLength
        MSB(sizeof(touch_report_desc)),
        // endpoint descriptor, USB spec 9.6.6, page 269-271, Table 9-13
        7,                                      // bLength
        5,                                      // bDescriptorType
        TOUCH_ENDPOINT | 0x80,                  // bEndpointAddress
        0x03,                                   // bmAttributes (0x03=intr)
        TOUCH_SIZE, 0,                          // wMaxPacketSize
        TOUCH_INTERVAL,                         // bInterval
#endif // TOUCH_INTERFACE

};


// **************************************************************
//   String Descriptors
// **************************************************************

// The descriptors above can provide human readable strings,
// referenced by index numbers.  These descriptors are the
// actual string data

/* defined in usb_names.h
struct usb_string_descriptor_struct {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wString[];
};
*/

extern struct usb_string_descriptor_struct usb_string_manufacturer_name
        __attribute__ ((weak, alias("usb_string_manufacturer_name_default")));
extern struct usb_string_descriptor_struct usb_string_product_name
        __attribute__ ((weak, alias("usb_string_product_name_default")));
extern struct usb_string_descriptor_struct usb_string_serial_number
        __attribute__ ((weak, alias("usb_string_serial_number_default")));

struct usb_string_descriptor_struct string0 = {
        4,
        3,
        {0x0409}
};

struct usb_string_descriptor_struct usb_string_manufacturer_name_default = {
        2 + MANUFACTURER_NAME_LEN * 2,
        3,
        MANUFACTURER_NAME
};
struct usb_string_descriptor_struct usb_string_product_name_default = {
    2 + PRODUCT_NAME_LEN * 2,
        3,
        PRODUCT_NAME
};
struct usb_string_descriptor_struct usb_string_serial_number_default = {
        12,
        3,
        {0,0,0,0,0,0,0,0,0,0}
};

void usb_init_serialnumber(void)
{
    char buf[11];
    uint32_t i, num;

    __disable_irq();
    FTFL_FSTAT = FTFL_FSTAT_RDCOLERR | FTFL_FSTAT_ACCERR | FTFL_FSTAT_FPVIOL;
    FTFL_FCCOB0 = 0x41;
    FTFL_FCCOB1 = 15;
    FTFL_FSTAT = FTFL_FSTAT_CCIF;
    while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF)) ; // wait
    num = *(uint32_t *)&FTFL_FCCOB7;
    __enable_irq();
    // add extra zero to work around OS-X CDC-ACM driver bug
    if (num < 10000000) num = num * 10;
    ultoa(num, buf, 10);
    for (i=0; i<10; i++) {
        char c = buf[i];
        if (!c) break;
        usb_string_serial_number_default.wString[i] = c;
    }
    usb_string_serial_number_default.bLength = i * 2 + 2;
}


// **************************************************************
//   Descriptors List
// **************************************************************

// This table provides access to all the descriptor data above.

const usb_descriptor_list_t usb_descriptor_list[] = {
    //wValue, wIndex, address,          length
    {0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
    {0x0200, 0x0000, config_descriptor, sizeof(config_descriptor)},
#ifdef SEREMU_INTERFACE
    {0x2200, SEREMU_INTERFACE, seremu_report_desc, sizeof(seremu_report_desc)},
    {0x2100, SEREMU_INTERFACE, config_descriptor+SEREMU_HID_DESC_OFFSET, 9},
#endif
#ifdef KEYBOARD_INTERFACE
    {0x2200, KEYBOARD_INTERFACE, keyboard_report_desc, sizeof(keyboard_report_desc)},
    {0x2100, KEYBOARD_INTERFACE, config_descriptor+KEYBOARD_HID_DESC_OFFSET, 9},
#endif
#ifdef MOUSE_INTERFACE
    {0x2200, MOUSE_INTERFACE, mouse_report_desc, sizeof(mouse_report_desc)},
    {0x2100, MOUSE_INTERFACE, config_descriptor+MOUSE_HID_DESC_OFFSET, 9},
#endif
#ifdef JOYSTICK_INTERFACE
    {0x2200, JOYSTICK_INTERFACE, joystick_report_desc, sizeof(joystick_report_desc)},
    {0x2100, JOYSTICK_INTERFACE, config_descriptor+JOYSTICK_HID_DESC_OFFSET, 9},
#endif
#ifdef TOUCH_INTERFACE
    {0x2200, TOUCH_INTERFACE, touch_report_desc, sizeof(touch_report_desc)},
    {0x2100, TOUCH_INTERFACE, config_descriptor+TOUCH_HID_DESC_OFFSET, 9},
#endif
#ifdef RAWHID_INTERFACE
    {0x2200, RAWHID_INTERFACE, rawhid_report_desc, sizeof(rawhid_report_desc)},
    {0x2100, RAWHID_INTERFACE, config_descriptor+RAWHID_HID_DESC_OFFSET, 9},
#endif
#ifdef FLIGHTSIM_INTERFACE
    {0x2200, FLIGHTSIM_INTERFACE, flightsim_report_desc, sizeof(flightsim_report_desc)},
    {0x2100, FLIGHTSIM_INTERFACE, config_descriptor+FLIGHTSIM_HID_DESC_OFFSET, 9},
#endif
    {0x0300, 0x0000, (const uint8_t *)&string0, 0},
    {0x0301, 0x0409, (const uint8_t *)&usb_string_manufacturer_name, 0},
    {0x0302, 0x0409, (const uint8_t *)&usb_string_product_name, 0},
    {0x0303, 0x0409, (const uint8_t *)&usb_string_serial_number, 0},
    {0, 0, NULL, 0}
};


// **************************************************************
//   Endpoint Configuration
// **************************************************************

const uint8_t usb_endpoint_config_table[NUM_ENDPOINTS] =
{
#if (defined(ENDPOINT1_CONFIG) && NUM_ENDPOINTS >= 1)
    ENDPOINT1_CONFIG,
#elif (NUM_ENDPOINTS >= 1)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT2_CONFIG) && NUM_ENDPOINTS >= 2)
    ENDPOINT2_CONFIG,
#elif (NUM_ENDPOINTS >= 2)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT3_CONFIG) && NUM_ENDPOINTS >= 3)
    ENDPOINT3_CONFIG,
#elif (NUM_ENDPOINTS >= 3)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT4_CONFIG) && NUM_ENDPOINTS >= 4)
    ENDPOINT4_CONFIG,
#elif (NUM_ENDPOINTS >= 4)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT5_CONFIG) && NUM_ENDPOINTS >= 5)
    ENDPOINT5_CONFIG,
#elif (NUM_ENDPOINTS >= 5)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT6_CONFIG) && NUM_ENDPOINTS >= 6)
    ENDPOINT6_CONFIG,
#elif (NUM_ENDPOINTS >= 6)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT7_CONFIG) && NUM_ENDPOINTS >= 7)
    ENDPOINT7_CONFIG,
#elif (NUM_ENDPOINTS >= 7)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT8_CONFIG) && NUM_ENDPOINTS >= 8)
    ENDPOINT8_CONFIG,
#elif (NUM_ENDPOINTS >= 8)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT9_CONFIG) && NUM_ENDPOINTS >= 9)
    ENDPOINT9_CONFIG,
#elif (NUM_ENDPOINTS >= 9)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT10_CONFIG) && NUM_ENDPOINTS >= 10)
    ENDPOINT10_CONFIG,
#elif (NUM_ENDPOINTS >= 10)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT11_CONFIG) && NUM_ENDPOINTS >= 11)
    ENDPOINT11_CONFIG,
#elif (NUM_ENDPOINTS >= 11)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT12_CONFIG) && NUM_ENDPOINTS >= 12)
    ENDPOINT12_CONFIG,
#elif (NUM_ENDPOINTS >= 12)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT13_CONFIG) && NUM_ENDPOINTS >= 13)
    ENDPOINT13_CONFIG,
#elif (NUM_ENDPOINTS >= 13)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT14_CONFIG) && NUM_ENDPOINTS >= 14)
    ENDPOINT14_CONFIG,
#elif (NUM_ENDPOINTS >= 14)
    ENDPOINT_UNUSED,
#endif
#if (defined(ENDPOINT15_CONFIG) && NUM_ENDPOINTS >= 15)
    ENDPOINT15_CONFIG,
#elif (NUM_ENDPOINTS >= 15)
    ENDPOINT_UNUSED,
#endif
};


#endif // NUM_ENDPOINTS
#endif // F_CPU >= 20 MHz
