# SportBuzzer
Wireless time measurement with two buzzers, which also work as standalone.

## Schematics
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
  These need to be pulled to VCC, which may require an external pull-up resistor if no internal 
  pull-up resistors exist on the chip.

And as always it is recommended to use a capacitor to debounce the buttons, and also use a resistor 
in front of the capacitors, to limit possible voltage spikes caused be the capacitor.

## Usage
The GUI is controlled by the three buttons, called previous, next, and confirm. Generally speaking, 
while navigating you use the previous button to go to previous element, the next button to go to the
next element, and press the confirm button, to leave or enter a menu.

To change a value, you first need to navigate to the corresponding element. Then you press the confirm 
button to start changing the value. Use the previous button to lower the value, and use the next button
to increase it. When done editing the value, press the confirm button again to confirm your input.

While being in either the stopwatch or timer mode, pressing the buzzer will start the time, and pressing
the buzzer again will stop it. You can also cancel out by pressing the confirm button on the cancel element. 
**After canceling no result is shown.**

When in the same wireless channel and same mode, pressing either of the buzzers will stop the time.

Detailed information about the usage and limits can be found in the wiki.

## Code
This project uses an [Arduino CMake toolchain](https://github.com/a9183756-gh/Arduino-CMake-Toolchain/),
but it is still possible to compile it within the [Arduino IDE](https://www.arduino.cc/en/Main/Software).
The only limitations are as follows:
* The `SportBuzzer.ino` file needs to be a file without any code in it to keep it compatible with both the
  Arduino IDE and the CMake toolchain. The only purpose of this file is to make it possible to open the
  project in the Arduino IDE.
* Due to the project size the project contains multiple source files, which are organized in directories.
  The starting point of those directories need to be `src/`, so the Arduino IDE picks it up. Still, the 
  Arduino IDE won't show the directories and the containing files in the Editor.

The basic concepts of the code are as followed:
* Abstract the hardware as much as need, so you are able to change it without needing to change stuff all over
  the code base.
* Having a main loop (`void loop(){}` to be more specific) which handles background tasks, like checking the
  battery level, and connection status. Also update the foreground task (called `GUITask`), providing 
  information about the hardware state it already got.
* The GUITask has it own state, which gets destroyed when exiting the task. It handles everything related 
  to its options, and time measurements. When done updating, it returns itself to keep alive, or a new
  GUITask to exit.

A full documentation of the code can be found in the wiki.

## Info about actually displaying the GUI on a display
As the time I am writing this, it is unclear when and how I will get access to display the GUI on an actual display. Currently, the GUI 
is just printed over the serial interface (aka Serial Monitor).
This may change when I find the time and motivation to write an PWA for it.
