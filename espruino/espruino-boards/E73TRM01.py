#!/bin/false
# This file is part of Espruino, a JavaScript interpreter for Microcontrollers
#
# Copyright (C) 2013 Gordon Williams <gw@pur3.co.uk>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# also see http://forum.espruino.com/comments/15016862/ and
# https://github.com/AkosLukacs/espruinostuff/blob/master/boards/E73TRM01.py
#
# # Board file for EBYTE's E73-TRM-01 based on the MDBT42Q.
# # There is no bootloader or anything, you need an SWD programmer to program it.
#
# # BUILD it:
# > make clean && BOARD=E73TRM01 RELEASE=1 BOOTLOADER=1 make
# > make clean && BOARD=E73TRM01 RELEASE=1 make
#
# # unlock flash in case it's locked:
# > nrfjprog --recover
# # and flash it with an SWD programmer...

# Found this in a pastebin:
# Can't find EBYTE test board E73-TRM-01 V1.0 schematic, so figured out connections by myself:
#
#  | NRF52832 | E73-2G4M04S1B |   Connected to  |
#  | -------- | ------------- | --------------- |
#  |   P0.05  |       21      | CH340G CTS      |
#  |   P0.06  |       22      | CH340G RX       |
#  |   P0.07  |       23      | CH340G RTS      |
#  |   P0.08  |       24      | CH340G TX       |
#  |   P0.13  |       29      | SW2 (act. low)  |
#  |   P0.14  |       30      | SW1 (act. low)  |
#  |   P0.17  |       33      | LED1 (act. low) |
#  |   P0.18  |       34      | LED2 (act. low) |
#

import pinutils;

info = {
 'name' : "E73TRM01",
 'link' :  [ "http://ebyte.com/en/product-view-news.aspx?id=243" ],
 'espruino_page_link' : '',
 'default_console' : "EV_SERIAL1",
 'default_console_tx' : "D6",
 'default_console_rx' : "D8",
 'default_console_baudrate' : "9600",
 'variables' : 2500, # How many variables are allocated for Espruino to use. RAM will be overflowed if this number is too high and code won't compile.
 #'bootloader' : 1,
 'binary_name' : 'espruino_%v_e73trm01.hex',
 'build' : {
   'optimizeflags' : '-Os',
   'libraries' : [
     'BLUETOOTH',
     'NET',
     'GRAPHICS',
     'CRYPTO','SHA256','SHA512',
     'AES',
     'NFC',
     'NEOPIXEL',
     'FILESYSTEM'
     #'TLS'
   ],
   'makefile' : [
     'DEFINES+=-DHAL_NFC_ENGINEERING_BC_FTPAN_WORKAROUND=1', # Looks like proper production nRF52s had this issue
     'DEFINES+=-DCONFIG_GPIO_AS_PINRESET', # Allow the reset pin to work
     'DEFINES+=-DBLUETOOTH_NAME_PREFIX=\'"E73-TRM-01"\'',
   ]
 }
};

chip = {
  'part' : "NRF52832",
  'family' : "NRF52",
  'package' : "QFN48",
  'ram' : 64,
  'flash' : 512,
  'speed' : 64,
  'usart' : 1,
  'spi' : 1,
  'i2c' : 1,
  'adc' : 1,
  'dac' : 0,
  'saved_code' : {
    'address' : ((118 - 10) * 4096), # Bootloader takes pages 120-127, FS takes 118-119
    'page_size' : 4096,
    'pages' : 10,
    'flash_available' : 512 - ((31 + 8 + 2 + 10)*4) # Softdevice uses 31 pages of flash, bootloader 8, FS 2, code 10. Each page is 4 kb.
  },
};

devices = {
  'LED1' : { 'pin' : 'D17' },
  'LED2' : { 'pin' : 'D18' },
  'BTN1' : { 'pin' : 'D14', 'pinstate' : 'IN_PULLDOWN' },
  'BTN2' : { 'pin' : 'D13', 'pinstate' : 'IN_PULLDOWN' },
  'NFC': { 'pin_a':'D9', 'pin_b':'D10' },
  'RX_PIN_NUMBER' : { 'pin' : 'D8'},
  'TX_PIN_NUMBER' : { 'pin' : 'D6'},
  'CTS_PIN_NUMBER' : { 'pin' : 'D7'},
  'RTS_PIN_NUMBER' : { 'pin' : 'D5'},
  # Pin D22 is used for clock when driving neopixels - as not specifying a pin seems to break things
};

def get_pins():
  pins = pinutils.generate_pins(0,31) # 32 General Purpose I/O Pins.
  pinutils.findpin(pins, "PD0", True)["functions"]["XL1"]=0;
  pinutils.findpin(pins, "PD1", True)["functions"]["XL2"]=0;
  pinutils.findpin(pins, "PD9", True)["functions"]["NFC1"]=0;
  pinutils.findpin(pins, "PD10", True)["functions"]["NFC2"]=0;
  pinutils.findpin(pins, "PD2", True)["functions"]["ADC1_IN0"]=0;
  pinutils.findpin(pins, "PD3", True)["functions"]["ADC1_IN1"]=0;
  pinutils.findpin(pins, "PD4", True)["functions"]["ADC1_IN2"]=0;
  pinutils.findpin(pins, "PD5", True)["functions"]["ADC1_IN3"]=0;
  pinutils.findpin(pins, "PD28", True)["functions"]["ADC1_IN4"]=0;
  pinutils.findpin(pins, "PD6", True)["functions"]["USART1_TX"]=0;
  pinutils.findpin(pins, "PD8", True)["functions"]["USART1_RX"]=0;
  pinutils.findpin(pins, "PD29", True)["functions"]["ADC1_IN5"]=0;
  pinutils.findpin(pins, "PD30", True)["functions"]["ADC1_IN6"]=0;
  pinutils.findpin(pins, "PD31", True)["functions"]["ADC1_IN7"]=0;

  pinutils.findpin(pins, "PD13", True)["functions"]["NEGATED"]=0;
  pinutils.findpin(pins, "PD14", True)["functions"]["NEGATED"]=0;
  pinutils.findpin(pins, "PD17", True)["functions"]["NEGATED"]=0;
  pinutils.findpin(pins, "PD18", True)["functions"]["NEGATED"]=0;
  # everything is non-5v tolerant
  for pin in pins:
    pin["functions"]["3.3"]=0;

  #The boot/reset button will function as a reset button in normal operation. Pin reset on PD21 needs to be enabled on the nRF52832 device for this to work.
  return pins
