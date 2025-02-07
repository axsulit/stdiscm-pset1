# STDISCM S12 Problem Set 1: Basic Threading
- SULIT, Anne Gabrielle M.

This project implements a prime number search in a specified range using multithreading. The user can configure the number of threads and the upper limit for the range in the provided configuration file, `config.txt`. The program supports different task division schemes and printing schemes for output customization.

## Configuration

- **x (number of threads)**: The maximum permissible number of threads depends on your hardware.
- **y (upper limit for range)**: The upper limit for searching prime numbers (from 2 to 2147483647). 

These values are configurable in the `config.txt` file.

## Task Division Scheme
- **Straight Division**: The search range is divided evenly among all threads (e.g., if the range is from 1 to 1000 and 4 threads are used, the range is divided into 1-250, 251-500, 501-750, and 751-1000).
- **Parallel Divisibility**: Each thread performs divisibility testing on individual numbers in parallel. This approach distributes the workload in a more granular manner.

## Printing Scheme
- **Immediate**: The results are printed immediately as prime numbers are discovered by each thread. The output includes the thread ID and timestamp for each prime number found.
- **Deferred**: The results are collected and stored during the computation, and then printed all at once after all threads have completed their execution. The prime numbers are printed in rows of 10 for better readability.

## Project Setup

### 1. Open the Project Solution File in Visual Studio.

- Double-click on `PSet1_Sulit.sln` to open the solution.

### 2. Click **Local Windows Debugger** to Run.

- Set the desired configuration options in the `config.txt` file before running the program.

## How to Use

1. **Edit Configuration File**: Modify the `config.txt` file to specify the number of threads (x) and the upper limit (y) for the prime number search.
2. **Select Task and Printing Scheme**: The program allows you to select between different task division schemes (Straight Division or Parallel Divisibility) and printing schemes (Immediate or Deferred).
3. **Run the Program**: Once the configuration file is updated, click **Local Windows Debugger** in Visual Studio to start the program.
