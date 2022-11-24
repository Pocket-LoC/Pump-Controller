## Pocket LoC Pump Controller

### Sample Software
This is a software sample to help you getting started with the controller firmware. The sample uses Python to establish a serial connection, configure the controller parameters and run a sample droplet program.

#### Getting Started
To run this code you will need to install Python (3.7+). Typically, it is easiest to use an IDE such as Spyder or Visual Studio. Depending on your setup you may have to install some of the used packages (see "import" statements at the top of the file or just run and wait for errors). Packages are usually installed using pip or conda, depending on your installation.

Connect the Pocket LoC Pump Controller (with the [firmware](https://github.com/Pocket-LoC/Pump-Controller/tree/main/Firmware) loaded and four micropumps connected). The blue LED should be on.

You can now run the Python code. The software will try to establish a connection to the first "Arduino Micro" that identifies as Pocket LoC pump controller. It will then configure the controller and wait for input confirmation to start the droplet sample. The sample assumes that pump 1 provides a constant background flow and pumps 2 through 4 are used to generate droplets. You will have to adjust the droplet timings and voltages for your specific application.

