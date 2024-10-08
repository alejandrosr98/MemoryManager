# MemoryManager

This application converts some files in an Intel Hex file for a memory. It is made for a data memory like an external EEPROM and so on. It does not support files to build an Hex file to create a program. It only support one _Record Type_ (see [Hex specification](https://onlinedocs.microchip.com/oxy/GUID-C3F66E96-7CDD-47A0-9AB7-9068BADB46C0-en-US-3/GUID-DF9E479D-6BA8-49E3-A2A5-997BBA49D34D.html)). It parse raw files and concatenates them leaving an unprogrammed byte between them.

## How to use

The application needs the size of the memory and the name of the file or files. Optionally, you can add the representation of an unprogrammed byte. When parsing the Hex file, the unprogrammed bytes will be ignored.

Mandatory:  
&emsp;-e &emsp; Params for EEPROM: \<size in kb\>.  
&emsp;-f &emsp; File names: \<file1\> \<file2\>... At least one.  
Options:  
&emsp;-b &emsp; Not flashed bytes representation in hex format. Only one byte long. Default 0xFF.  

Windows:

    memoryManager.exe -e 1 -f path-to-file1 path-to-file2 [-b value]

Linux:

    memoryManager -e 1 -f path-to-file1 path-to-file2 [-b value]