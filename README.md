# AWG-RedPitaya
An arbitrary waveform generator is implemented in the FPGA of Red Pitaya. A text files with the set of points defining the waveform can be loaded into the programming logic (PL) directly via the processing subsystem (PS) of the FPGA. The DAC can output the waveform at a maximum rate of 125 MSps.

# Usage
The project is aimed at the Red Pitaya STEM 14 board running the Zynq 7010 FPGA. 
The folder "tcl_files" contains a .tcl file that can be read from Vivado to recreate the project with the block diagram. Alternatively, bit files that have been precompiled can be directly loaded onto the FPGA. There aree ttow bit files, outputing at 5 MSPs and 125 MSps rates.

