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
        0x10000 espruino_2v06.633_m5stickc.bin
"""
import pinutils;
info = {
 'name'                     : "M5STICKC",
 'espruino_page_link'       : 'ESP32',
 'default_console'          : "EV_SERIAL1",
 'default_console_baudrate' : "115200",
 'variables'                : 2500, # JSVAR_MALLOC is defined below - so this can vary depending on what is initialised
 'binary_name'              : 'espruino_%v_m5stickc.bin',
 'build' : {
   'optimizeflags' : '-Og',
   'libraries' : [
     'ESP32',
     'NET',
     'GRAPHICS',
    #  'CRYPTO','SHA256','SHA512',
    #  'TLS',
    #  'TELNET',
     'NEOPIXEL',
    #  'FILESYSTEM',
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
  'LED1' : { 'pin' : 'D10', 'inverted' : True },
  'BTN1' : { 'pin' : 'D37' },
  'BTN2' : { 'pin' : 'D39' },
  # 'LCD' : {
  #           'width' : 160, 'height' : 80, 'bpp' : 4,
  #           'controller' : 'st7735',
  #           'pin_dc' : 'D23',
  #           'pin_cs' : 'D5',
  #           'pin_rst' : 'D8',
  #           'pin_sck' : 'D13',
  #           'pin_mosi' : 'D15',
  #         },
};

def get_pins():
  # pins = [];
  pins = pinutils.generate_pins(0,39)

  pinutils.findpin(pins, "PD10", False)["functions"]["NEGATED"]=0;
  pinutils.findpin(pins, "PD5", False)["functions"]["SPI1_CS"]=0;
  pinutils.findpin(pins, "PD8", False)["functions"]["TFT_RST"]=0;
  pinutils.findpin(pins, "PD13", False)["functions"]["SPI1_SCK"]=0;
  pinutils.findpin(pins, "PD15", False)["functions"]["SPI1_MOSI"]=0;
  pinutils.findpin(pins, "PD23", False)["functions"]["TFT_DC"]=0;
  pinutils.findpin(pins, "PD21", False)["functions"]["I2C1_SDA"]=0;
  pinutils.findpin(pins, "PD22", False)["functions"]["I2C1_SCL"]=0;
  pinutils.findpin(pins, "PD37", False);
  pinutils.findpin(pins, "PD39", False);

  # everything is non-5v tolerant
  for pin in pins:
    pin["functions"]["3.3"]=0;

  return pins
