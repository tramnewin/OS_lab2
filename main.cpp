#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct processInfo {
    int pid;
    int arrivalTime;
    int burstTime;


};

void final(vector<processInfo> info, int waitTime[], int turnaroundTime[], int contextSwitch[], int finishTime[], vector<int> timelineP){
    int totalTime = 0;
    float avgTime;
    vector<string> printTimelineP;
    //int indexP[4];
    int j;
    vector<processInfo> arrangedP;
    processInfo temp;
    int compareT = finishTime[0];
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + info[i].burstTime;
//i need to do the gantt chart before this table
//the table
    if (timelineP.size() == 0){
        for(int i = 0; i<info.size();i++){
            temp.pid = i;
            temp.arrivalTime = info[i].arrivalTime + waitTime[i];
            temp.burstTime = 0;
            arrangedP.push_back(temp);
        }
        for (int i = 1; i < arrangedP.size(); i++)
        {
            temp = arrangedP[i];
            j = i - 1;

            /* Move elements of arr[0..i-1], that are
            greater than key, to one position ahead
            of their current position */
            while (j >= 0 && arrangedP[j].arrivalTime > temp.arrivalTime)
            {
                arrangedP[j+1].arrivalTime =arrangedP[j].arrivalTime;
                arrangedP[j+1].pid = arrangedP[j].pid;
                arrangedP[j+1].burstTime = arrangedP[j].burstTime;
                //arr[j + 1] = arr[j];
                j = j - 1;
            }
            arrangedP[j + 1] = temp;
        }
        cout<<arrangedP[0].pid+1;
        //cout<< "|";
        for (int i = arrangedP[0].arrivalTime; i<arrangedP[1].arrivalTime-1;i++)
            cout<<" ";
        for (j = 1; j<arrangedP.size()-1;j++){
            cout << arrangedP[j].pid + 1;
            //cout<<"|";
            for (int i = arrangedP[j].arrivalTime; i < arrangedP[j+1].arrivalTime -1 && j>=0; i++ ){
                cout<<" ";
            }

        }
        cout<<arrangedP[arrangedP.size()-1].pid+1;
    }else{      //arrange it for SRTF via timelineP
        //cout<< "do sth here"<<endl;
        //copy int vector to char vector thats gonna print out
        for (int i =0; i< timelineP.size();i++){
            string a = to_string(timelineP[i]+1);
            printTimelineP.push_back(a);
        }
        j = 0;
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
        for (int i =0; i< printTimelineP.size();i++)
            cout<<printTimelineP[i];

    }
    j =0;
    cout<< endl;
    // Loop to store largest number to compareT
    for(int i = 1;i < info.size(); i++) {

        if(compareT < finishTime[i])
            compareT = finishTime[i];
    }

    for(int i = 0; i< compareT; i++){
        cout<< "*";
    }
    cout<< endl;
    cout<< "(each number represents the process id(pid))"<< endl;
    cout<<"(each star represents one ms)"<<endl;
    cout<< "PID\tarrival\tCPU-burst\tfinish\twaiting time\tturnaround\tNo. of context\n";
    for (int i = 0; i < info.size(); i++){
        cout << info[i].pid <<"\t\t";
        cout << info[i].arrivalTime <<"\t\t";
        cout << info[i].burstTime << "\t\t";
        cout<< finishTime[i]<<"\t\t\t"<<waitTime[i]<<"\t\t\t"<<turnaroundTime[i]<<"\t\t\t"<<contextSwitch[i]<<endl;
    }
    avgTime = (float)totalTime /(float)info.size();
    cout<< "Average CPU burst time = "<< avgTime<< " ms\n";

    totalTime = 0;
    avgTime = 0;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + waitTime[i];
    avgTime = (float)totalTime /(float)info.size();
    cout<< "Average waiting time = "<< avgTime<< " ms\n";

    totalTime = 0;
    avgTime = 0;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + turnaroundTime[i];
    avgTime = (float)totalTime /(float)info.size();
    cout<< "Average turnaround time = "<< avgTime<< " ms\n";

    totalTime = 0;
    avgTime = 0;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + contextSwitch[i];
    cout<< "Total No. of context switching performed = "<< totalTime<< endl;
}

vector<processInfo> setInput(string filename){
    ifstream inputfile;
    stringstream ss;
    string substring;
    string anotherstring;
    string temp;
    vector<processInfo> info;
    processInfo tempo;
    int count =0;
    inputfile.open(filename);
    if (!inputfile.is_open()) {
        cout << "Unable to read file";
    }
    ss << inputfile.rdbuf();
    while (getline(ss, anotherstring)) {        //1 0 10 -> anotherString
        substring = anotherstring;                    //replicate of another string
        stringstream iss(substring);               //get string into iss which is string stream
        getline(iss, temp, ' ');          //parse with delimiter temp has 1 in string datatype
        stringstream convertStoI(temp);      //convert string to int store it into val
        int val = 0;
        convertStoI>> val;
        tempo.pid = val;
        convertStoI.str("");
        convertStoI.clear();
        getline(iss, temp, ' ');          //parse with delimiter temp has 0 in string datatype
        convertStoI << temp;
        convertStoI >> val;
        tempo.arrivalTime = val;
        convertStoI.clear();
        getline(iss, temp, ' ');          //parse with delimiter temp has 0 in string datatype
        convertStoI << temp;
        convertStoI >> val;
        tempo.burstTime = val;
        info.push_back(tempo);

    }
    inputfile.close();
    return info;
}


//service time is the amount of time after which a process can start execution.
//it keeps track of the time stamp
void FCFS(vector<processInfo> info){
    //vector<int> processes;
    vector<int> timelineP;
    int serviceTime[info.size()];
    int waitTime[info.size()], turnaroundTime[info.size()], finishTime[info.size()];
    int  contextSwitch[info.size()];
    for(int i = 0; i<info.size();i++)
        contextSwitch[i] = 0;
    serviceTime[0] = info[0].arrivalTime;
    waitTime[0] = 0;
    cout<<"************ Scheduling algorithm : FCFS *******************\n";
    cout<<"************************************************************\n";
    // calculating waiting time
    for (int i = 1; i < info.size() ; i++)
    {

        // Add burst time of previous processes
        serviceTime[i] = serviceTime[i-1] + info[i-1].burstTime;
        //each service time is the finish time of the previous process
        finishTime[i-1] = serviceTime[i];

        // Find waiting time for current process =
        // sum - at[i]
        waitTime[i] = serviceTime[i] - info[i].arrivalTime;

        // If waiting time for a process is in negative
        // that means it is already in the ready queue
        // before CPU becomes idle so its waiting time is 0
        if (waitTime[i] < 0)
            waitTime[i] = 0;
    }

    //calculate the last process's finish time
    finishTime[info.size()-1] = serviceTime[info.size()-1] + info[info.size()-1].burstTime;

    //calculate turnaround time = burst time + wait time
    for (int i = 0; i < info.size() ; i++)
        turnaroundTime[i] = info[i].burstTime + waitTime[i];

    final(info, waitTime, turnaroundTime, contextSwitch,finishTime,timelineP);
}

void SRTF(vector<processInfo> info){
    vector<int> timelineP;
    int reBurstTime[info.size()];
    int completeP = 0, t =0, minmReTime = INT_MAX;
    int shortestP = 0, finishTime;
    bool flag = false;
    int currentP =0;
    //bool queueP[info.size()];
    int waitTime[info.size()], turnaroundTime[info.size()], finishtimeArr[info.size()];
    int contextSwitch[info.size()];
    //for (int i = 0; i< info.size();i++)
    //queueP[i]= false;
    cout<<"************ Scheduling algorithm : SRTF *******************\n";
    cout<<"************************************************************\n";
    for(int i = 0; i<info.size();i++)
        contextSwitch[i] = 0;
    for (int i = 0; i < info.size(); i++)
        reBurstTime[i] = info[i].burstTime;

    // Process until all processes gets completed
    while (completeP < info.size()) {
        // Find process with the smallest remaining time among the
        // processes that arrives up to the current time
        currentP = shortestP;
        for (int j = 0; j < info.size(); j++) {
            //int temp = shortestP;
            //cout<< j<<" ";
            //if the arrival time of the process is before the current time and
            // has remaining burst time less than the smallest remaining time (but making sure it is positive time)
            if ((info[j].arrivalTime <= t) && (reBurstTime[j] < minmReTime) && reBurstTime[j] > 0) {
                //cout<<"time: "<<t<<"current P: "<<currentP+1<< " "<<info[j].pid<<" & "<< shortestP + 1<<endl;
                int temp = shortestP;
                minmReTime = reBurstTime[j];    //update the smallest remaining time to the new process
                shortestP = j;                  //change the process with the smallest burst time
                // if (shortestP )
                flag = true;                    //set the flag to true


                //i think the increment of context switching would be here cuz we are updating the new process
                //and preempt the current process??

            }
        }

        //if we cannot find any other process that has the smallest remaining burst time at the current time,
        //we increment the time
        if (flag == false) {
            t++;
            continue;
        }

        // Reduce remaining time by one
        reBurstTime[shortestP]--;
        timelineP.push_back(currentP);


        //if (queueP[shortestP] == true)
        //  queueP[shortestP] = false;
        // Update minimum
        minmReTime = reBurstTime[shortestP];
        if (minmReTime == 0)    //if the smallest remaining time is 0, the process has finished executed
            minmReTime = INT_MAX;   //change the smallest remaining time so that it can get the next process
        //with the smallest burst time
        //cout<<"time: "<<t<<"current P: "<<currentP+1<< " & last timelineP is"<< timelineP[timelineP.size()-1]+1<<"remaining burst time of "
        //  <<timelineP[timelineP.size()-2]+1<<"is "<<reBurstTime[timelineP[timelineP.size()-2]]<<"reburt of current P: "<<
        // reBurstTime[currentP]<<endl;
        if(t>0 && timelineP[timelineP.size()-2] != currentP && reBurstTime[timelineP[timelineP.size()-2]]> reBurstTime[currentP]){
            contextSwitch[timelineP[timelineP.size()-2]]++;
        }

        // If a process gets completely executed
        if (reBurstTime[shortestP] == 0) {
            // Increment  the number of completed process
            completeP++;
            flag = false;
            // Find finish time of current
            // process
            finishTime = t + 1;
            finishtimeArr[shortestP] = finishTime;  //store the process's finish time into the array
            // Calculate waiting time
            waitTime[shortestP] = finishTime - info[shortestP].burstTime - info[shortestP].arrivalTime;
            // If waiting time for a process is in negative
            // that means it is already in the ready queue
            // before CPU becomes idle so its waiting time is 0
            if (waitTime[shortestP] < 0)
                waitTime[shortestP] = 0;
        }
        // Increment time
        t++;
    }
    for (int i = 0; i < info.size(); i++)
        turnaroundTime[i] = info[i].burstTime + waitTime[i];

    final(info, waitTime, turnaroundTime, contextSwitch,finishtimeArr, timelineP);

}

/* 1. Declare arrival[], burst[], wait[], turn[] arrays and initialize them. Also declare a timer
   variable and initialize it to zero. To sustain the original burst array create another
   array (temp_burst[]) and copy all the values of burst array in it.

2. To keep a check we create another array of bool type which keeps the record of whether a
   process is completed or not. we also need to maintain a queue array which contains the process
   indices (initially the array is filled with 0).

3. Now we increment the timer variable until the first process arrives and when it does, we add the
   process index to the queue array

4. Now we execute the first process until the time quanta and during that time quanta, we check
   whether any other process has arrived or not and if it has then we add the index in the queue
   (by calling the fxn. queueUpdate()).

5. Now, after doing the above steps if a process has finished, we store its exit time and
   execute the next process in the queue array. Else, we move the currently executed process at
   the end of the queue (by calling another fxn. queueAdj()) when the time slice expires.

6. The above steps are then repeated until all the processes have been completely executed. If a
   scenario arises where there are some processes left but they have not arrived yet, then we
   shall wait and the CPU will remain idle during this interval.*/

//the new arrival process is added to the queue
void queueUpdate(int queue[], int infosize, int maxProccessIndex){
    int zeroIndex;
    for(int i = 0; i < infosize; i++){
        if(queue[i] == 0){
            zeroIndex = i;
            break;
        }
    }
    queue[zeroIndex] = maxProccessIndex + 1;
}


//swap the processes in the queue, the process that has been in the CPU,
//at the front of the queue, is being moved to the end of the queue (happens after each quantum time)
void queueAdj(int queue[], int infosize){

    for(int i = 0; (i < infosize - 1) && (queue[i + 1] != 0) ; i++){
        int temp = queue[i];
        queue[i] = queue[i+1];
        queue[i+1] = temp;
    }
}

//check whether there's any process arrives
void newProcessArrival(int t, vector<processInfo> info, int maxProccessIndex, int queue[]){
    if(t <= info[info.size() - 1].arrivalTime){     //if the time is less than the last process's arrival time
        bool ArrivalP = false;                    //initialize arrivalP as false
        for(int j = (maxProccessIndex+1); j < info.size(); j++){
            if(info[j].arrivalTime <= t){
                if(maxProccessIndex < j){
                    maxProccessIndex = j;
                    ArrivalP = true;
                }
            }
        }
        //adds the incoming process to the ready queue
        //(if any arrives)
        if(ArrivalP)
            queueUpdate(queue, info.size(), maxProccessIndex);
    }
}

void queueUpdation(int queue[],int n, int maxProccessIndex){
    int zeroIndex;
    for(int i = 0; i < n; i++){
        if(queue[i] == 0){
            zeroIndex = i;
            break;
        }
    }
    queue[zeroIndex] = maxProccessIndex + 1;
}

void queueMaintainence(int queue[], int n){
    for(int i = 0; (i < n-1) && (queue[i+1] != 0) ; i++){
        int temp = queue[i];
        queue[i] = queue[i+1];
        queue[i+1] = temp;
    }
}

void checkNewArrival(int timer, int arrival[], int n, int maxProccessIndex,int queue[]){
    if(timer <= arrival[n-1]){
        bool newArrival = false;
        for(int j = (maxProccessIndex+1); j < n; j++){
            if(arrival[j] <= timer){
                if(maxProccessIndex < j){
                    maxProccessIndex = j;
                    newArrival = true;
                }
            }
        }
        //adds the incoming process to the ready queue
        //(if any arrives)
        if(newArrival)
            queueUpdation(queue,n, maxProccessIndex);
    }
}
void RR(vector<processInfo> info, int quantum) {

    int t = 0, maxProccessIndex = 0;
    float avgWait = 0, avgTT = 0;
    vector<int> temp;
    vector<int> temp2;
    int arrival[info.size()], burst[info.size()], waitTime[info.size()], turnaroundTime[info.size()], queue[info.size()], procBurst[info.size()], contextSwitch[info.size()],finishTime[info.size()];
    bool complete[info.size()];
    vector<int> timelineP;
    vector<int> times;
    vector<int> timestamp;
    for(int i = 0; i < info.size(); i++)
        arrival[i] = info[i].arrivalTime;


    for(int i = 0; i < info.size(); i++){
        burst[i] = info[i].burstTime;
        procBurst[i] = burst[i];
    }

    for(int i = 0; i < info.size(); i++){ //Initializing the queue and complete array
        complete[i] = false;
        queue[i] = 0;
        contextSwitch[i] = 0;
        finishTime[i] =0;
    }
    while(t < arrival[0]) //Incrementing Timer until the first process arrives
        t++;
    queue[0] = 1;

    while(true){
        bool flag = true;
        for(int i = 0; i < info.size(); i++){
            if(procBurst[i] != 0){
                flag = false;
                break;
            }
        }
        if(flag)
            break;

        for(int i = 0; (i < info.size()) && (queue[i] != 0); i++){
            int ctr = 0;
            while((ctr < quantum) && (procBurst[queue[0] - 1] > 0)){
                procBurst[queue[0] - 1] -= 1;
                t += 1;
                //temp =t;
                ctr++;
                //timelineP.push_back(temp);
                //Checking and Updating the ready queue until all the processes arrive
                checkNewArrival(t, arrival, info.size(), maxProccessIndex, queue);
            }
            //cout<< t<<"\t"<<queue[0]<<"\t"<<procBurst[queue[0]-1] <<"\t"<<queue[1]<<"\t"<<procBurst[queue[1]-1] <<endl;
            timestamp.push_back(t);
            temp.push_back(queue[0]-1);

            if(t>=quantum && procBurst[queue[0]-1]>0){
                contextSwitch[queue[0]-1]++;
            }
            //If a process is completed then store its exit time
            //and mark it as completed
            if((procBurst[queue[0] - 1] == 0) && (complete[queue[0] - 1] == false)){
                //turnaroundTime array currently stores the completion time
                finishTime[queue[0]-1] = t;
                turnaroundTime[queue[0] - 1] = t;
                complete[queue[0]-1] = true;
            }

            //checks whether or not CPU is idle
            bool idle = true;
            if(queue[info.size() - 1] == 0){
                for(int i = 0; i < info.size() && queue[i] != 0; i++){
                    if(complete[queue[i]-1] == false){
                        idle = false;
                    }
                }
            }
            else
                idle = false;

            if(idle){
                t++;
                checkNewArrival(t, arrival, info.size(), maxProccessIndex, queue);
            }

            //Maintaining the entries of processes
            //after each premption in the ready Queue
            queueMaintainence(queue, info.size());
        }
    }

    for(int i = 0; i < info.size(); i++){
        turnaroundTime[i] = turnaroundTime[i] - info[i].arrivalTime;
        waitTime[i] = turnaroundTime[i] - info[i].burstTime;
    }
    int i = 0;
    while(i< timestamp.size()){
        for(int j = i +1; j<timestamp.size();j++){
            if(timestamp[j] == timestamp[i]){
                timestamp[j] = 0;
                //timestamp.erase(timestamp.begin()+j);
                temp[j] = 0;
                //temp.erase(temp.begin()+j);

            }else{
                break;
            }
        }
        i++;
    }

    for(int k =0; k < timestamp.size();k++){
        if(timestamp[k]!=0){
            times.push_back(timestamp[k]);
            temp2.push_back(temp[k]);
        }
    }
    for(int j = 0; j<times.size();j++)
        cout<<times[j] <<"\t"<< temp2[j]<<endl;
    i=0;
    for(int j=0;j<timestamp[i];j++){
        timelineP.push_back(temp[i]);
    }
    i++;
    while(i<times.size()){
        for(int j= times[i-1]; j<times[i];j++){
            timelineP.push_back(temp2[i]);
        }
        i++;
    }
    final(info, waitTime,turnaroundTime,contextSwitch,finishTime,timelineP);
}

int main() {
    //string filename;
    string input;
    int quantum = 0;
    string anotherstring;
    string substring;
    stringstream ss;
    vector<processInfo> result;
    getline(cin, input);
    ss<< input;
    cout<<"************************************************************\n";
    while(getline(ss, anotherstring,' ')){
        if (anotherstring == "input_file.txt"){
            result = setInput(anotherstring);
        } else{
            if(anotherstring == "FCFS"){
                FCFS(result);
            } else if(anotherstring == "SRTF"){
                SRTF(result);

            }else if(anotherstring == "RR"){
                getline(ss, substring, ' ');
                stringstream convertStoI(substring);      //convert string to int store it into val
                convertStoI>> quantum;
                RR(result, quantum);
            }
        }
    }


    return 0;
}
