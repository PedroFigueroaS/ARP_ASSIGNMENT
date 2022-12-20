# ARP-Hoist-Assignment-User Guide to Compile and run the project

In particular, the repository is organized as follows:
- The `src` folder contains the source code for the Command, Inspection, Master, Motor X, Motor Z and the Watchdog
- The `include` folder contains all the data structures and methods used within the ncurses framework to build the two GUIs. 
- The `bin` folder is where the executable files of the codes in the src files are stored

## ncurses installation
To install the ncurses library, simply open a terminal and type the following command:
```console
sudo apt-get install libncurses-dev
```

## Compiling the project

In the file where the src, include bin files are stored, there is a shell executable called build.sh. This executable is run as it follows:

```console
./build.sh
```
the content of this executable are the following code lines:
```console
#! /usr/bin/bash
gcc ./src/inspection_console.c -lncurses -lm -o ./bin/inspection
gcc ./src/command_console.c -lncurses -o ./bin/command
gcc ./src/motor_x.c -o ./bin/motor_x
gcc ./src/motor_z.c -o ./bin/motor_z
gcc ./src/world.c -o ./bin/world
gcc ./src/watchdog.c -o ./bin/watchdog
gcc ./src/master.c -o ./bin/master
```
This shell code allows to compile all the process involved in the simulation, and also being a more efficient way to make modifications.

## Running the project

In the same address of the shell executable to compile the project, there is the other executable that allows to run the project called run_program.sh. In a terminal, type the next command

```console
./build.sh
```
the content of this executable are the following code lines:

```console
konsole  -e ./bin/master
```
This shell code allows to run the master code of the project, that in its own run and spawn the other process involved in the project.

## Troubleshooting after launching the run executable

It is possible that after launching the run_program.sh, the motor x or motor z process crashes and compromises the simulation. In any case, you should try to terminate the other process and launch again the shell executable.




