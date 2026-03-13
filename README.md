# Adaptive CPU Scheduling Simulator

## Overview

This project implements several CPU scheduling algorithms used in Operating Systems.
The simulator allows users to input process details and analyze how different scheduling algorithms affect CPU performance.

## Algorithms Implemented

* First Come First Serve (FCFS)
* Shortest Job First (SJF)
* Shortest Remaining Time First (SRTF)
* Priority Scheduling
* Round Robin
* Multilevel Queue Scheduling
* Adaptive Scheduling Algorithm

## Features

* Generates **Gantt Chart output**
* Calculates **Waiting Time (WT)** and **Turnaround Time (TAT)**
* **Algorithm comparison mode**
* **Manual input or file input support**
* Implemented using **C++ and STL**

## Project Structure

CPU-Scheduling-Simulator
│
├── scheduler.cpp
├── input.txt
└── README.md

## Sample Input

Example process input format:

Number_of_processes
Arrival_Time Burst_Time Priority

Example:

5
0 5 2
1 3 1
2 8 3
3 6 2
4 2 1

## How to Run

Compile the program:

g++ scheduler.cpp -o scheduler

Run the program:

.\scheduler.exe

## Output

The program displays:

* Gantt chart
* Process table
* Average waiting time
* Average turnaround time

## Technologies Used

* C++
* Standard Template Library (STL)
* Git
* GitHub

## Author

Yashwanth
