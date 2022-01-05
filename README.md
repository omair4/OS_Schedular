# OS_Schedular
Multilevel queue based Operating System Schedular, 
Simulating Process Scheduling  that will be implemented using
Multilevel Feedback Queues & Waiting Queues using Singly Linked List.

Ready Queues
FCFS (First Come First Serve)
It is a traditional scheduling technique in which all the processes have same priority, FCFS scheduled
the processes in the order in which they come, process which come first will take CPU time slice first
then next and so on, processes are inserted into the Rear of a queue when they are submitted.
SRR (Selfish Round Robin)
SJF (Shortest Job First)
The process which has short burst time will schedule first to run then next shortest job (process) gets
the CPU time. A scheduler arranges the processes with the least burst time in Front of the queue and
longest burst time in Rear of the queue. Process has to be executed to its entirety (except if I/O event
occurred).
Note: Burst time: Requires advanced knowledge or estimations about the time required for a process
to complete. So you should keep track of how many numbers of instructions left. Number of
instructions will tell us about burst time.
Multilevel Feedback Queues
In MLFQ (Multi-level feedback queue) scheduling, the queue (MLFQ) is divided into three queues,
where first queue has FCFS scheduling technique, second queue has Selfish Round Robin and the
remaining one has SJF scheduling technique.
1. FCFS scheduling technique with highest priority (FCFS-1).
2. SRR scheduling technique with medium priority (SRR).
3. SJF scheduling technique with lowest priority.




Scheduling
1. A new process is inserted at the Rear of highest priority queue (FCFS-1) according to the arrival
time.
2. At some stage the process reaches the Front of the queue and is assigned the CPU.
3. If the process uses the entire time slice, it is switched and inserted at the Rear of the next lower
level queue. This next lower level queue will have a time slice which is more than that of the
previous higher level queue.
4. This scheme will continue until the process completes or it reaches the base level queue (SJF).
5. SJF will sort (ascending order) the process whenever process enter in it on the basis of
instructions left. And if above higher priority queues are empty then it runs the process till it
completes.
6. If the I/O event occurred, process will be inserted at the Rear of waiting queue, and when the
process becomes ready again it is inserted in the highest priority queue. (see Waiting Queue)
7. For scheduling, the scheduler always starts picking up processes from the Front of the highest
level queue. If the highest level queue has become empty, then only will the scheduler take up
a process from the next lower level queue. The same policy is implemented for picking up in
the subsequent lower level queues. Meanwhile, if a process comes into any of the higher level
queues, it will switches to that higher level queue after completing its time slice.
Also, a new process is always inserted at the highest queue with the assumption that it would be a short
time consuming process. Long processes will automatically sink to lower level queues based on their
time consumption and interactivity level. In the multilevel feedback queue, a process is given just one
chance to complete at a given queue level before it is forced down to a lower level queue.

National University of Computer & Emerging Sciences (NUCES),
Islamabad Department of Electrical Engineering

Waiting Queue
There will be three waiting queues. All of them use FCFS scheduling technique having same priority.
Whenever CPU encounters I/O operations Hard Disk, Network & Peripheral devices (i.e. Monitor,
Keyboard and Printer) that specific process will move to their respective queue.
And wait for specified time. When I/O operation is completed, process will move back to highest
priority ready queue.



Waiting queues are independent to each other. But within the queue, processes will wait for the prior
process to complete its I/O operation.
Instructions of Process
The data between Start and End of the input file specify a set of instructions associated with each
process. There are three types of instructions, READ, WRITE and COMPUTE. These instructions
represent CPU computations and read/write data to the Hard Disk, Network and Peripheral devices
(Monitor, Keyboard and Printer). COMPUTE instruction always will be 1 tick. Different keywords
as below:
DISK: This keyword represents the hard disk.
SCREEN: represents the monitor. And it’s a Peripheral device.
PRINTER: represents the printer. And it’s a Peripheral device.
KEYBOARD: represents the keyboard. And it’s a Peripheral device.
NETWORK: represents the Network communication.
Example:
READ KEYBOARD 1 Tick
Read from keyboard while waiting time is 1 tick.
