/*
* Operating Systems Multilevel Queue Scheduling
*/

/*
q0 = Round Robin Algorithm for high priority processes with Time Quantum = 20
q1 = Shortest Job First Algorithm for mid priority processes 
q2 = Shortest Job First Algorithm for mid priority processes 
q3 = First In First Out Algorithm for low priority processes
*/

#include <iostream>     //for input and output
#include <unistd.h>     //for sleep function
#include <queue>        //for queue data structure and its functions
#include <vector>       //for vector data structure and its functions
#include <algorithm>    //for sort function

using namespace std;

//Global Variables
int processCount = 0;       //Global variable to set the total number of processes
const int timeQuantum = 20; //Time Quantum for Round Robin Algorithm
int clockPC = 0;              //Global variable to set the clock

class Process
{
protected:
	int pid;
	int priority;
	int burstTime;
	int arrivalTime;
	int waitingTime;
	int turnaroundTime;
	int remainingTime;

public:
    //Constructors and Destructors
    Process() : pid(0), priority(0), burstTime(0), arrivalTime(0), waitingTime(0), turnaroundTime(0), remainingTime(0) {}; //Default Constructor
    Process(int pid, int priority, int arrivalTime, int burstTime, int remainingTime, int wt, int tat) //Parameterized Constructor
    {
        this -> pid = pid;
        this -> priority = priority;
        this -> arrivalTime = arrivalTime;
        this -> burstTime = burstTime;
        this -> remainingTime = burstTime;
        this -> waitingTime = wt;
        this -> turnaroundTime = tat;
    }
    ~Process(){} //Destructor

    //Getters
    int getPid()            { return pid;               }
    int getPriority()       { return priority;          }
    int getArrivalTime()    { return arrivalTime;       }
    int getBurstTime()      { return burstTime;         }
    int getRemainingTime()  { return remainingTime;     }
    int getWaitingTime()    { return waitingTime;       }
    int getTurnaroundTime() { return turnaroundTime;    }

    //Setters
    void setPid(int pid)                        { this -> pid               = pid;              }
    void setPriority(int priority)              { this -> priority          = priority;         }
    void setArrivalTime(int arrivalTime)        { this -> arrivalTime       = arrivalTime;      }
    void setBurstTime(int burstTime)            { this -> burstTime         = burstTime;        }
    void setRemainingTime(int remainingTime)    { this -> remainingTime     = remainingTime;    }
    void setWaitingTime(int waitingTime)        { this -> waitingTime       = waitingTime;      }
    void setTurnaroundTime(int turnaroundTime)  { this -> turnaroundTime    = turnaroundTime;   }

};

//Vectors are used since the efficiency in sorting according to burst time
vector<Process> q0; //Queue for high priority processes (RR)
vector<Process> q1; //Queue for mid priority processes (SJF)
vector<Process> q2; //Queue for mid priority processes (SJF)
queue<Process> q3; //Queue for low priority processes (FIFO)
vector<Process> processesMerged; //vector to merge all the processes from the queues

void processTOqueque()
{
    //update global variable
    cout << "Enter the number of processes: ";
    cin >> processCount;
    clockPC += 2;

    if(processCount == 0) return; //end the function if the user enters 0 | Saves time & Memory

    //Adding processes to the queues
    for(int i = 1; i <= processCount; i++)
    {
        //The user should be prompted to enter the number of processes along with their priority associated with each queue.
        //Therefore, burst time and arrival time are randomly generated for each process.
        int priority, burstTime;
        clockPC += 7; 

        cout << "Add details to Process " << i << ": " << endl;  
        cout << "Process ID\t: " << i << endl;                   
        cout << "Process Priority: ";                            
        cin >> priority;                                         
        
        burstTime = rand() % processCount + 31; //randomly generate burst time while being non-zero non-negative
        
        cout << "Burst Time\t: " << burstTime << endl;           
        cout << "Arrival Time\t: " << clockPC << endl; 

        Process process(i , priority, clockPC, burstTime, burstTime, 0, 0); //remaining time is set to burst time initially
        //cout << "process details: " << process.getPid() << " " << process.getPriority() << " " << process.getArrivalTime() << " " << process.getBurstTime() << " " << process.getRemainingTime() << endl;
        
        /* 
            PRIORITY Grading:
                invalid --> below 0
                    q0  --> 0 - 2
                    q1  --> 3 - 8
                    q2  --> 3 - 8
                    q3  --> 9 & Above
        */
        clockPC += 5;

        if(priority >= 0 && priority < 3)
		{	
			q3.push(process);                    //add the process to the queue
			cout << "\nAdded to FIFO\n" << endl; //notify which queue the process is added to
		}
        else if(priority >= 3 && priority <= 8)
		{
			if( ( (rand()) % 2) == 0 ) //randomly decide which sub priority should the process be added to
			{	
				q1.push_back(process);
				cout << "\nAdded to SJF-high\n" << endl;
			}	
			else 
			{	
				q2.push_back(process);
				cout << "\nAdded to SJF-low\n" << endl; 
			}
		}
        else if(priority > 8) 
		{	
			q0.push_back(process);
			cout << "\nAdded to RR\n" << endl;
		}
        else
        {
            cout << "Invalid Priority" << endl;
        }
    }

    
}

void RoundRobin()
{
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    int count = 0;

    while (!q0.empty())
    {
        Process now = q0.front();
        q0.erase(q0.begin()); //remove the process from the front of the queue
        count++;

        now.setRemainingTime(now.getRemainingTime() - timeQuantum); //reduce the remaining time by the time quantum

        if (now.getRemainingTime() > 0) //if the process is not finished, it will be sent to the back of the queue
        {
            now.setWaitingTime(now.getWaitingTime() + timeQuantum);
            q0.push_back(now);
            count--;

            //cout << "Process " << now.getPid() << " remaining time: " << now.getRemainingTime() << " | Process is sent to the back of the queue" << endl;
        }
        else //if the process is finished, calculate waiting time and turnaround time
        {
            now.setTurnaroundTime(clockPC - now.getArrivalTime() + now.getWaitingTime());
            now.setWaitingTime(now.getTurnaroundTime() - now.getBurstTime());
            
            //cout << "Process " << now.getPid() << " turnaround time: " << now.getTurnaroundTime() << endl;
            //cout << "Process " << now.getPid() << " waiting time: " << now.getWaitingTime() << endl;

            totalTurnaroundTime += now.getTurnaroundTime();
            totalWaitingTime += now.getWaitingTime();

            processesMerged.push_back(now);//add the process to the merged vector
        }
        clockPC += timeQuantum;
    }

    float avgTurnaroundTime = ((float) totalTurnaroundTime) / count;
    float avgWaitingTime = ((float) totalWaitingTime) / count;

    cout << "Average Turnaround Time: " << avgTurnaroundTime << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
}

bool sortForSJF(Process atBegin, Process atEnd)
{
    return atBegin.getBurstTime() < atEnd.getBurstTime();
}

void ShortJobFirst(char lvl)
{
    if(lvl == 'h')
    {
        int totalTurnaroundTime = 0;
        int totalWaitingTime = 0;
        int size = q1.size();

        sort(q1.begin(), q1.end(), sortForSJF); //sort the vector according to the burst time

        while(!q1.empty())
        {
            Process now = q1.front();
            q1.erase(q1.begin());

            now.setRemainingTime(0); //process is finished

            //calculate waiting time and turnaround time
            now.setTurnaroundTime(clockPC - now.getArrivalTime() + now.getWaitingTime());
            now.setWaitingTime(now.getTurnaroundTime() - now.getBurstTime());
            clockPC += 5;

            totalTurnaroundTime += now.getTurnaroundTime();
            totalWaitingTime += now.getWaitingTime();
            //cout << "Process " << now.getPid() << " turnaround time: " << now.getTurnaroundTime() << endl;
            //cout << "Process " << now.getPid() << " waiting time: " << now.getWaitingTime() << endl;

            processesMerged.push_back(now);//add the process to the merged vector
        }

        cout << "Average Turnaround Time: " << ( (float)totalTurnaroundTime / size ) << endl;
        cout << "Average Waiting Time: " << ( (float)totalWaitingTime / size ) << endl;
    }
    else
    {
        int totalTurnaroundTime = 0;
        int totalWaitingTime = 0;
        int size = q2.size();

        sort(q2.begin(), q2.end(), sortForSJF); //sort the vector according to the burst time

        while(!q2.empty())
        {
            Process now = q2.front();
            q2.erase(q2.begin());

            now.setRemainingTime(0); //process is finished

            //calculate waiting time and turnaround time
            now.setTurnaroundTime(clockPC - now.getArrivalTime() + now.getWaitingTime());
            now.setWaitingTime(now.getTurnaroundTime() - now.getBurstTime());
            clockPC += 5;

            totalTurnaroundTime += now.getTurnaroundTime();
            totalWaitingTime += now.getWaitingTime();
            //cout << "Process " << now.getPid() << " turnaround time: " << now.getTurnaroundTime() << endl;
            //cout << "Process " << now.getPid() << " waiting time: " << now.getWaitingTime() << endl;
            processesMerged.push_back(now);//add the process to the merged vector
        }

        cout << "Average Turnaround Time: " << ( (float)totalTurnaroundTime / size ) << endl;
        cout << "Average Waiting Time: " << ( (float)totalWaitingTime / size ) << endl;
    }
    
}

void FIFO()
{
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    int size = q3.size();

    while(!q3.empty())
    {
        Process now = q3.front();
        q3.pop();

        now.setRemainingTime(0); //process is finished
        now.setTurnaroundTime(clockPC - now.getArrivalTime() + now.getWaitingTime());
        now.setWaitingTime(now.getTurnaroundTime() - now.getBurstTime());

        clockPC += 3;

        totalTurnaroundTime += now.getTurnaroundTime();
        totalWaitingTime += now.getWaitingTime();
        //cout << "Process " << now.getPid() << " turnaround time: " << now.getTurnaroundTime() << endl;
        //cout << "Process " << now.getPid() << " waiting time: " << now.getWaitingTime() << endl;

        processesMerged.push_back(now);//add the process to the merged vector
    }

    cout << "Average Turnaround Time: " << ( (float)totalTurnaroundTime / size ) << endl;
    cout << "Average Waiting Time: " << ( (float)totalWaitingTime / size ) << endl;
}

bool sortForFinal(Process atBegin, Process atEnd)
{
    return atBegin.getTurnaroundTime() < atEnd.getTurnaroundTime();
}

void printProcesses(vector<Process> processes)
{
    sort( processes.begin(), processes.end(), sortForFinal );

    cout << "PID\t | Priority\t | Arrival Time\t | Burst Time\t | Turnaround Time\t | Waiting Time" << endl; 

    while (!processes.empty())
    {
        Process currentProcess = processes.front();
        processes.erase(processes.begin()); // Remove the first element

        cout << currentProcess.getPid() << "\t | " << currentProcess.getPriority() << "\t\t | "
             << currentProcess.getArrivalTime() << "\t\t\t | " << currentProcess.getBurstTime() << "\t\t | " << currentProcess.getTurnaroundTime() << "\t\t\t | " << currentProcess.getWaitingTime() << endl;

        sleep(1); // Sleep for 1 second (to simulate the printing of the process details
    }
}

int main()
{
    int i = 0;
    
    clockPC++;
    processTOqueque();                      // calling the function to add processes to the queues
    if(processCount == 0) return -111;      // end the program if the user enters 0 | Saves time & Memory

	while(i < 4)                            // loop to execute the algorithms
    {
        clockPC++;
		switch(i) //remaineder of i divided by 4 --> 0,1,2,3
        {
            case 0:
                if (!q0.empty())
                {
                    cout << "Executing RR:" << endl;
                    RoundRobin();
                }
                break;

            case 1:
                if (!q1.empty())
                {
                    cout << "Executing SJF-high:" << endl;
                    ShortJobFirst('h');
                }
                break;

            case 2:
                if (!q2.empty())
                {
                    cout << "Executing SJF-low:" << endl;
                    ShortJobFirst('l');
                }
                break;

            case 3:
                if (!q3.empty())
                {
                    cout << "Executing FIFO:" << endl;
                    FIFO();
                }
                break;

            default:
                cout << "Invalid Queue" << endl;
                break;
        }

        //simulating "CPU should switch between queues after every 20 seconds"
        i++; 
        cout << "\nCPU is switching to the next queue in 20 seconds...\n" << endl;
        clockPC += 20;
        sleep(0.20); 
	}
    printProcesses(processesMerged);

    return 0;
}