#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;
//holds the iformation of the processes
struct processInfo {
    int pid;
    int arrivalTime;
    int burstTime;
    int burstTimecopy;
    int finishTime;
    int turnaroundTime;
    int waitTime;
    bool isComplete;
    bool inQueue;
};

//prints out the result onto the console
void final(vector<processInfo> info,  int contextSwitch[], vector<int> timelineP){
    int totalTime = 0;
    float avgTime;
    vector<string> printTimelineP;  //holds the timeline of the process (RR, SRTF scheduler) in each unit time
    int j;
    vector<processInfo> arrangedP;  //arrangeP has the timeline for FCFS scheduler
    processInfo temp;
    int compareT = info[0].finishTime;  //compareT has the time stamp when all process have been executed
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + info[i].burstTime;  //calculate the total time of processes' burst time
    if (timelineP.size() == 0){     //if the scheduler is FCFS, the timeline for processes will be made in the code block below
        for(int i = 0; i<info.size();i++){      //store the processes' information into arrangedP vector
            temp.pid = i;
            temp.arrivalTime = info[i].arrivalTime + info[i].waitTime;
            temp.burstTime = 0;
            arrangedP.push_back(temp);
        }
        for (int i = 1; i < arrangedP.size(); i++)      //sort the vector in increment order in terms of arrival time
        {
            temp = arrangedP[i];
            j = i - 1;

            /* Move elements of arrangeP[0..i-1], that are
            greater than the one after it, to one position ahead
            of their current position */
            while (j >= 0 && arrangedP[j].arrivalTime > temp.arrivalTime)
            {
                arrangedP[j+1].arrivalTime =arrangedP[j].arrivalTime;
                arrangedP[j+1].pid = arrangedP[j].pid;
                arrangedP[j+1].burstTime = arrangedP[j].burstTime;
                j = j - 1;
            }
            arrangedP[j + 1] = temp;
        }
        //prints out the process id onto the Gantt chart
        cout<<arrangedP[0].pid+1;
        for (int i = arrangedP[0].arrivalTime; i<arrangedP[1].arrivalTime-1;i++)
            cout<<" ";
        for (j = 1; j<arrangedP.size()-1;j++){
            cout << arrangedP[j].pid + 1;
            for (int i = arrangedP[j].arrivalTime; i < arrangedP[j+1].arrivalTime -1 && j>=0; i++ ){
                cout<<" ";
            }

        }
        cout<<arrangedP[arrangedP.size()-1].pid+1;
    }else{      //if the scheduling algorithm is SRTF or RR
        //convert int vector to char vector for printing out
        for (int i =0; i< timelineP.size();i++){
            string a = to_string(timelineP[i]+1);
            printTimelineP.push_back(a);
        }
        j = 0;
        //since timelineP maps the process on every time unit and we can only print out the process when it
        //gets to the cpu in the first ms, replace the remaining duplicates in that time block with " "
        while(j < printTimelineP.size()){
            if (printTimelineP[j] != " "){
                for (int i = j+1; i < printTimelineP.size();i++){
                    if(printTimelineP[i] == printTimelineP[j]){
                        printTimelineP[i] = " ";
                    }else{
                        break;
                    }

                }
            }
            j++;
        }
        for (int i =0; i< printTimelineP.size();i++)    //prints out the process id onto the Gantt chart
            cout<<printTimelineP[i];

    }
    j =0;
    cout<< endl;
    // Loop to get finish time of the processes execution
    for(int i = 1;i < info.size(); i++) {

        if(compareT < info[i].finishTime)
            compareT = info[i].finishTime;
    }
    //prints out '*', each represents 1ms
    for(int i = 0; i< compareT; i++){
        cout<< "*";
    }
    cout<< endl;
    //prints out the results onto the console
    cout<< "(each number represents the process id(pid))"<< endl;
    cout<<"(each star represents one ms)"<<endl;
    cout<< "PID\tarrival\tCPU-burst\tfinish\twaiting time\tturnaround\tNo. of context\n";
    for (int i = 0; i < info.size(); i++){
        cout << info[i].pid <<"\t\t";
        cout << info[i].arrivalTime <<"\t\t";
        cout << info[i].burstTime << "\t\t";
        cout<< info[i].finishTime<<"\t\t\t"<<info[i].waitTime<<"\t\t\t"<<info[i].turnaroundTime<<"\t\t\t"<<contextSwitch[i]<<endl;
    }
    avgTime = (float)totalTime /(float)info.size(); //prints out the calculated average burst time
    cout<< "Average CPU burst time = "<< avgTime<< " ms\n";
    //reset the variables to 0 for the next calculation
    totalTime = 0;
    avgTime = 0;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + info[i].waitTime;
    avgTime = (float)totalTime /(float)info.size();     //prints out the calculated waiting time
    cout<< "Average waiting time = "<< avgTime<< " ms\n";
    //reset the variables to 0 for the next calculation
    totalTime = 0;
    avgTime = 0;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + info[i].turnaroundTime;
    avgTime = (float)totalTime /(float)info.size();     //prints out the calculated turnaround time
    cout<< "Average turnaround time = "<< avgTime<< " ms\n";
    //reset the variables to 0 for the next calculation
    totalTime = 0;
    avgTime = 0;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + contextSwitch[i];   //prints out the total amount of context switch
    cout<< "Total No. of context switching performed = "<< totalTime<< endl;
}

vector<processInfo> setInput(string filename){  //reads the file and stores the information into info vector
    ifstream inputfile;
    stringstream ss;
    string substring;
    string anotherstring;
    string temp;
    vector<processInfo> info;
    processInfo tempo;
    inputfile.open(filename);           //open the file and read the file
    if (!inputfile.is_open()) {
        cout << "Unable to read file";
    }
    ss << inputfile.rdbuf();              //stores the file into a stream buffer
    while (getline(ss, anotherstring)) {        //get the information of a process line by line
        substring = anotherstring;                    //replicate of another string
        stringstream iss(substring);               //get string into iss which is string stream
        getline(iss, temp, ' ');          //parse with delimiter, temp has first value of the line (pid)
        stringstream convertStoI(temp);      //convert string to int store it into val
        int val = 0;
        convertStoI>> val;
        tempo.pid = val;                        //stores pid into tempo
        convertStoI.str("");
        convertStoI.clear();                    //flush the convertStoI for next usage
        getline(iss, temp, ' ');          //parse with delimiter temp has the next value in the line (arrival)
        convertStoI << temp;
        convertStoI >> val;
        tempo.arrivalTime = val;                //stores arrival time into tempo
        convertStoI.clear();                    //flush the convertStoI for next usage
        getline(iss, temp, ' ');          //parse with delimiter temp has the last value in the line (burst)
        convertStoI << temp;
        convertStoI >> val;
        tempo.burstTime = val;                  //stores burst time into tempo
        tempo.burstTimecopy = tempo.burstTime;  //copy burst time into burst time copy to use iin SRTF and RR
        tempo.finishTime = 0;                   //sets the output requirement to 0/false
        tempo.turnaroundTime = 0;
        tempo.isComplete = false;
        tempo.inQueue = false;
        info.push_back(tempo);                  //push tempo to info vector

    }
    inputfile.close();                          //close the input file and return the vector
    return info;
}


//first come first serve scheduling algorithm
void FCFS(vector<processInfo> info){
    vector<int> timelineP;
    //timer is the amount of time after which a process can start execution.
    //it keeps track of the time stamp
    int timer[info.size()];
    int  contextSwitch[info.size()];
    for(int i = 0; i<info.size();i++)
        contextSwitch[i] = 0;
    timer[0] = info[0].arrivalTime;   //set the service time the first arrival process
    info[0].waitTime = 0;
    cout<<"************ Scheduling algorithm : FCFS *******************\n";
    cout<<"************************************************************\n";
    // calculating waiting time, finish time
    for (int i = 1; i < info.size() ; i++)
    {

        // Add burst time of previous processes
        timer[i] = timer[i - 1] + info[i - 1].burstTime;
        //each timer is the finish time of the previous process
        info[i-1].finishTime = timer[i];
        // Find waiting time for current process
        info[i].waitTime = timer[i] - info[i].arrivalTime;
        // If waiting time for a process is in negative
        // that means it is already in the ready queue
        // before CPU becomes idle so its waiting time is 0
        if (info[i].waitTime < 0)
            info[i].waitTime = 0;
    }

    //calculate the last process's finish time
    info[info.size()-1].finishTime = timer[info.size() - 1] + info[info.size() - 1].burstTime;

    //calculate turnaround time = burst time + wait time
    for (int i = 0; i < info.size() ; i++)
        info[i].turnaroundTime = info[i].burstTime + info[i].waitTime;
    //prints out the result
    final(info,contextSwitch,timelineP);
}

//shortest remaining time scheduling algorithm
void SRTF(vector<processInfo> info){
    vector<int> timelineP;
    //completeP keeps track of the number of processes finish, t is the time,
    // minmReTime is the smallest remaining burst time of a process.
    int completeP = 0, t =0, minmReTime = INT_MAX;
    //shortestP is the process that has the smallest remaining burst time of a process
    //finishTime has the finish time of a process
    int shortestP = 0, finishTime;
    bool flag = false;      //flag keeps track of whether the smallest remaining burst time is found
    int currentP =0;        //currentP keeps track of which process is being processes during time t
    int contextSwitch[info.size()];
    cout<<"************ Scheduling algorithm : SRTF *******************\n";
    cout<<"************************************************************\n";
    for(int i = 0; i<info.size();i++)
        contextSwitch[i] = 0;       //set up the context switch as default

    // Process until all processes gets completed
    while (completeP < info.size()) {
        //as the process with the smallest remaining burst time is being placed into cpu, the current process
        //is always the one with the smallest remaining burst time
        currentP = shortestP;
        // Find process with the smallest remaining time among the
        // processes that arrives up to the current time
        for (int j = 0; j < info.size(); j++) {

            //if the arrival time of the process is before the current time and
            // has remaining burst time less than the smallest remaining time (but making sure it is positive time)
            if ((info[j].arrivalTime <= t) && (info[j].burstTimecopy < minmReTime) && info[j].burstTimecopy > 0) {
                //int temp = shortestP;
                minmReTime = info[j].burstTimecopy;    //update the smallest remaining time to the new process
                shortestP = j;                  //change the process with the smallest burst time
                // if (shortestP )
                flag = true;                    //set the flag to true

            }
        }

        //if we cannot find any other process that has the smallest remaining burst time at the current time,
        //we increment the time
        if (flag == false) {
            t++;
            continue;
        }

        // Reduce remaining time by one
        info[shortestP].burstTimecopy--;
        timelineP.push_back(currentP);  //push the process onto the timeline

        // Update smallest remaining burst time
        minmReTime = info[shortestP].burstTimecopy;
        if (minmReTime == 0)    //if the smallest remaining time is 0, the process has finished executed
            minmReTime = INT_MAX;   //change the smallest remaining time so that it can get the next process
                                    //with the smallest burst time
        //if the current process in cpu is different than the one being process on the previous time
        //and it still has burst time, context switch occurs
        if(t>0 && timelineP[timelineP.size()-2] != currentP && info[timelineP[timelineP.size()-2]].burstTimecopy > info[currentP].burstTimecopy){
            contextSwitch[timelineP[timelineP.size()-2]]++; //increment the context switch of the process being processed previously
        }

        // If a process gets completely executed
        if (info[shortestP].burstTimecopy == 0) {
            // Increment  the number of completed process
            completeP++;
            flag = false;
            // Find finish time of current process
            finishTime = t + 1;
            info[shortestP].finishTime = finishTime;  //store the process's finish time into the array
            // Calculate waiting time
            info[shortestP].waitTime = finishTime - info[shortestP].burstTime - info[shortestP].arrivalTime;
            // If waiting time for a process is in negative
            // that means it is already in the ready queue
            // before CPU becomes idle so its waiting time is 0
            if (info[shortestP].waitTime < 0)
                info[shortestP].waitTime = 0;
        }
        // Increment time
        t++;
    }
    //calculate the turnaound time
    for (int i = 0; i < info.size(); i++)
        info[i].turnaroundTime = info[i].burstTime + info[i].waitTime;
    //calls final function to print the result onto the console
    final(info, contextSwitch, timelineP);

}


//check for new arriving process that happens during an excution of a process or when the CPU is idle
void checkForNewArrivals(processInfo info[], const int quantity, const int t, queue<int> &theQueue)
{
    for (int i = 0; i < quantity; i++)
    {
        processInfo proc = info[i]; //runs through each process provided in the input file
        // if there's a process arrives during the time t and is not yet on the queue as well as not done
        if (proc.arrivalTime <= t && !proc.inQueue && !proc.isComplete)
        {
            info[i].inQueue = true; //push it onto the queue and set inQueue to true
            theQueue.push(i);
        }
    }
}


//core function of RR algorithm, calculates wait time, turnaround time , context switch, finish time
// of each process accordingly
void updateQueue(processInfo info[], const int quantity, const int quantum, queue<int> &readyQueue, int &t, int &compeletP, int contextSwitch[], vector<int> &timelineP)
{
    int i = readyQueue.front(); //i has the process that needs to be placed into the cpu
    readyQueue.pop();       //pop that process out of the queue

    // when it's remaining burst time is less than time quantum
    if (info[i].burstTimecopy <= quantum)
    {
        info[i].isComplete = true;     // mark it completed and increase the current time by adding the remaining burst time
        t += info[i].burstTimecopy;
        info[i].finishTime = t;      // and calculate its finish time, waiting time along with turnaround time
        info[i].waitTime = info[i].finishTime - info[i].arrivalTime - info[i].burstTime;
        info[i].turnaroundTime = info[i].waitTime + info[i].burstTime;
        for(int j = 0; j < info[i].burstTimecopy; j++){   //iterate through the remaining burst time and
            timelineP.push_back(info[i].pid - 1);       //push the process onto the timeline
        }
        //if the wait time is negative, the process is alrady on the queue, so set the wait time to 0
        if (info[i].waitTime < 0)
            info[i].waitTime = 0;
        //as it already finish its task, set the burst time to 0
        info[i].burstTimecopy = 0;

        // if all the process are not yet in the queue,
        // we check for new arrivals
        if (compeletP != quantity)
        {
            checkForNewArrivals(info, quantity, t, readyQueue);
        }
    }
    else
    {
        // if the process's burst time is larger than the quantum time
        // the process is going to be preempted onto the queue
        //first, subtract the time of the remaining burst time by the quantum time
        info[i].burstTimecopy -= quantum;
        for(int j = 0; j<quantum;j++){   //iterate through the quantum time
            timelineP.push_back(info[i].pid - 1);     //push the process onto the timeline
        }
        t += quantum;     //update the time by adding the quantum time

        // if all the process are not yet in the queue,
        // check for new arrivals
        if (compeletP != quantity)
        {
            checkForNewArrivals(info, quantity, t, readyQueue);
        }
        // insert the incomplete process back into the queue
        //and increment the contextswitch of the process
        if(info[i].burstTimecopy != quantum)
            contextSwitch[i]++;
        readyQueue.push(i);
    }
}


void RR(processInfo info[], int quantity ,int quantum) {
    cout<<"************ Scheduling algorithm : RR *******************\n";
    cout<<"************************************************************\n";
    queue<int> theQueue;
    vector<processInfo> proc;
    int contextSwitch[quantity];
    vector<int> timelineP;
    theQueue.push(0); // initially, pushing the first process which arrived first
    info[0].inQueue = true;
    int t = 0; // holds the current time after each process has been executed
    int completeP = 0; // holds the number of programs executed so far
    for(int i = 0; i<quantity;i++)
        contextSwitch[i] = 0;   //initialize the context switch as 0
    while (!theQueue.empty())   //runs RR scheduling until the queue is empty
    {
        updateQueue(info, quantity, quantum, theQueue, t, completeP,contextSwitch,timelineP);
    }
    for(int i = 0; i<quantity;i++){
        proc.push_back(info[i]);
    }
    //prints out the result onto the console
    final(proc,contextSwitch,timelineP);

}

int main() {
    string input;
    int quantum = 0;
    string anotherstring;
    string substring;
    stringstream ss;
    vector<processInfo> info;
    vector<int> temp;
    vector<int> temp2;
    vector<int> timelineP;
    vector<int> times;
    vector<int> timestamp;
    getline(cin, input);
    ss<< input;
    cout<<"************************************************************\n";
    while(getline(ss, anotherstring,' ')){
        if (anotherstring == "input_file.txt"){
            info = setInput(anotherstring);
        } else{
            if(anotherstring == "FCFS"){
                FCFS(info);
            } else if(anotherstring == "SRTF"){
                SRTF(info);

            }else if(anotherstring == "RR"){
                getline(ss, substring, ' ');
                stringstream convertStoI(substring);      //convert string to int store it into val
                convertStoI >> quantum;
                int quantity = info.size();
                processInfo processes[quantity];
                for(int i =0; i< info.size();i++){
                    processes[i] = info[i];
                }
                RR(processes, quantity, quantum);

            }
        }
    }

    return 0;
}
