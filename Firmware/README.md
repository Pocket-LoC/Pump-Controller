## Pocket LoC Pump-Controller

### Firmware
This is the firmware for the Pocket LoC pump controller. As the controller uses an ATmega32U4 microcontroller, we can use it like an Arduino Micro. To use this firmware install the [Arduino IDE](https://www.arduino.cc/en/software).



#### Flashing the bootloader
Before we can use the freshly assembled sensor via the USB connection, we have to flash it with a bootloader. This can be easily done from the Arduino IDE using either a different [Arduino](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP#the-bootloader) or a dedicated [ISP](https://docs.arduino.cc/hacking/software/Bootloader#burning-the-bootloader) device. Remember to select "Arduino Micro" as a target. See below for the ISP header pinout. All other necessary information can be found in the linked manuals.

<img src="https://github.com/Pocket-LoC/Pump-Controller/blob/main/Firmware/ISP%20Pinout.png" width="40%"/>

Once flashed, remove the ISP header pins from the sensor. We will (hopefully) never need them again and they obstruct the sensor placement in the Pocket LoC.

#### Using the Firmware
Clone this repository and open the .ino file in the Arduino IDE. Connect the prepared sensor with a USB cable (micro USB-B), select the correct port in the IDE (the sensor will appear as "Arduino Micro"), and upload the sketch. That will take a few moments and the LEDs on the sensor will flash.

Finally, you are ready to go! You can now set and use the sensor as a serial device. Here is a list of commands you can send (each command must be terminated with "\r\n"):

- START

  Start streaming data read from the spectral sensors over the serial connection. The data will have the format 
  
  "\<A0>\t\<B0>\t\<C0>\t\<D0>\t\<E0>\t\<F0>\t\<X0>\t\<A1>\t\<B1>\t\<C1>\t\<D1>\t\<E1>\t\<F1>\t\<X1>"
  
  where \<A0> through \<F0> are the sensor values read for the activated photodiodes (rational number with 10 digit decimal precision) of the first sensor and \<X0> is a flag to indicate if at least one of the photodiodes is saturated (i.e. the maximal value was reached). \<A1> through \<X1> are the same parameters for the second sensor.
 
- STOP
 
  Stop outputting data to the serial connection.
  
- T\<n> (e.g. T5)
  
  Set the sample time in ms used for each sensor value acquisition. As two sensor values have to be read and some margin for timing errors is added in the firmware, the actual time between values reported on the serial connection is 4*n. As the sensitivity depends on the sample duration, a longer duration (i.e. lower sample rate) will also increase accuracy. Typically, the gain (see below) will have to be adjusted to fit the sample duration and environment conditions.
  
- G\<n> (e.g. G3)
  
  Set the input gain applied to the photodiodes. The gain should ideally be set to the highest value that never causes a saturation error (see above: Flag reported with output data). "n" can be any integer between 0 and 10 and corresponds to the selectable gains: 0.5x, 1x, 2x, 4x, ...
  
- C<0123456789> (e.g. C1010101011)
  
  Configure which photodiodes should be used. The [AS7341](https://ams.com/en/as7341) contains a total of 10 different photodiodes that may be used in the sensor, but only 6 ADCs. We therefore have to select which 6 photodiodes we want to use (i.e. which wavelengths we want to observe). A photodiode is selected by setting a "1" in the command and deselected with a "0". The positions correspond to the following list (see the Fig. below, taken from the spectral sensors manual, for explanation): F1, F2, F3, F4, F5, F6, F7, F8, CLEAR, NIR. The first six activated photodiodes will be connected to ADCs and the output will be in the same order as the list. Do not worry - the sensor will confirm the actually set photodiodes on the serial connection.
  
  ![Spectral Distriution AS7341](https://user-images.githubusercontent.com/42568983/200896588-a6347954-e436-44e7-8c64-45ed0d1019f8.PNG)

#### Getting Started
You can use the "serial monitor" in the Arduino IDE to quickly test the sensor with some first commands. In the [software folder](https://github.com/Pocket-LoC/Sensor/tree/main/Software) you will find a Python example to configure the sensor, perform an automatic gain adjustment, and show a live plot of the recorded data.

#### General Information

- The sensor has a blue LED that indicates it is powered.
- Once powered up, the sensor will wait for a serial connection before initialisation. This is also signalised by a red flashing LED.
- Two yellow LEDs show the Rx and Tx activity - useful for debugging during sketch upload.
- A green LED is active when data is beeing streamed to the serial connection.
- The red LED will light up during streaming when a saturation error occurs on any photodiode.


