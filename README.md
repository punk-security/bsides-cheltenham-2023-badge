# bsides-cheltenham-2023-badge
The Delorean badge for BSIDES Cheltenham 2023!

We raised over £300 for Mind charity selling parts to complete these on the day!!!!

## Bill Of Materials

You can see a parts list on DigiKey [here](https://www.digikey.co.uk/en/mylists/list/DBJ9N5IB9O)

Essentially its:

* ATTINY402 microcontroller - any spec 402 will do
* Button - any surface mount button fitting the footprint will work
* CR2032 battery clip - surface mount not through hole
* Neopixel addressable LED - Definitely get the right version of these
* Any 3528 Blue LED will work for the bumper rail

## Eagle designs and ordering your own badge

We've uploaded the AutoCAD EAGLE schematic, board design and custom footprints so you can make your own variants!  You will probably have to do some tweaks to get it working as they're designed to be standalone exports.

We've also uploaded the gerber_files.zip.  Take this to JLCPCB and you can order your own exact copies of the board :)

## Writing and flashing code!

We use [MegaTinyCore](https://github.com/SpenceKonde/megaTinyCore) for the arduino interface for ATTINY4x2 chips.

Dev and flashing is done within the Arduino IDE ( < v2 ) using a jtag2updi interface, as discussed [here](https://github.com/SpenceKonde/AVR-Guidance/blob/master/UPDI/jtag2updi.md)

We use [these little usb sticks](https://amzn.eu/d/c0lx0wG), with a 4.7k resistor soldered between the Tx and Rx lines.  

### Installing MegaTinyCore dependencies

This board package can be installed via the board manager in arduino. The boards manager URL is:

`http://drazzy.com/package_drazzy.com_index.json`

1. File -> Preferences, enter the above URL in "Additional Boards Manager URLs"
2. Tools -> Boards -> Boards Manager...
3. Wait while the list loads (takes longer than one would expect, and refreshes several times).
4. Select "megaTinyCore by Spence Konde" and click "Install". For best results, choose the most recent version.

### Setting up the IDE

1. Open the file **bsides-cheltenham-2023-badge.ino** in arduino
2. Select tools > Board > megaTinyCore > ATtiny412/402/212/202
3. Select tools > Chip > ATtiny402
4. Select tools > Clock > 8Mhz internal *(we get power issues otherwise)*
5. Select tools > Programmer > SerialUPDI SLOW
6. Select tools > Port > ( pick your COM port)

### Flashing firmware
1. Connect the 3v3 pin to 3v on your usb (maybe wedge it in the battery clip)
2. Connect GND to GND
3. Connect UPDI to the RxD port
4. Click Upload button
