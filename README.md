# Operating Systems Multilevel Queue Scheduling

This project implements a Multilevel Queue Scheduling algorithm for process management in operating systems. It uses different scheduling algorithms for different priority levels of processes.

## Queue Structure

The system uses four queues with different scheduling algorithms:

- `q0`: Round Robin (RR) Algorithm for high priority processes (Time Quantum = 20)
- `q1`: Shortest Job First (SJF) Algorithm for mid-high priority processes
- `q2`: Shortest Job First (SJF) Algorithm for mid-low priority processes
- `q3`: First In First Out (FIFO) Algorithm for low priority processes

## Features

- Dynamic process creation with user-defined priorities
- Random generation of burst times for processes
- Calculation of waiting time and turnaround time for each process
- Calculation of average waiting time and average turnaround time for each queue
- CPU switches between queues every 20 seconds (simulated)
- Final output of all processes sorted by turnaround time

## How to Use

1. Compile the C++ file:
   ```
   g++ -o scheduler 22001281_Assignment_08.cpp
   ```

2. Run the executable:
   ```
   ./scheduler
   ```

3. Follow the prompts to enter the number of processes and their priorities.

4. The program will simulate the scheduling and display the results.

## Implementation Details

- The project uses C++ and includes the following libraries:
  - `iostream`: for input and output operations
  - `unistd.h`: for the sleep function (used to simulate time passing)
  - `queue`: for implementing the FIFO queue
  - `vector`: for implementing the other queues and for easier sorting
  - `algorithm`: for sorting processes

- The `Process` class is used to represent individual processes with attributes such as PID, priority, burst time, arrival time, waiting time, turnaround time, and remaining time.

- The main scheduling algorithms (Round Robin, Shortest Job First, and First In First Out) are implemented in separate functions.

## Output

The program provides the following output:
- Average Turnaround Time and Average Waiting Time for each queue
- A final table showing details of all processes, sorted by turnaround time

## Note

This is a simulation and the time units are arbitrary. In a real system, these would correspond to actual CPU cycles or time slices.

## Contributing

Feel free to fork this project and submit pull requests with improvements or bug fixes.

## License

[Insert your chosen license here]