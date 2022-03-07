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
vector<string> setInput(string filename){
    ifstream inputfile;
    stringstream ss;
    string substring;
    string anotherstring;
    string temp;
    vector<string> info;
    int count =0;
    inputfile.open(filename);
    if (!inputfile.is_open()) {
        cout << "Unable to read file";
    }
    ss << inputfile.rdbuf();
    while (getline(ss, anotherstring)) {
        substring = anotherstring;
        stringstream iss(substring);
        getline(iss, temp, ' ');
        count = 0;
        while (count<3) {
            info.push_back(temp);
            getline(iss, temp, ' ');
            ++count;
        }
    }
    inputfile.close();
    return info;
}
int main() {

    string filename;
    vector<string> result;
    cout<<"Enter file name: ";
    cin>> filename;
    result = setInput(filename);
    for (int i = 0; i < result.size(); i++)
        cout << result[i] <<endl;
    return 0;
}
