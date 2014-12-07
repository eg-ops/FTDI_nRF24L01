FTDI_nRF24L01
=============

nRF24L01 configuration tool


![alt tag](http://ecx.images-amazon.com/images/I/519-VsWyIPL._SY300_.jpg)

This qt based tool allow you to connect nRF24L01 or nRF24L01+ to you windows/linux PC using widespread FTDI(FT232R and FT245R) usb to serial converter.
For development I'm using module like this, any other module should work too.

![alt tag](http://i00.i.aliimg.com/photo/v1/705790468/FT232RL_module_USB_to_TTL_UART_3.jpg)

Features
========

- receiving packets
- sending packets
- saving and loading configuration
- variable pin configuration

How To Use
==========

Connect your FTDI Module/Adaptor with nRF24l01 or nRF24l01+

- CE <-> DTR#
- SCK <-> RXD
- MOSI <-> RTS#
- MISO <-> DTR#
- CSN <-> TXD
- GND <-> GND
- VCC <-> VCC
- IRQ - not connected

Make sure that your FTDI Module/Adaptor use 3.3V VCC.

![Alt text](http://arduino-info.wikispaces.com/file/view/24L01Pinout-800.jpg/243330999/24L01Pinout-800.jpg "Connection Ref")


Here is example possible connection.

![Alt text](/docs/2014-12-07 13.06.16.jpg?raw=true "Connection Ref")

Screenshot
==========

![alt tag](/docs/Screenshot 2014-12-07 11.51.35.png)


Download
========
Executables are inside r direcotory.

- latest version https://github.com/g-gabber/FTDI_nRF24L01/archive/master.zip

