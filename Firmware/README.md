## Pocket LoC Pump-Controller

### Firmware
This is the firmware for the Pocket LoC pump controller. As the controller uses an ATmega32U4 microcontroller, we can use it like an Arduino Micro. To use this firmware install the [Arduino IDE](https://www.arduino.cc/en/software).

#### Flashing the bootloader
Before we can use the controller via the USB connection, we have to flash it with a bootloader. This can be easily done from the Arduino IDE using either a different [Arduino](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP#the-bootloader) or a dedicated [ISP](https://docs.arduino.cc/hacking/software/Bootloader#burning-the-bootloader) device. Remember to select "Arduino Micro" as a target. The controller has a compact [Tag-Connect](https://www.tag-connect.com/product/tc2030-idc-nl-10-6-pin-plug-of-nails-spring-pin-cable-with-0-1-ribbon-connector-10-version) ISP header - see below for the pinout. All other necessary information can be found in the linked manuals.

<img src="https://github.com/Pocket-LoC/Pump-Controller/blob/main/Firmware/ISP%20Pinout.png" width="40%"/>

#### Using the Firmware
Clone this repository and open the .ino file in the Arduino IDE. Connect the pump controller with a USB cable (micro USB-B), select the correct port in the IDE (the sensor will appear as "Arduino Micro"), and upload the sketch. That will take a few moments and the LEDs will flash.

Finally, you are ready to go! You can now use the controller as a serial device (do not forget to connect the Bartels Highdriver-4). Here is a list of commands you can send (each command must be terminated with "\r\n"):

- P\<x>V\<yyy>

  Set the voltage of pump <x> to <yyy>. The voltage is proportional to the pump output and can be in the range of 0V to 250V.
  
- P\<x>ON

  Activate pump \<x>. Voltage and frequency will be applied depending on the current settings.
 
- P\<x>OFF

  Deactivate pump \<x>. This will not reset the voltage setting.
  
- PA\<aaa>#\<bbb>#\<ccc>#\<ddd>

  Set the voltage of all four pumps (\<aaa> for pump 1, \<bbb> for pump 2, ...). Each value must be zero-padded to exactly three characters.
  
- PON

  Activate all pumps (i.e. apply voltage set previously).
  
- POFF

  Deactivate all pumps.
  
- SELECTQUADDRIVER

  Setup the controller to use the Bartels Highdriver-4. This should be called during initialisation.

#### Getting Started
You can use the "serial monitor" in the Arduino IDE to quickly test the controller with some first commands. Make sure you select carriage return and newline as command terminators. In the [software folder](https://github.com/Pocket-LoC/Pump-Controller/tree/main/Software) you will find a Python example to configure the controller and run a sample droplet program.

#### General Information

- The controller has a blue LED that indicates it is powered.
- A red LED is connected to PD7 (Arduino 27) and is active high.
- Both settable I2C-Adress pins on the Highdriver-4 are bound to high (Adress: 0x7b).
