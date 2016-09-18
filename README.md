# Arduino EEPROM Programmer
An Arduino-Mega based eeprom programmer. Binary data is loaded onto the Mega's onboard rom, and is then written to the rom chip through its I/O pins.  
# Usage
The desired binary data is stored in the byte array, program_dat in the source file in hex representation. Then pin definitions `oe, cs, we`, and `dbit0-dbit7` and `abit0, abit1, abit2, ...`, that represent the eeprom chip's out enable, chip select, write enable, and data/address lines, respectively, are changed to accomodate your needs. IMPORTANT: The eeprom control signals `oe, cs, we` need a pullup resistor!
