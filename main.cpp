#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct processInfo {
    int pid;
    int arrivalTime;
    int burstTime;


};

void findAVG(vector<processInfo> info, int waitTime[], int turnaroundTime[], int contextSwitch[]){
    int totalTime = 0;
    float avgTime;
    for (int i =0; i< info.size();i++)
        totalTime = totalTime + info[i].burstTime;

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
        exit;
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
    int serviceTime[info.size()];
    int waitTime[info.size()], turnaroundTime[info.size()], finishTime[info.size()];
    int  contextSwitch[info.size()];
    for(int i = 0; i<info.size();i++)
        contextSwitch[i] = 0;
    serviceTime[0] = info[0].arrivalTime;
    waitTime[0] = 0;

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

    findAVG(info, waitTime, turnaroundTime, contextSwitch);
}

void SRTF(vector<processInfo> info){
    int reBurstTime[info.size()];
    int completeP = 0, t =0, minmReTime = INT_MAX;
    int shortestP = 0, finishTime;
    bool flag = false;
    int waitTime[info.size()], turnaroundTime[info.size()], finishtimeArr[info.size()];
    int contextSwitch[info.size()];

    for(int i = 0; i<info.size();i++)
        contextSwitch[i] = 0;
    for (int i = 0; i < info.size(); i++)
        reBurstTime[i] = info[i].burstTime;

    // Process until all processes gets completed
    while (completeP < info.size()) {
        // Find process with the smallest remaining time among the
        // processes that arrives up to the current time
        for (int j = 0; j < info.size(); j++) {
            int temp = shortestP;
            //if the arrival time of the process is before the current time and
            // has remaining burst time less than the smallest remaining time (but making sure it is positive time)
            if ((info[j].arrivalTime <= t) && (reBurstTime[j] < minmReTime) && reBurstTime[j] > 0) {
                minmReTime = reBurstTime[j];    //update the smallest remaining time to the new process
                shortestP = j;                  //change the process with the smallest burst time
                flag = true;                    //set the flag to true
                contextSwitch[temp]++;
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

        // Update minimum
        minmReTime = reBurstTime[shortestP];
        if (minmReTime == 0)    //if the smallest remaining time is 0, the process has finished executed
            minmReTime = INT_MAX;   //change the smallest remaining time so that it can get the next process
                                    //with the smallest burst time

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

    findAVG(info, waitTime, turnaroundTime, contextSwitch);

}



int main() {

    string filename;
    string algo;
    vector<processInfo> result;
    cout<<"Enter file name: ";
    cin>> filename;
    result = setInput(filename);
    for (int i = 0; i < result.size(); i++){
        cout << result[i].pid <<"\t";
        cout << result[i].arrivalTime <<"\t";
        cout << result[i].burstTime << endl;
    }
    cout<< "Enter scheduling algorithm";
    cin>> algo;
    if(algo == "FCFS"){
        FCFS(result);
    } else if(algo == "SRTF"){
        SRTF(result);

    }else if(algo == "RR"){


    }

    return 0;
}
