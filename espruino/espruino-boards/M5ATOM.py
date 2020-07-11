#!/bin/false
# This file is part of Espruino, a JavaScript interpreter for Microcontrollers
#
# Copyright (C) 2013 Gordon Williams <gw@pur3.co.uk>
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# ----------------------------------------------------------------------------------------
# This file contains information for a specific board - the available pins, and where LEDs,
# Buttons, and other in-built peripherals are. It is used to build documentation as well
# as various source and header files for Espruino.
# ----------------------------------------------------------------------------------------
#
# Flashing:
"""
esptool.py erase_flash && esptool.py                \
        --port /dev/ttyUSB0                         \
        --baud 1500000                              \
        --before default_reset                      \
        --after hard_reset write_flash              \
        -z                                          \
        --flash_mode dio                            \
        --flash_freq 80m                            \
        --flash_size detect                         \
        0x1000 bootloader.bin                       \
        0x8000 partitions_espruino.bin              \
        0xe000 /home/tristan/snap/arduino/41/.arduino15/packages/esp32/hardware/esp32/1.0.4/tools/partitions/boot_app0.bin \
        0x10000 espruino_2v06.633_m5atom.bin
"""

import pinutils;
info = {
 'name'                     : "M5ATOM",
 'espruino_page_link'       : 'ESP32',
 'default_console'          : "EV_SERIAL1",
 'default_console_baudrate' : "115200",
 'variables'                : 2500, # JSVAR_MALLOC is defined below - so this can vary depending on what is initialised
 'binary_name'              : 'espruino_%v_m5atom.bin',
 'build' : {
   'optimizeflags' : '-Og',
   'libraries' : [
     'ESP32',
     'NET',
     'GRAPHICS',
     'CRYPTO','SHA256','SHA512',
     'TLS',
     'TELNET',
     'NEOPIXEL',
     'FILESYSTEM',
    #  'FLASHFS',
     'BLUETOOTH'
   ],
   'makefile' : [
     'DEFINES+=-DESP_PLATFORM -DESP32=1',
     'DEFINES+=-DJSVAR_MALLOC', # Allocate space for variables at jsvInit time
     'ESP32_FLASH_MAX=1572864'
   ]
 }
};

chip = {
  'part'    : "ESP32",
  'family'  : "ESP32",
  'package' : "",
  'ram'     : 512,
  'flash'   : 0,
  'speed'   : 240,
  'usart'   : 3,
  'spi'     : 2,
  'i2c'     : 2,
  'adc'     : 2,
  'dac'     : 0,
  'saved_code' : {
    'address' : 0x320000,
    'page_size' : 4096,
    'pages' : 64,
    'flash_available' : 1344, # firmware can be up to this size - see partitions_espruino.csv
  },
};
devices = {
  'BTN1' : { 'pin' : 'D39', 'pinstate' : 'IN_PULLDOWN' },
};

def get_pins():
  pins = pinutils.generate_pins(0,39) # 40 General Purpose I/O Pins.

  pinutils.findpin(pins, "PD39", True)["functions"]["NEGATED"]=0;
  # pins = [
  #   { "name":"PD19", "sortingname":"D19", "port":"D", "num":"19", "functions":{}, "csv":{} },
  #   { "name":"PD21", "sortingname":"D21", "port":"D", "num":"21", "functions":{}, "csv":{} },
  #   { "name":"PD22", "sortingname":"D22", "port":"D", "num":"22", "functions":{}, "csv":{} },
  #   { "name":"PD23", "sortingname":"D23", "port":"D", "num":"23", "functions":{}, "csv":{} },
  #   { "name":"PD25", "sortingname":"D25", "port":"D", "num":"25", "functions":{}, "csv":{} },
  #   { "name":"PD33", "sortingname":"D33", "port":"D", "num":"33", "functions":{}, "csv":{} },
  #   { "name":"PD39", "sortingname":"D39", "port":"D", "num":"39", "functions":{}, "csv":{} },
  # ];

  # everything is non-5v tolerant
  for pin in pins:
   pin["functions"]["3.3"]=0;

  return pins
