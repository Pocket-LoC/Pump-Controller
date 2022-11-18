# Pocket-LoC
<img align = "right" src="https://user-images.githubusercontent.com/42568983/202521498-0bb95a05-1dd4-4db9-ad12-fc51b9aba1ed.jpg" width="40%" /> 
Lab-on-chip (LoC) technology is becoming increasingly relevant, especially in the field of medicine. However, often LoC setups rely on complex lab equipment for operation. The aim of this project is to create an affordable and portable LoC setup as a proof-of-concept for truly pocket-sized LoC - the Pocket LoC.

Pocket LoC can be assembled with standard equipment found in a typcial engineering lab (such as a maker space or FabLab). Once assembled, Pocket LoC is fully portable and only needs a PC to operate.

## Pump Controller

The pump controller for the Pocket LoC is designed as an interface for the [Bartels mp-Highdriver4](https://www.bartels-mikrotechnik.de/en/microelectronics/) and uses a microcontroller (Atmel ATmega32U4). The sensor can be operated like an Arduino with connected peripherals, making coding simple.

The mp-Highdriver4 and four micropumps (mp6-liq) can be plugged directly into the controller to achieve a compact form factor.

### PCB assembly

All design files for the PCB can be found as an Altium project in the [hardware folder](https://github.com/Pocket-LoC/Pump-Controller/tree/main/Hardware). You can order the ready printed PCB from manufacturers like [JLCPCB](https://jlcpcb.com/) or [multi-cb](https://www.multi-circuit-boards.eu/en/index.html) and assemble it yourself (requires tools and skills for SMD assembly), order an assembled PCB or contact me.

Once assembled, the microcontroller will have to be initially flashed with a bootloader (requires an ISP device, e.g. an Arduino) and then can be programmed and used via a regular USB connection. Details on flashing the bootloader and firmware usage can be found [here](https://github.com/Pocket-LoC/Pump-Controller/tree/main/Firmware).

### Device assembly

When assembling the pump driver section, ensure that the marked "PIN1" on the driver section (Bartels mp-Highdriver4) is aligned with the USB connector of the controller. Then insert the micropumps (Bartels mp6-liq), ensuring that the arrows are facing away from the USB connector, and close the connectors by pushing them down. Tweezers may help if you have to open the connectors again to remove the pumps.

<p float="left">
        <img src="https://github.com/Pocket-LoC/Housing/blob/main/Photos/50%20driver%20components.jpg" width="22%" />
        <img src="https://github.com/Pocket-LoC/Housing/blob/main/Photos/51%20controller%20assembly.jpg" width="22%" /> 
        <img src="https://github.com/Pocket-LoC/Housing/blob/main/Photos/52%20first%20pump%20connected.jpg" width="22%" />
        <img src="https://github.com/Pocket-LoC/Housing/blob/main/Photos/53%20driver%20assembled.jpg" width="22%" />
</p>
