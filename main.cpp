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

void FCFS(vector<processInfo> info){
    int serviceTime[info.size()];
    serviceTime[0] = info[0].arrivalTime;
    cout<<endl;

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

    }else if(algo == "RR"){


    }

    return 0;
}
