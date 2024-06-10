# AWG-RedPitaya
An arbitrary waveform generator is implemented in the FPGA of Red Pitaya. A text files with the set of points defining the waveform can be loaded into the programming logic (PL) directly via the processing subsystem (PS) of the FPGA. The DAC can output the waveform at a maximum rate of 125 MSps.

# Usage
The project is aimed at the Red Pitaya STEMlab 125-14 board running the Zynq 7010 FPGA. 
The AWG.tcl file can be read from Vivado to recreate the project with the block diagram. The srcs and constrs folder need to be copied at the same path as the tcl file.

Alternatively, bit files located in the bits folder that have been pre-compiled can be directly loaded onto the FPGA. The two bit files create arbitrary waveform generators that output at 5 and 125 MHz MSps.

The C file awg_load.c can be compiled and run from the PS part of the FPGA to load any waveform to be generated from the AWG. The example folder contains a Python file to generate the arbiraty waveform file to be used with the awg_load.c file. The arbitrary waveform file is 2^16 in length. The compiled files takes the number of points and the name of the file as the command line parameter.

<div>
  <button class="copy-button" onclick="copyToClipboard(this.parentElement.nextElementSibling.textContent)"></button>
  <pre><code> gcc awg_load.c -o awg_load <br>./awg_load 65536 awg_double_lor.csv
  </code></pre>
</div>

` root@rp# gcc awg_load.c -o awg_load `

`root@rp# ./awg_load 65536 awg_double_lor.csv`

The example folder also contains image of an arbitrary waveform generated with a 5MSps and 125MSps FPGA images.




