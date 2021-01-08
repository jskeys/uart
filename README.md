In order to use an FTDI device on a Raspberry Pi, you may have to unload the ftdi and usb serial modules:

- sudo rmmod ftdi_sio
- sudo rmmod usbserial
