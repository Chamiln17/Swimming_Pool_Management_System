# Swimming Pool Management System
This project was made in collaboration with [Makhloufi Aymen](https://github.com/AymenMakhloufi) as part of the Operating System subject in university.
Swimming Pool Management System is a project that simulates the management of a swimming pool with multiple swimmers (nageurs) using semaphores and processes in C language.

# Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Files and Structure](#files-and-structure)

# Introduction 

The Swimming Pool Management System aims to manage the allocation of cabins and baskets to swimmers, simulating their behavior in a shared environment.

# Prerequisites

To run this project, you need to have the following installed:

- C compiler
- Unix-like environment (Linux or Mac.os)

# Installation

1. Clone the repository:
   `git clone https://github.com/Chamiln17/swimming-pool-management.git`

2. Navigate to the project directory:
   `cd swimming-pool-management`
3. Compile the main program:
   `gcc main.c -o main`
4. Compile the swimmer program:
   `gcc Pgme_nageur.c -o nageur`

# Usage
Run the following on the CLI:
`./main`

# Files and Structure
- `main.c`: The main program responsible for managing the swimming pool environment.
- `Pgme_nageur.c`: The swimmer program that simulates the behavior of individual swimmers.
