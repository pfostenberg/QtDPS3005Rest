# QtDPS3005Rest
Control a DPS3005 power supply USB/RS232 Modbus over a GUI(qt) and a REST interface 

The source based on QtSerialBus Modbus example.
Also QSimpleRestServer is used for REST interface.

It was designed to work with DSP3005 hardware with a USB/RS232 interface.

the projekt desired OS is Windows and Raspberry PI3 and 4 
to control the power supply via a REST interface without any security.


REST API
========

Read Voltage
Set Voltage ( waits for voltage is reached!) 