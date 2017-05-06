# Split Flap Display
Train stations and airports used to be riddled with these before LCD displays and monitors became commonplace. They used to show the departing and arrival scheduled as well as any delays but they fell out of favour due to the ease and creativity afforded with newer display technologies. What you lose along with that is the riffle sound that draws the eye when something changes, low power consumption, and unsurpassed visibility.

They are also referred to by genericised trademarks such as *Solari boards* after Solari di Udine, or *Pragoton* after the Czech manufacturer. New ones are no longer made and old ones under heavy disuse seldom show up on eBay and cost in the thousands per character, for a likely non-working one.

This project acts as a modern, modular, easy to interface alternative to finding vintage terminal displays. The goal is to have medium run (~100) production costs to near 10-15 USD/module.

## Module
The frame is laser cut acrylic, thickness is kept low and with a few modifications it can be made to injection mould and snap together. Currently, solvent welded. Electronics are mounded with screws, and communication occurs through pogo pins. Magnets connect modules to each other, and their polarity and positioning make sure it can only be plugged in properly. A magnet is also on the great wheel and is used for homing.

![][singleModulePresp]

The male pins can be seen on this side along with the gears, motor and main hub. Main shaft is mounted in inexpensive skateboard bearings and gears are those popularised by 3D printers, therefore prices are always falling. GT2 belts, collar clamps and 8mm hardened shaft also fall under that category.

![][singleModuleGearPresp]

The "flaps" are based on die cut notches in plastic card blanks. Many vendors will print and die cut large quantities of cards, making scaling simple and labour intensive. Font and spacing can be changed, but as the goal is visibility, a san-serif monospaced font should be used. 

## Electronics
PCB and layout files found under electronics. Runs on a ATtiny13A, hall sensor for angular feedback, and a Darlington array for controlling the stepper. Under 5 USD in parts for medium volumes including the motor and pogo pins.

The two boards, in/out are connected with a ribbon able and IDC connectors (think PATA hard drives), inexpensive, easy to crimp... and even available pre-crimped in 4/6" lengths.

![][singleModuleNoSpool]

TODO: Schematic/layout images, BOM

### Firmware
AVR firmware along with makefile under firmware directory. On start it rotates to home, and then acts as a shift register forwarding the last character given

TODO: Insert communication clock diagram
TODO: Walk through code, decently commented, but avr ports are confusing

[singleModulePresp]: renderings/singleModulePresp.png "Assembled view"
[singleModuleGearPresp]: renderings/singleModuleGearPresp.png "Gear side view"
[singleModuleNoSpool]: renderings/singleModuleNoSpool.png "Detail of electronics with spool removed"
