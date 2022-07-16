# QtDPS3005Rest
Control a DPS3005 power supply USB/RS232 Modbus over a GUI(qt) and a REST interface 

The source based on QtSerialBus Modbus example.
Also QSimpleRestServer is used for REST interface.

It was designed to work with DSP3005 hardware with a USB/RS232 interface.

the projekt desired OS is Windows and Raspberry PI3 and 4 
to control the power supply via a REST interface without any security.

this can be done by CURL or a browser with get request.

At the startup the PS is aktivated and Current Limit is set to 1A.

REST API
========

- http://127.0.0.1:5053/
- set Voltage http://127.0.0.1:5053/v1/set?V=5.0
  the rest call waits till voltage is read back.
- set Current http://127.0.0.1:5053/v1/set?I=1.0


Needed stuff
============
you need:

- QT designer + MSVC ( at my side 5.15.2 for windows ) or qmake for linux.
- DSP3005-USB hardware
- USB RS232 Board
- free USB port -:) 


Releases
========
you find a windows release (i386) version in the release on githup you may need MSVC runtime to run.
