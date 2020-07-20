# SportBuzzer
Wireless time measurement with two buzzers, which also work as standalone.

## Example Schematics
![Imgur](https://i.imgur.com/vRQOkD1.png)

The pinout can be changed as long as it follows these rules:
* The buzzer needs to be connected to a pin which supports interrupts. 
  More information about interrupts can be found in the 
  [official Arduino reference](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/).
* The HC12 module's TX and RX pins need to be connected to pins which are support by the 
  [Arduino SoftwareSerial library](https://www.arduino.cc/en/Reference/SoftwareSerial).
* The HC12 module's SET pin needs to be connected to a pin which can be used as digital output. 
  It's also required to pull this pin to GND.
* The buttons for the interface need to be connected to pins which can be used as input.
  These need to be pulled to VCC, which may require an external pull-up resistor if no internal pull-up resistors exist on the chip.

And as always it is recommended to use a capacitor to debounce the buttons, and also use a resistor in front of the capacitors, to
limit possible voltage spikes caused be the capacitor.

## Usage
Detailed information about the usage and limits of the software can be found in the wiki.

## Code
Documentation of the code can also be found in the wiki.

## Info about actually displaying the GUI on a display
As the time I am writing this, it is unclear when and how I will get access to display the GUI on an actual display.
Currently, the GUI is just printed over the serial interface (aka Serial Monitor).
This may change when I find the time and motivation to write an PWA for it.
