# AVR_ac Library

The `AVR_ac` library provides a simple interface to the Analog Comparator interrupt on an Arduino running with an ATmega328P and *probably* works on an ATmega168 as well -- they are extremely similar.

The code has been tested on projects created in the Arduino IDE version 1.8.13, and also in PlatformIO (platformio.org), with no problems on either system.

## Arduino

To install the library:

* Click Sketch->Include Library->Add .ZIP Library;
* Navigate to where the zip file for this library is located on your system;
* Select it and click the OK button;
* Follow the prompts.


## PlatformIO

As of the time of typing, I have not built this library into a format suitable for uploading to the official PlatformIO Library Registry. 

This means you have a couple of simple options, as described below.

### Install per Project

To install this library for a single project:

* Create the project in the normal manner.
* Edit the `platformio.ini` file, and add:

  ```
  lib_deps = https://github.com/NormanDunbar/AVRac^1.0.0
  ```

  This will ensure that you only pick up tagged versions from 1.0.0 and above, and not the contents of the development files which might be broken. 


Norman Dunbar
norman@dunbar-it.co.uk

27 October 2020.
