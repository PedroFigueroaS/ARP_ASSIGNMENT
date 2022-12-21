# ARP-Hoist-Assignment-Summary

The Assignment consists in the design, test and deploy of a interactive simmulation of a hoist that moves in two axis (X & Z axis), with two manual consoles that allows the user to control the velocity in each axis of the hoist, and to stop or return to its initial position.

The simmulation involves 7 codes that are the processes required to control the project. This process are as follows:


## The master process
The main process of the simulation, that calls the other processes and give each one of them a PID.

In the master code, in order to call other process, it is used a spawn function that receives a string that contains the address of the desired executable. Into the spawn function, it is used the fork commmand to create a child process for the executable, and run it properly with the execvp command.

## The command console process
The command console is the first part of the simulation, that allows the user to send velocity data to the motor process.

The command console will send a velocity value to each motor process always, independently if one of the buttons is pushed/clicked, in order to avoid the blocking issue of the named pipes while reading the data sent. There are two float variables (velx,vely), that are the velocity data sent through pipes to the motor processess.

Because the code initializes with velx=0, and vely=0, the velocity sent to the motor process will be zero, thus the position will be also zero and will only increase or decrease if one of the buttons is pushed. 

Depending of the axis, each time a V+ or V- button is pressed, the velocity variable will increase or decrease 0.25, until they reach a max/min velocity of +/-2. If the stop button of each axis is pushed, the velocity value will become zero, stopping the movement of the hoist.

The command console also have three signals interruptions:                                                                                                          Exit handler: This interruption causes the programm to end its execution, due to the task of the watchdog                                                               Stop: This interruption causes to convert any value of velocity into 0, due to the stop/reset button of the inspection console                                k_process: This interruption is activated each time a button is pushed, and to send a signal to the watchdog

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
