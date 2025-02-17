# Operating System Assignments  

This repository contains my Operating System assignments, first two assignments are written in C and the third assignment is in Python. Each assignment covers different fundamental OS concepts, including process handling, memory management, and networking.  

## Assignment 1: Introduction to C and Signal Handling  
This assignment introduced me to the C programming language and involved two tasks:  

1. **Even Number Printer with Signal Handling**  
   - The program takes an integer `n` as input and prints the first `n` even numbers, pausing between each using `sleep()`.  
   - Signal Handling:  
     - On receiving a **HUP (SIGHUP)** signal, it prints `"Ouch!"` and continues execution.  
     - On receiving an **INT (SIGINT)** signal, it prints `"Yeah!"` and continues execution.  

2. **Modifying a Mini Shell**  
   - A basic shell program was modified to meet the assignment's requirements.  

## Assignment 2: Page Replacement Algorithms  
This assignment focused on implementing various **page replacement algorithms** to manage virtual memory efficiently:  

1. **Random Replacement**  
2. **First-In-First-Out (FIFO)**  
3. **Least Recently Used (LRU)**  
4. **Clock Algorithm**  

## Assignment 3: Multi-Threaded Socket Programming  
This assignment involved writing a **multi-threaded network application** using **socket programming**. Key features include:  

- Implementation of **multiple threads** for handling concurrent client connections.  
- Use of **mutex locks** to prevent race conditions and ensure thread safety.  
