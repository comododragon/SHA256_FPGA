# Digital signature system for embedded platforms (SHA-256 version)

## Authors

* André Bannwart Perina (abperina at usp dot br)
* Luciano Falqueto
* Wallison de Oliveira

### Contributors

* Joachim Strömbergson (SHA-256 Verilog module)

## Description

This project is composed by a host platform (e.g. Intel Galileo Gen2 or Raspberry Pi) and an FPGA platform (e.g. Arrow BeMicro MAX 10). Its purpose is to use the FPGA as an external device for computing SHA-256 hashes, in order to create a digital signature (AES-256 used as a cipher on the host platform).

## Notes

**This project was developed as a coursework for a MSc module. By no means it is intended to be used as is, since current communication bus is a huge bottleneck and many security measures are not taken into account (dynamic IV, use of PKI, etc.).**

## Project structure

The project is structured as follows:

* **DelayedSPI:** Contains Verilog module for SPI communication
	* **Verilog**
		* **SPISlaveDelayedResponse.v:** SPI Slave Verilog Module. It reads `WIDTH` bits, wait for `DELAY` cycles and sends `WIDTH` bits
* **Full:** Full project with Quartus II project and C source code
	* **C:** C projects
		* **Galileo:** Projects for Intel Galileo Gen2 Platform
			* **NoFPGA:** SHA-256 and AES-256 done in software
				* **bin:** Binaries folder
					* **main:** Main binary. It generates a file `data.out` on current working directory with tuples of three lines. The first line has the raw input data (32-bytes automatically acquired), second line the hash for this data and third line the ciphered hash, using key and IV set in the source code
					* **compare:** Opens the `data.out` file (has to be in current working directory), deciphers the signature and prints the results
				* **include:** Includes folder
					* **common.h:** Common functions for assertions
					* **crypt.h:** Small cryptography library, contains some hash and (de)cipher functions
				* **obj:** Objects folder
					* **crypt.o:** Object file for criptography library
				* **src:** Sources
					* **compare.c:** Source code for comparison binary
					* **crypt.c:** Source code for cryptography library
					* **main.c:** Source code for main binary
				* **Makefile:** Makefile for this project. Call `make bin/main` to make the main binary or `make bin/compare` to make the comparison binary
			* **WithFPGA:** SHA-256 done in FPGA, AES-256 done in software
				* Same as `NoFPGA` structure
		* **Pi:** Projects for Raspberry Pi (tested on Raspberry Pi 3 Model B)
			* Same as `Galileo` structure
	* **Quartus:** Quartus II project
		* **output_files**
			* **SHA256.sof:** Pre-compiled bitstream for FPGA
		* **SHA256.out.sdc:** Synopsys Design Constraints, defining system clocks
		* **SHA256.qpf:** Quartus II Project file
		* **golden_top.tcl:** Beautiful script with all pins set for MAX 10 FPGA
		* **TOP.v:** Top-level module
	* **Verilog:** Verilog source codes
		* **ActivityLED.v:** Activity Indicator module
		* **Manager.v:** SHA-256 and communications manager module
		* **sha_256_\*.v:** SHA-256 related modules
* **report.pdf:** Report about the project (in portuguese)

## Connection scheme

### Intel Galileo Gen2

```
Location of connectors in schematics:
 -----------------------------------
| DEVICE         | CONNECTOR | PAGE |
|----------------|-----------|------|
| GALILEO GEN2   | J2A1      |   24 |
| GALILEO GEN2   | J2B1      |   24 |
| BEMICRO MAX 10 | J4        |    4 |
 -----------------------------------

Connections (pins names as presented in schematics):
 ---------------------------------------------------------------------------------
| GALILEO GEN2                     |             | BEMICRO MAX 10                 |
| CONNECTOR | PIN NAME   | PIN NO. | DESCRIPTION | PIN NO. | PIN NAME | CONNECTOR |
|-----------|------------|---------|-------------|---------|----------|-----------|
| J2A1      | GND        |       7 | GROUND      |       7 | GND      |        J4 |
| J2B1      | DIGITAL 11 |       4 | SPI MOSI    |       3 | I2C_SDA  |        J4 |
| J2B1      | DIGITAL 12 |       5 | SPI MISO    |       5 | GPIO_A   |        J4 |
| J2B1      | DIGITAL 13 |       6 | SPI_SCLK    |       4 | I2C_SCL  |        J4 |
 ---------------------------------------------------------------------------------
```

### Raspberry Pi 3 Model B

```
Location of connectors in schematics:
 -----------------------------------
| DEVICE         | CONNECTOR | PAGE |
|----------------|-----------|------|
| RASPBERRY PI   | J8        |    1 |
| BEMICRO MAX 10 | J4        |    4 |
 -----------------------------------

Connections (pins names as presented in schematics):
 ---------------------------------------------------------------------------------
| RASPBERRY PI                     |             | BEMICRO MAX 10                 |
| CONNECTOR | PIN NAME   | PIN NO. | DESCRIPTION | PIN NO. | PIN NAME | CONNECTOR |
|-----------|------------|---------|-------------|---------|----------|-----------|
| J8        | GPIO10     |      19 | SPI MOSI    |       3 | I2C_SDA  |        J4 |
| J8        | GPIO9      |      21 | SPI MISO    |       5 | GPIO_A   |        J4 |
| J8        | GPIO11     |      23 | SPI_SCLK    |       4 | I2C_SCL  |        J4 |
| J8        | GND        |      25 | GROUND      |       7 | GND      |        J4 |
 ---------------------------------------------------------------------------------
```

## How to use

1. Compile Quartus II project (you can skip this step and use provided .sof file)
2. Connect boards according to presented connection schemes
3. Power up both boards
4. Download desired project to the host platform (e.g. using `scp`)
5. Build programs
	1. `make bin/main`
	2. `make bin/compare`
6. Run the main binary: `./bin/main`
	* When using `WithFPGA` project, run as root
7. Program FPGA using provided .sof file and press enter in the host platform
	* You can skip this step if using `NoFPGA` project
8. `data.out` will have tuples of three lines, consisting of:
	1. Raw data
	2. Hashed data
	3. Ciphered data
9. Use `bin/compare` to compare deciphered values with provided values of `data.out`

## Useful Links

* **BeMicro MAX 10 Schematic:** http://www.alterawiki.com/uploads/e/ec/BeMicro_Max_10-Schematic_A4-20141008.pdf
* **Intel Galileo Gen2 Schematic:** http://www.intel.com/content/www/us/en/embedded/products/galileo/galileo-g2-schematic.html
* **Intel Galileo Gen2 Pins Description:** https://www.safaribooksonline.com/library/view/intel-galileo-and/9781430268383/images/TableB-1.jpg
* **Raspberry Pi 3 Model B Schematic:** https://www.raspberrypi.org/documentation/hardware/raspberrypi/schematics/RPI-3B-V1_2-SCHEMATIC-REDUCED.pdf
* **SHA-256 Verilog module:** https://github.com/secworks/sha256/
