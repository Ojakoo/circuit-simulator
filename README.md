<!---
# Sample README file for C++ programming project

This is an initial git repository for your programming project.
It contains an initial directory structure that we wish you could
use as follows:

  * `plan/` -- Your project plan should be put here. You may use different sources,
    but we hope to get the plan **also as a pdf file**.

  * `doc/` -- The documentation sources related to your project should be put here.
    You may use different tools for documentation (e.g. latex),
    but we hope to get the final version of the document
    **also as pdf file**.

  * `src/` -- Here are the C++ source files. You can have subfolders as needed.

  * `README.md` -- This file. You should modify it to represent
    your project.

You may also add other new directories as needed, for example
for testing tools.
!--->

# Circuit Simulator (C++ programming project)

![preview](images/preview.png)

### Course: ELEC-A7151 - Object oriented programming with C++
### Group : circuit-sim-2020-2

## Project Description

Circuit simulator program that can calculate steady state analysis for DC and AC. You can add, rotate, move, connect and remove components and wires. You can also move and zoom the view and build an electronic circuit from various components and sources by connecting them with wires. After you have constructed a circuit you can solve the circuits node voltages and component currents. The program uses MNA (Modified Nodal Analysis) when solving the voltages and currents in the circuit. When calculating the currents and voltages with steady state DC analysis, inductors will appear as short circuits and capacitors will appear as open circuits for the solver.

Instructions on how to build and run the program can be found in /doc folder.